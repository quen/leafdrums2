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
#ifndef WAVFILE_H
#define WAVFILE_H

#include <mmsystem.h>
#include "ByteBuffer.h"
#include "vorbis/codec.h"

// This class is provided to load and save WAV files. If you just need to do this,
// use the static functions. The dynamic functions allow you to load and save in
// any size blocks you like.

// Both functions work only on 44 kHz 16-bit stereo files. Any attempt to *load*
// anything else will return FailFormat; any attempt to *save* anything else 
// will just screw up. There is one exception: you can statically load mono
// files, which will automatically be converted to stereo

// LoadACM supports all .wav file formats that the ACM driver can read. All are
// converted to 44 kHz 16-bit stereo.

// When you open a dynamic file for writing, checks are made as usual; but this is
// not true when you *close* the file, which occurs in the destructor. At that
// point, errors are silently ignored.

class CWavFile
{
private:
	BOOL m_fLoad;
	int m_iSamples;
	int m_iSamplePos;
	HMMIO m_hmmio;

	BOOL m_fVorbis;
	FILE* m_fp;
	ogg_stream_state m_os;
	ogg_page m_og;
	ogg_packet m_op;
	vorbis_info m_vi;
	vorbis_comment m_vc;
	vorbis_dsp_state m_vd;
	vorbis_block m_vb;

	MMCKINFO m_mmckinfoParent; 
	MMCKINFO m_mmckinfoSubchunk; 

	static void ConvertFormat(CByteBuffer& bbSource,int iChannels,int iRate,CByteBuffer& bbTarget);

public:
	enum Return {OK,FailOpen,FailRead,FailWrite,FailFormat,FailMemory};
	static CString ErrorText(Return r);

	// Loads a WAV file, allocating memory as required.
	static Return Load(LPCTSTR strFilename,int& iSamples,short* &psData);
	static Return LoadACM(LPCTSTR strFilename,int& iSamples,short* &psData);
	static Return LoadVorbis(LPCTSTR strFilename,int& iSamples,short* &psData);

	// Saves a WAV file, overwriting any existing file
	static Return Save(LPCTSTR strFilename,int iSamples,short* psData);

	// Create a WAV file object for gradual access
	CWavFile();
	// Finalise any writing/reading
	~CWavFile();
	

	// Opens the file ready to read from it
	Return Load(LPCTSTR strFilename);

	// Actually reads data from the file (if you request more samples
	// than are available, fills with zeros)
	Return Read(short* psData,int iSamples);

	// Returns samples
	int SamplesLeft() { return m_iSamples-m_iSamplePos; }


	// Opens the file ready to write to it
	Return Save(LPCTSTR strFilename);
	Return SaveVorbis(LPCTSTR strFilename,float fQuality);

	// Writes data to the file
	Return Write(short* psData,int iSamples);
};

#endif // WAVFILE_H
