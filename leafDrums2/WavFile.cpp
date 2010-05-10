/*
    This file is part of leafDrums.

    leafDrums is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    leafDrums is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with leafDrums.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "stdafx.h"
#include "WavFile.h"
#include "mmreg.h"
#include "msacm.h"
#include "ByteBuffer.h"

#include "vorbis/vorbisfile.h"
#include "vorbis/vorbisenc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////////
// Static functions

CString CWavFile::ErrorText(CWavFile::Return r)
{
	CString strRet;

	switch(r)
	{
	case OK	: ASSERT(FALSE); break;
	case FailOpen :
		strRet="The .wav file could not be opened. Ensure that the file exists "
			"and that you're using its correct name. Note: the file may not load if "
			"it is across a network. If you are trying to write to the file, ensure "
			"the drive it's on is not write-protected or a read-only drive.";
		break;
	case FailRead:
		strRet="There was an error reading data from the .wav file. Ensure that "
			"the file is a valid .wav file and has not been corrupted.";
		break;
	case FailWrite:
		strRet="There was an error writing data to the .wav file. Ensure that "
			"you have plenty of disk space.";
		break;
	case FailFormat:
		strRet="The file you specified is not a valid 16-bit uncompressed .wav file. "
			"You can convert files to this format in Sound Recorder or another sound "
			"editing program.";
		break;
	case FailMemory:
		strRet="The WAV file could not be loaded due to insufficient memory. Check "
			"you have plenty of memory and disk space.";
		break;
	default:
		ASSERT(FALSE);
	}

	return strRet;
}

CWavFile::Return CWavFile::Save(LPCSTR strFilename,int iSamples,short* psData)
{
	// Open file
	HMMIO    hmmio;  
	MMCKINFO mmckinfoParent; 
	MMCKINFO mmckinfoSubchunk; 

	MMRESULT mmr;

	Return rVal=OK;

	hmmio=mmioOpen((char*)strFilename,
		NULL,MMIO_WRITE|MMIO_CREATE|MMIO_EXCLUSIVE);
	if(!hmmio)
		return FailOpen;

	try
	{
		// Create a "RIFF" chunk with a "WAVE" form type 
		FillMemory(&mmckinfoParent,sizeof(MMCKINFO),0);
		mmckinfoParent.fccType = mmioFOURCC('W','A','V','E'); 
		mmr=mmioCreateChunk(hmmio, 
			(LPMMCKINFO)&mmckinfoParent, MMIO_CREATERIFF);
		if(mmr!=MMSYSERR_NOERROR) throw FailWrite;

		// Write the "FMT" chunk 
		FillMemory(&mmckinfoSubchunk,sizeof(MMCKINFO),0);
		mmckinfoSubchunk.ckid = mmioFOURCC('f', 'm', 't', ' '); 
		mmr=mmioCreateChunk(hmmio, 
			(LPMMCKINFO)&mmckinfoSubchunk,0);
		if(mmr!=MMSYSERR_NOERROR) throw FailWrite;
			
		// Write WAV header
		WAVEFORMATEX wfe;

		wfe.wFormatTag=WAVE_FORMAT_PCM;
		wfe.nChannels=2;
		wfe.nSamplesPerSec=44100;
		wfe.nAvgBytesPerSec=44100*2*2;
		wfe.nBlockAlign=4;
		wfe.wBitsPerSample=16;
		wfe.cbSize=0;

		int iBytesWritten=mmioWrite(hmmio,
			(char*)&wfe,sizeof(wfe));
		if(iBytesWritten!=sizeof(wfe)) throw FailWrite;

		// Ascend out of the "FMT" subchunk. 
		mmr=mmioAscend(hmmio, &mmckinfoSubchunk, 0); 
		if(mmr!=MMSYSERR_NOERROR) throw FailWrite;

		// Write the "DATA" chunk 
		FillMemory(&mmckinfoSubchunk,sizeof(MMCKINFO),0);
		mmckinfoSubchunk.ckid = mmioFOURCC('d', 'a', 't', 'a'); 
		mmr=mmioCreateChunk(hmmio, 
			(LPMMCKINFO)&mmckinfoSubchunk, 0);
		if(mmr!=MMSYSERR_NOERROR) throw FailWrite;

		// Copy in actual data
		iBytesWritten=mmioWrite(hmmio,(char*)psData,iSamples*4);
		if(iBytesWritten!=iSamples*4) throw FailWrite;

		// Ascend out of the "DATA" subchunk
		mmr=mmioAscend(hmmio, &mmckinfoSubchunk, 0); 
		if(mmr!=MMSYSERR_NOERROR) throw FailWrite;

		// Ascend out of the "WAVE" chunk
		mmr=mmioAscend(hmmio, &mmckinfoParent, 0); 
		if(mmr!=MMSYSERR_NOERROR) throw FailWrite;
	}
	catch(Return r)
	{
		rVal=r;
	}

	// Close the file
	mmr=mmioClose(hmmio,0);
	if(mmr!=MMSYSERR_NOERROR) return FailWrite;

	return rVal;
}


CWavFile::Return CWavFile::LoadACM(LPCTSTR strFilename,int& iSamples,short* &psData)
{
	// Open file
	MMCKINFO mmckinfoParent; 
	MMCKINFO mmckinfoSubchunk; 
	MMRESULT mmr;

	HACMSTREAM has=0;

	Return rVal=OK;

	HMMIO hmmio=mmioOpen((char*)(strFilename),
		NULL,MMIO_READ | MMIO_DENYWRITE);
	if(hmmio==NULL)	return FailOpen;

	try
	{
		// Locate a "RIFF" chunk with a "WAVE" form type 
		mmckinfoParent.fccType = mmioFOURCC('W','A','V','E'); 
		if (mmioDescend(hmmio, (LPMMCKINFO)&mmckinfoParent, 
			NULL,MMIO_FINDRIFF))
			// The file is not a waveform-audio file. 
			throw FailRead;

		// Find the "FMT" chunk (form type "FMT"); it must be 
		// a subchunk of the "RIFF" chunk. 
		mmckinfoSubchunk.ckid = mmioFOURCC('f', 'm', 't', ' '); 
		if (mmioDescend(hmmio, &mmckinfoSubchunk, &mmckinfoParent, 
			MMIO_FINDCHUNK)) 
			throw FailRead;
			
		// Read WAV header
		CByteBuffer bbWaveFormat(mmckinfoSubchunk.cksize);

		int iBytesRead=mmioRead(hmmio,
			(char*)bbWaveFormat,bbWaveFormat.Size());
		if(iBytesRead!=bbWaveFormat.Size())
			throw FailRead;
		WAVEFORMATEX* pwfeSource=(WAVEFORMATEX*)(BYTE*)bbWaveFormat;

		// Ascend out of the "FMT" subchunk. 
		mmr=mmioAscend(hmmio, &mmckinfoSubchunk, 0); 
		if(mmr!=MMSYSERR_NOERROR) throw FailRead;

		// Find the data subchunk. The current file position should be at 
		// the beginning of the data chunk; however, you should not make 
		// this assumption. Use mmioDescend to locate the data chunk. 
		mmckinfoSubchunk.ckid = mmioFOURCC('d', 'a', 't', 'a'); 
		if (mmioDescend(hmmio, &mmckinfoSubchunk, &mmckinfoParent, 
			MMIO_FINDCHUNK)) 
			throw FailRead;

		// Load data subchunk
		CByteBuffer bbSource(mmckinfoSubchunk.cksize);
		iBytesRead=mmioRead(hmmio,(char*)bbSource,bbSource.Size());
		if(iBytesRead!=bbSource.Size()) throw FailRead;

		// Desired format
		WAVEFORMATEX wfeDest;
		wfeDest.wFormatTag=WAVE_FORMAT_PCM;
		wfeDest.nChannels=2;
		wfeDest.nSamplesPerSec=44100;
		wfeDest.wBitsPerSample=16;
		wfeDest.nBlockAlign=(wfeDest.nChannels * wfeDest.wBitsPerSample)/8;
		wfeDest.nAvgBytesPerSec=wfeDest.nSamplesPerSec*wfeDest.nBlockAlign;
		wfeDest.cbSize=0;

		// Two-stage conversion necessary for non-PCM files
		CByteBuffer bbIntermediate;
		WAVEFORMATEX wfeDestSuggested;
		if(pwfeSource->wFormatTag!=WAVE_FORMAT_PCM)
		{
			wfeDestSuggested.wFormatTag=WAVE_FORMAT_PCM;
			mmr=acmFormatSuggest(NULL,pwfeSource,&wfeDestSuggested,sizeof(wfeDestSuggested),
				ACM_FORMATSUGGESTF_WFORMATTAG);
			if(mmr!=0)
				throw FailFormat;

			// Prepare ACM conversion
			mmr=acmStreamOpen(&has,NULL,pwfeSource,&wfeDestSuggested,NULL,NULL,0,ACM_STREAMOPENF_NONREALTIME);
			if(mmr!=0) throw FailFormat;

			// Get size for output
			DWORD dwDestSize;
			if(acmStreamSize(has,bbSource.Size(),&dwDestSize,ACM_STREAMSIZEF_SOURCE))
				throw FailFormat;

			// Allocate output
			bbIntermediate.Create(dwDestSize);

			// Prepare stream
			ACMSTREAMHEADER ash;
			ash.cbStruct=sizeof(ash);
			ash.fdwStatus=0;
			ash.dwUser=0;
			ash.pbSrc=(BYTE*)bbSource;
			ash.cbSrcLength=bbSource.Size();
			ash.cbSrcLengthUsed=0;
			ash.dwSrcUser=0;
			ash.pbDst=(BYTE*)bbIntermediate;
			ash.cbDstLength=bbIntermediate.Size();
			ash.cbDstLengthUsed=0;
			ash.dwDstUser=0;
			if(acmStreamPrepareHeader(has,&ash,0))
				throw FailFormat;

			// Do conversion
			if(acmStreamConvert(has,&ash,ACM_STREAMCONVERTF_START | ACM_STREAMCONVERTF_END))
				throw FailFormat;

			// Clean up
			acmStreamUnprepareHeader(has,&ash,0);
			acmStreamClose(has,0);

			// Set source buffer
			pwfeSource=&wfeDestSuggested;
			bbIntermediate.Transfer(bbSource);
		}


		// Prepare ACM conversion
		mmr=acmStreamOpen(&has,NULL,pwfeSource,&wfeDest,NULL,NULL,0,ACM_STREAMOPENF_NONREALTIME);
		if(mmr!=0) throw FailFormat;

		// Get size for output
		DWORD dwDestSize;
		if(acmStreamSize(has,bbSource.Size(),&dwDestSize,ACM_STREAMSIZEF_SOURCE))
			throw FailFormat;

		// Allocate output
		psData=new short[dwDestSize/2];
		iSamples=dwDestSize/4;

		// Prepare stream
		ACMSTREAMHEADER ash;
		ash.cbStruct=sizeof(ash);
		ash.fdwStatus=0;
		ash.dwUser=0;
		ash.pbSrc=(BYTE*)bbSource;
		ash.cbSrcLength=bbSource.Size();
		ash.cbSrcLengthUsed=0;
		ash.dwSrcUser=0;
		ash.pbDst=(LPBYTE)psData;
		ash.cbDstLength=dwDestSize;
		ash.cbDstLengthUsed=0;
		ash.dwDstUser=0;
		if(acmStreamPrepareHeader(has,&ash,0))
			throw FailFormat;

		// Do conversion
		if(acmStreamConvert(has,&ash,ACM_STREAMCONVERTF_START | ACM_STREAMCONVERTF_END))
			throw FailFormat;

		// Clean up
		acmStreamUnprepareHeader(has,&ash,0);

		// Return val already initialised to OK
	}
	catch(Return r)
	{
		rVal=r;
	}
	catch(CMemoryException* e)
	{
		e->Delete();
		rVal=FailMemory;
	}

	if(has!=0)
		acmStreamClose(has,0);
	mmioClose(hmmio,0);
	hmmio=NULL;

	return rVal;

}

// Converts format of data (appending to target buffer)
void CWavFile::ConvertFormat(CByteBuffer& bbSource,int iChannels,int iRate,CByteBuffer& bbTarget)
{
	// Shortcut for the same size
	if(iChannels==2 && iRate==44100)
	{
		int iSizeBefore=bbTarget.Size();
		bbTarget.Resize(bbTarget.Size()+bbSource.Size());
		CopyMemory(&((BYTE*)bbTarget)[iSizeBefore],(BYTE*)bbSource,bbSource.Size());
		return;
	}


	WAVEFORMATEX wfeSource;
	wfeSource.wFormatTag=WAVE_FORMAT_PCM;
	wfeSource.nChannels=iChannels;
	wfeSource.nSamplesPerSec=iRate;
	wfeSource.wBitsPerSample=16;
	wfeSource.nBlockAlign=(wfeSource.nChannels * wfeSource.wBitsPerSample)/8;
	wfeSource.nAvgBytesPerSec=wfeSource.nSamplesPerSec*wfeSource.nBlockAlign;
	wfeSource.cbSize=0;

	WAVEFORMATEX wfeDest;
	wfeDest.wFormatTag=WAVE_FORMAT_PCM;
	wfeDest.nChannels=2;
	wfeDest.nSamplesPerSec=44100;
	wfeDest.wBitsPerSample=16;
	wfeDest.nBlockAlign=(wfeDest.nChannels * wfeDest.wBitsPerSample)/8;
	wfeDest.nAvgBytesPerSec=wfeDest.nSamplesPerSec*wfeDest.nBlockAlign;
	wfeDest.cbSize=0;

	// Prepare ACM conversion
	HACMSTREAM has;
	MMRESULT mmr=acmStreamOpen(&has,NULL,&wfeSource,&wfeDest,NULL,NULL,0,ACM_STREAMOPENF_NONREALTIME);
	if(mmr!=0) throw FailFormat;

	try
	{
		// Get size for output
		DWORD dwDestSize;
		if(acmStreamSize(has,bbSource.Size(),&dwDestSize,ACM_STREAMSIZEF_SOURCE))
			throw FailFormat;

		// Allocate output
		int iSizeBefore=bbTarget.Size();
		bbTarget.Resize(bbTarget.Size()+dwDestSize);

		// Prepare stream
		ACMSTREAMHEADER ash;
		ash.cbStruct=sizeof(ash);
		ash.fdwStatus=0;
		ash.dwUser=0;
		ash.pbSrc=(BYTE*)bbSource;
		ash.cbSrcLength=bbSource.Size();
		ash.cbSrcLengthUsed=0;
		ash.dwSrcUser=0;
		ash.pbDst=&((BYTE*)bbTarget)[iSizeBefore];
		ash.cbDstLength=dwDestSize;
		ash.cbDstLengthUsed=0;
		ash.dwDstUser=0;
		if(mmr=acmStreamPrepareHeader(has,&ash,0))
			throw FailFormat;

		// Do conversion
		if(acmStreamConvert(has,&ash,ACM_STREAMCONVERTF_START | ACM_STREAMCONVERTF_END))
			throw FailFormat;

		// Clean up
		acmStreamUnprepareHeader(has,&ash,0);
		acmStreamClose(has,0);
	}
	catch(Return r)
	{
		acmStreamClose(has,0);
		throw r;
	}
}

CWavFile::Return CWavFile::LoadVorbis(LPCTSTR strFilename,int& iSamples,short* &psData)
{
	Return rVal=OK;

	FILE* fp;
	OggVorbis_File vf;
	BOOL bOpened=FALSE;

	try
	{
		// Open file
		fp=fopen(strFilename,"rb");
		if(fp==NULL) throw FailRead;

		// Open stream with Vorbis decoder
		if(ov_open(fp, &vf, NULL, 0) < 0) throw FailFormat;
		bOpened=TRUE;

		// Buffer for Vorbis decoded output
		CByteBuffer bbPCM;
		// # channels and sample rate of current buffer
		int iChannels=0,iSampleRate=0;

		// Buffer for 44kHz 16-bit stereo
		CByteBuffer bbReformatted;

		// Read file into buffer
		int iBitstream;
		BYTE abBuffer[4096];
		while(TRUE)
		{
			long lReadBytes=ov_read(&vf,(char*)abBuffer,sizeof(abBuffer),0,2,1,&iBitstream);
			if (lReadBytes == 0) 
			{
				// EOF

				// Flush data into reformatted buffer
				if(bbPCM.Size()>0) ConvertFormat(bbPCM,iChannels,iSampleRate,bbReformatted);
				break;
			} 
			else if (lReadBytes < 0) 
			{
				// Vorbis reporting a glitch in file, which we ignore
		    } 
			else 
			{
				// Check if samplerate changed
				vorbis_info* pvi=ov_info(&vf,iBitstream);
				if(pvi->channels!=iChannels || pvi->rate!=iSampleRate)
				{
					if(bbPCM.Size()>0)
					{
						// Flush previous buffer to reformatted
						ConvertFormat(bbPCM,iChannels,iSampleRate,bbReformatted);
						bbPCM.Free();
					}

					iChannels=pvi->channels;
					iSampleRate=pvi->rate;
				}

				// Write to PCM buffer
				int iBefore=bbPCM.Size();
				bbPCM.Resize(iBefore + lReadBytes);
				CopyMemory(&((BYTE*)bbPCM)[iBefore],abBuffer,lReadBytes);
			}
		}

		// OK, done, yay! Set destination
		iSamples=bbReformatted.Size()/4;
		psData=(short*)bbReformatted.Detach();
	}
	catch(Return r)
	{
		rVal=r;
	}

	if(bOpened)
		ov_clear(&vf); // Also closes file
	else if(fp!=NULL) 
		fclose(fp);

	return rVal;
}

CWavFile::Return CWavFile::Load(LPCTSTR strFilename,int& iSamples,short* &psData)
{
	// Open file
	MMCKINFO mmckinfoParent; 
	MMCKINFO mmckinfoSubchunk; 
	MMRESULT mmr;

	Return rVal=OK;

	HMMIO hmmio=mmioOpen((char*)(strFilename),
		NULL,MMIO_READ | MMIO_DENYWRITE);
	if(hmmio==NULL)	return FailOpen;

	try
	{
		// Locate a "RIFF" chunk with a "WAVE" form type 
		mmckinfoParent.fccType = mmioFOURCC('W','A','V','E'); 
		if (mmioDescend(hmmio, (LPMMCKINFO)&mmckinfoParent, 
			NULL,MMIO_FINDRIFF))
			// The file is not a waveform-audio file. 
			throw FailRead;

		// Find the "FMT" chunk (form type "FMT"); it must be 
		// a subchunk of the "RIFF" chunk. 
		mmckinfoSubchunk.ckid = mmioFOURCC('f', 'm', 't', ' '); 
		if (mmioDescend(hmmio, &mmckinfoSubchunk, &mmckinfoParent, 
			MMIO_FINDCHUNK)) 
			throw FailRead;
			
		// Read WAV header
		WAVEFORMATEX wfe;
		int iBytesRead=mmioRead(hmmio,
			(char*)&wfe,sizeof(wfe));
		if(iBytesRead!=sizeof(wfe))
			throw FailRead;

		// Make sure none of that nasty compression
		if(wfe.wFormatTag!=WAVE_FORMAT_PCM) 
			throw FailFormat;

		// See if file needs converting
		if(wfe.wBitsPerSample!=16)
			throw FailFormat;

		BOOL fResample=FALSE;
		float fFactor=1.0;
		if(wfe.nSamplesPerSec!=44100)
		{
			fResample=TRUE;
			fFactor=(float)44100 / (float)wfe.nSamplesPerSec;
		}

		BOOL fMono=FALSE;
		if(wfe.nChannels!=2)
			fMono=TRUE;

		// Ascend out of the "FMT" subchunk. 
		mmr=mmioAscend(hmmio, &mmckinfoSubchunk, 0); 
		if(mmr!=MMSYSERR_NOERROR) throw FailRead;

		// Find the data subchunk. The current file position should be at 
		// the beginning of the data chunk; however, you should not make 
		// this assumption. Use mmioDescend to locate the data chunk. 
		mmckinfoSubchunk.ckid = mmioFOURCC('d', 'a', 't', 'a'); 
		if (mmioDescend(hmmio, &mmckinfoSubchunk, &mmckinfoParent, 
			MMIO_FINDCHUNK)) 
			throw FailRead;

		if(!fMono)
		{
			// Number of samples considering stereo, 16-bit
			iSamples=mmckinfoSubchunk.cksize/4;

			// Allocate memory using shorts
			psData=new short[iSamples*2];

			// Load actual data
			iBytesRead=mmioRead(hmmio,(char*)psData,iSamples*4);
			if(iBytesRead!=iSamples*4) throw FailRead;
		}
		else
		{
			// Number of samples for mono
			iSamples=mmckinfoSubchunk.cksize/2;

			// Allocate memory using shorts
			psData=new short[iSamples*2];

			// Load actual data
			iBytesRead=mmioRead(hmmio,(char*)psData,iSamples*2);
			if(iBytesRead!=iSamples*2) throw FailRead;

			// Convert data to stereo
			for(int i=iSamples-1;i>=0;i--)
			{
				psData[i*2+1]=psData[i*2]=psData[i];
			}
		}

		if(fResample)
		{
			int iNewSamples=(int)((float)iSamples * fFactor);
			short* psNewData=new short[iNewSamples*2];

			double fOrigPos=double(0.0),
				  fOrigPlus=double(iSamples) / (iNewSamples);
			for(int i=0;i<iNewSamples;i++)
			{
				int iPos=int(fOrigPos),iNextPos=iPos+1;
				if(iNextPos>=iSamples) 
				{
					iNextPos-=iSamples;
					if(iPos>=iSamples)
						iPos-=iSamples;
				}

				double fAmount=fOrigPos-double(int(fOrigPos));

				psNewData[i*2]=short(
					double(psData[iPos*2])*(1-fAmount)+
					double(psData[iNextPos*2])*fAmount);
				psNewData[i*2+1]=short(
					double(psData[iPos*2+1])*(1-fAmount)+
					double(psData[iNextPos*2+1])*fAmount);

				fOrigPos+=fOrigPlus;
			}

			delete psData;
			psData=psNewData;
			iSamples=iNewSamples;
		}
	}
	catch(Return r)
	{
		rVal=r;
	}
	catch(CMemoryException* e)
	{
		e->Delete();
		rVal=FailMemory;
	}

	mmioClose(hmmio,0);
	hmmio=NULL;

	return rVal;
}

//////////////////////////////////////////////////////////////////////////////
// Class functions

CWavFile::CWavFile()
{
	m_hmmio=NULL;
	m_fVorbis=FALSE;
}

CWavFile::~CWavFile()
{
	if(m_fVorbis)
	{
		// Flush remaining samples (...or something)
		Write(NULL,0);

		ogg_stream_clear(&m_os);
		vorbis_block_clear(&m_vb);
		vorbis_dsp_clear(&m_vd);
		vorbis_comment_clear(&m_vc);
		vorbis_info_clear(&m_vi); // Must be called last

		fclose(m_fp);
	}
	else
	{
		if(m_hmmio)
		{
			if(!m_fLoad)
			{
				MMRESULT mmr;

				// Ascend out of the "DATA" subchunk
				mmr=mmioAscend(m_hmmio, &m_mmckinfoSubchunk, 0); 

				// Ascend out of the "WAVE" chunk
				mmr=mmioAscend(m_hmmio, &m_mmckinfoParent, 0); 
			}

			mmioClose(m_hmmio,0);
			m_hmmio=NULL;
		}
	}
}

CWavFile::Return CWavFile::Load(LPCTSTR strFilename)
{
	// Open file
	MMCKINFO mmckinfoParent; 
	MMCKINFO mmckinfoSubchunk; 
	MMRESULT mmr;

	Return rVal=OK;

	m_hmmio=mmioOpen((char*)(strFilename),
		NULL,MMIO_READ | MMIO_DENYWRITE);
	if(m_hmmio==NULL) return FailOpen;

	try
	{
		// Locate a "RIFF" chunk with a "WAVE" form type 
		mmckinfoParent.fccType = mmioFOURCC('W','A','V','E'); 
		if (mmioDescend(m_hmmio, (LPMMCKINFO)&mmckinfoParent, 
			NULL,MMIO_FINDRIFF))
			// The file is not a waveform-audio file. 
			throw FailRead;

		// Find the "FMT" chunk (form type "FMT"); it must be 
		// a subchunk of the "RIFF" chunk. 
		mmckinfoSubchunk.ckid = mmioFOURCC('f', 'm', 't', ' '); 
		if (mmioDescend(m_hmmio, &mmckinfoSubchunk, &mmckinfoParent, 
			MMIO_FINDCHUNK)) 
			throw FailRead;
			
		// Read WAV header
		WAVEFORMATEX wfe;
		int iBytesRead=mmioRead(m_hmmio,
			(char*)&wfe,sizeof(wfe));
		if(iBytesRead!=sizeof(wfe))
			throw FailRead;

		// Make sure none of that nasty compression
		if(wfe.wFormatTag!=WAVE_FORMAT_PCM) 
			throw FailFormat;

		// See if file needs converting
		if(wfe.nChannels!=2 || wfe.wBitsPerSample!=16)
			throw FailFormat;

		// Ascend out of the "FMT" subchunk. 
		mmr=mmioAscend(m_hmmio, &mmckinfoSubchunk, 0); 
		if(mmr!=MMSYSERR_NOERROR) throw FailRead;

		// Find the data subchunk. The current file position should be at 
		// the beginning of the data chunk; however, you should not make 
		// this assumption. Use mmioDescend to locate the data chunk. 
		mmckinfoSubchunk.ckid = mmioFOURCC('d', 'a', 't', 'a'); 
		if (mmioDescend(m_hmmio, &mmckinfoSubchunk, &mmckinfoParent, 
			MMIO_FINDCHUNK)) 
			throw FailRead;

		// Number of samples considering stereo, 16-bit
		m_iSamples=mmckinfoSubchunk.cksize/4;
		m_iSamplePos=0;

		m_fLoad=TRUE;
	}
	catch(Return r)
	{
		rVal=r;
	}

	return rVal;
}

CWavFile::Return CWavFile::Read(short* psData,int iSamples)
{
	// Check we're in a sensible state
	ASSERT(m_fLoad);
	ASSERT(m_hmmio);

	// Figure out if that many samples are left
	int iFillIn=0;
	if(iSamples>SamplesLeft())
	{
		iFillIn=iSamples-SamplesLeft();
		iSamples=SamplesLeft();
	}

	// Load actual data
	if(iSamples>0)
	{
		int iBytesRead=mmioRead(m_hmmio,(char*)psData,iSamples*4);
		if(iBytesRead!=iSamples*4) return FailRead;

		m_iSamplePos+=iSamples;
	}

	// Fill in the blanks
	FillMemory((BYTE*)(&psData[iSamples*2]),iFillIn*4,0);

	return OK;
}

CWavFile::Return CWavFile::Save(LPCTSTR strFilename)
{
	m_fVorbis=FALSE;

	// Open file
	MMRESULT mmr;

	Return rVal=OK;

	m_hmmio=mmioOpen((char*)strFilename,
		NULL,MMIO_WRITE|MMIO_CREATE|MMIO_EXCLUSIVE);
	if(!m_hmmio) return FailOpen;

	try
	{
		// Create a "RIFF" chunk with a "WAVE" form type 
		FillMemory(&m_mmckinfoParent,sizeof(MMCKINFO),0);
		m_mmckinfoParent.fccType = mmioFOURCC('W','A','V','E'); 
		mmr=mmioCreateChunk(m_hmmio, 
			(LPMMCKINFO)&m_mmckinfoParent, MMIO_CREATERIFF);
		if(mmr!=MMSYSERR_NOERROR) throw FailWrite;

		// Write the "FMT" chunk 
		FillMemory(&m_mmckinfoSubchunk,sizeof(MMCKINFO),0);
		m_mmckinfoSubchunk.ckid = mmioFOURCC('f', 'm', 't', ' '); 
		mmr=mmioCreateChunk(m_hmmio, 
			(LPMMCKINFO)&m_mmckinfoSubchunk,0);
		if(mmr!=MMSYSERR_NOERROR) throw FailWrite;
			
		// Write WAV header
		WAVEFORMATEX wfe;

		wfe.wFormatTag=WAVE_FORMAT_PCM;
		wfe.nChannels=2;
		wfe.nSamplesPerSec=44100;
		wfe.nAvgBytesPerSec=44100*2*2;
		wfe.nBlockAlign=4;
		wfe.wBitsPerSample=16;
		wfe.cbSize=0;

		int iBytesWritten=mmioWrite(m_hmmio,
			(char*)&wfe,sizeof(wfe));
		if(iBytesWritten!=sizeof(wfe)) throw FailWrite;

		// Ascend out of the "FMT" subchunk. 
		mmr=mmioAscend(m_hmmio, &m_mmckinfoSubchunk, 0); 
		if(mmr!=MMSYSERR_NOERROR) throw FailWrite;

		// Write the "DATA" chunk 
		FillMemory(&m_mmckinfoSubchunk,sizeof(MMCKINFO),0);
		m_mmckinfoSubchunk.ckid = mmioFOURCC('d', 'a', 't', 'a'); 
		mmr=mmioCreateChunk(m_hmmio, 
			(LPMMCKINFO)&m_mmckinfoSubchunk, 0);
		if(mmr!=MMSYSERR_NOERROR) throw FailWrite;

		m_fLoad=FALSE;
		m_iSamples=0;
		m_iSamplePos=0;
	}
	catch(Return r)
	{
		rVal=r;
	}

	return rVal;
}

CWavFile::Return CWavFile::Write(short* psData,int iSamples)
{
	if(m_fVorbis)
	{
		// For EOF we actually call this with a zero, I guess to
		// flush stuff
		if(iSamples!=0)
		{
			// Get Vorbis buffer to put data into
			float **buffer=vorbis_analysis_buffer(&m_vd,iSamples);
			for(int i=0;i<iSamples;i++)
			{
				// Uninterleave samples
				buffer[0][i]=((float)psData[i*2])/32768.0f;
				buffer[1][i]=((float)psData[i*2+1])/32768.0f;
			}
		}

		// Tell the library how much we actually submitted
		vorbis_analysis_wrote(&m_vd,iSamples);

		// Loop around output blocks 
		while(vorbis_analysis_blockout(&m_vd,&m_vb)==1)
		{
			// Analysis, assume we want to use bitrate management
			vorbis_analysis(&m_vb,NULL);
			vorbis_bitrate_addblock(&m_vb);

			while(vorbis_bitrate_flushpacket(&m_vd,&m_op))
			{
				// Put packet into bitstream
				ogg_stream_packetin(&m_os,&m_op);

				// Write out any ogg pages now available
				while(TRUE)
				{
					int result=ogg_stream_pageout(&m_os,&m_og);
					if(result==0) break;

					if(fwrite(m_og.header,1,m_og.header_len,m_fp)!=(size_t)m_og.header_len) throw CWavFile::FailWrite;
					if(fwrite(m_og.body,1,m_og.body_len,m_fp)!=(size_t)m_og.body_len) throw CWavFile::FailWrite;
				}
			}
		}
	}
	else
	{
		ASSERT(m_hmmio);
		ASSERT(!m_fLoad);

		int iBytesWritten=mmioWrite(m_hmmio,(char*)psData,iSamples*4);
		if(iBytesWritten!=iSamples*4) return FailWrite;

		m_iSamples+=iSamples;
		m_iSamplePos+=iSamples;
	}

	return OK;
}

CWavFile::Return CWavFile::SaveVorbis(LPCTSTR strFilename,float fQuality)
{
	m_fVorbis=TRUE;

	// Open file
	m_fp=fopen(strFilename,"wb");
	if(!m_fp) return FailOpen;

	try
	{
		// Initialise vorbis_info
		vorbis_info_init(&m_vi);
		if(vorbis_encode_init_vbr(&m_vi,2,44100,fQuality)<0) throw CWavFile::FailFormat;

		// Initialise other data
		vorbis_comment_init(&m_vc);
		vorbis_analysis_init(&m_vd,&m_vi);
		vorbis_block_init(&m_vd,&m_vb);  
		ogg_stream_init(&m_os,42);

		// Create three initial headers
		ogg_packet header;
		ogg_packet header_comm;
		ogg_packet header_code;
		vorbis_analysis_headerout(&m_vd,&m_vc,&header,&header_comm,&header_code);

		// Chuck them into ogg packets
		ogg_stream_packetin(&m_os,&header); 
		ogg_stream_packetin(&m_os,&header_comm);
		ogg_stream_packetin(&m_os,&header_code);

		// Need to flush header data to ensure that actual audio starts
		// on a new 'page'
		while(true)
		{
			int result=ogg_stream_flush(&m_os,&m_og);
			if(result==0) break;
			if(fwrite(m_og.header,1,m_og.header_len,m_fp)!=(size_t)m_og.header_len) throw CWavFile::FailWrite;
			if(fwrite(m_og.body,1,m_og.body_len,m_fp)!=(size_t)m_og.body_len) throw CWavFile::FailWrite;
		}
	}
	catch(Return r)
	{
		// Ensure file isn't left open
		fclose(m_fp);
		return r;
	}
}


