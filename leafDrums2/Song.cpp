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
#include "Song.h"

#include "DSoundPlayDevice.h"
#include "WavFile.h"

#include "LeafDrumsView.h"

#include "FileSystem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Construct
CSong::CSong()
{
	m_iLevel=128;
	m_bLoop=TRUE;
	m_bPlaying=FALSE;
	
	m_iStartBar=0;
	m_iStopBar=1;

	// Default to one pattern, track, and sound initially
	GetTimeChange(0).SetTempo(120);
	GetTimeChange(0).SetTimeSignature(4);
	AddPattern();
	GetPattern(0).SetName("1");
	AddTrack();
	AddTrack();
	GetTrack(0).SetBar(*this,0,&GetPattern(0));
	AddSound();
}

// Plays a section of the given bar, beginning at sample offset and 
// for length given, into the target sample, overwriting anything that's
// already there. Returns max playback level
int CSong::Play(CSample& sTarget,int iTargetOffset,
	SongLocation sl,int iOffset,int iLength,CPlayLimits& pl) 
{
	INTERNAL_SYNCHRONIZE; 

	// Create buffer
	CMixBuffer mb;
	mb.Create(iLength);

	// Mix all tracks into buffer
	for(int iTrack=0;iTrack<m_vtTracks.Size();iTrack++)
	{
		if(pl.PlayTrack(iTrack))
			m_vtTracks[iTrack].Play(mb,0,sl,iOffset,iLength,
				*this,m_iLevel,m_iLevel,pl);
	}

	// Copy from mixbuffer to sample
	DWORD dwSource=(DWORD)mb.Data();
	DWORD dwTarget=(DWORD)sTarget.Data() + (iTargetOffset * 2*sizeof(WORD));

	int iMin,iMax;

	_asm
	{
		push esi
		push edi

		// Set up registers
		mov esi,dwSource
		mov edi,dwTarget
		mov ecx,iLength
		shl ecx,1		// Double it for stereo
		mov ebx,0	// Max
		mov edx,0	// Min

	fadeclipLoop:
		// Get sample
		mov eax,[esi]

		// Clip
		cmp eax,32767
		jg clipHigh

		cmp eax,-32768
		jl clipLow

	afterClip:
		cmp eax,ebx
		jg newMax

		cmp eax,edx
		jl newMin

	afterMax:
		mov [edi],ax

		add edi,2
		add esi,4

		dec ecx
		jnz fadeclipLoop

		jmp endLoop

	clipHigh:
		mov eax,32767
		jmp afterClip

	clipLow:
		mov eax,-32768
		jmp afterClip

	newMax:
		mov ebx,eax
		jmp afterMax

	newMin:
		mov edx,eax
		jmp afterMax

	endLoop:
		mov iMin,edx
		mov iMax,ebx
		pop edi
		pop esi
	}

	if(-iMin > iMax)
		return -iMin;
	else
		return iMax;
}

void CSong::Serialize(CArchive& a)
{
	INTERNAL_SYNCHRONIZE; 

	int iVersion=4;

	if(a.IsStoring())
	{
		a << iVersion;

		a << m_iLevel;
		a << m_bLoop;
		a << m_iStartBar;
		a << m_iStopBar;

		CFileSystem::sm_pCurrent->SaveSounds(a,*this,m_bEmbedSounds);

		a << GetNumSounds();
		for(int iSound=0;iSound<GetNumSounds();iSound++)
			GetSound(iSound).Serialize(a,*this);
		a << GetNumPatterns();
		for(int iPattern=0;iPattern<GetNumPatterns();iPattern++)
			GetPattern(iPattern).Serialize(a);
		a << GetNumTracks();
		for(int iTrack=0;iTrack<m_vtTracks.Size();iTrack++)
			GetTrack(iTrack).Serialize(a,*this);

		// Find out how many coda there are (ignore blank ones)
		int iCodaLength=m_vcCoda.Size();
		while(iCodaLength>0 && m_vcCoda[iCodaLength-1].IsEmpty()) iCodaLength--;

		// Write coda
		a << iCodaLength;
		for(int iCoda=0;iCoda<iCodaLength;iCoda++)
			m_vcCoda[iCoda].Serialize(a);

		// Write sound map
		int iMapSize=m_viSoundMap.Size();
		a << iMapSize;
		for(int iMap=0;iMap<iMapSize;iMap++)
		{
			a << m_viSoundMap[iMap];
		}

		// Write time changes
		int iTimeSize=m_vtcTime.Size();
		a << iTimeSize;
		for(int iTime=0;iTime<iTimeSize;iTime++)
		{
			m_vtcTime[iTime].Serialize(a);
		}
	}
	else
	{
		a >> iVersion;
		if(iVersion!=1 && iVersion!=2 && iVersion!=3 && iVersion!=4)
			throw "File format not supported. Make sure this is a valid file and you are "
			  "using the latest version of leafDrums.";


		int iOldTempo=0,iOldTimeSignature=0;
		if(iVersion<4) a >> iOldTempo;
		a >> m_iLevel;
		if(iVersion<4) a >> iOldTimeSignature;
		a >> m_bLoop;
		a >> m_iStartBar;
		a >> m_iStopBar;

		m_vtcTime.Clear();
		if(iVersion<4)
		{
			CTimeChange* ptc=new CTimeChange(this);
			m_vtcTime.Add(ptc); // Must do this before setting the tempo/timesig
			ptc->SetTimeSignature(iOldTimeSignature);
			ptc->SetTempo(iOldTempo);
		}

		CFileSystem::sm_pCurrent->LoadSounds(a);

		int iCount;
		a >> iCount;
		m_vsSounds.Clear();
		for(int iSound=0;iSound<iCount;iSound++)
		{
			CSound* pSound=new CSound();
			pSound->Serialize(a,*this);
			m_vsSounds.Add(pSound);
		}
		a >> iCount;
		m_vpPatterns.Clear();
		for(int iPattern=0;iPattern<iCount;iPattern++)
		{
			CPattern* pPattern=new CPattern(this);
			pPattern->Serialize(a);
			m_vpPatterns.Add(pPattern);
		}
		a >> iCount;
		m_vtTracks.Clear();
		for(int iTrack=0;iTrack<iCount;iTrack++)
		{
			CTrack* pTrack=new CTrack(this);
			pTrack->Serialize(a,*this);
			m_vtTracks.Add(pTrack);
		}

		m_vcCoda.Clear();
		if(iVersion>=2)
		{
			a >> iCount;
			for(int iCoda=0;iCoda<iCount;iCoda++)
			{
				CCoda* pCoda=new CCoda(this);
				pCoda->Serialize(a);
				m_vcCoda.Add(pCoda);
			}
		}

		m_viSoundMap.Clear();
		if(iVersion>=3)
		{
			a >> iCount;
			for(int iMap=0;iMap<iCount;iMap++)
			{
				int iMapEntry;
				a >> iMapEntry;
				m_viSoundMap.Add(iMapEntry);
			}
		}

		if(iVersion>=4)
		{
			a >> iCount;
			for(int iTime=0;iTime<iCount;iTime++)
			{
				CTimeChange* ptc=new CTimeChange(this);
				ptc->Serialize(a);
				m_vtcTime.Add(ptc);
			}
		}

		m_vslLinear.Clear();
	}	
}

int CSong::GetSlowestTempo()
{
	// Check through all time changes
	int iMinTempo=0x7fffffff;
	for(int i=0;i<GetBarCount();i++)
	{
		CTimeChange& tc=GetTimeChange(i);
		if(!tc.IsEmpty())
		{
			int iThisTempo=tc.GetTempo();
			if(iThisTempo < iMinTempo) iMinTempo=iThisTempo;
		}
	}
	return iMinTempo;
}

BOOL g_fQuitTrackPlayThread;

UINT TrackPlayThread(LPVOID lpvParam)
{
	CSong* pThis=(CSong*)lpvParam;

	CInternalSynchLock isl(&(pThis->m_isoLock),FALSE);

	try
	{
		// Create sound device
		CDSoundPlayDevice dsd;

		// Data for track position
		int iSixteenth=0;
		SongLocation slLocation=pThis->GetStartLocation(); 

		// Data to hold beats
		CSample sample;

		// Hold previous play levels (circular buffer)
		const int PLAYLEVELBUFFER=64;
		int aiPlayLevel[PLAYLEVELBUFFER];
		for(int iPlayLevelPos=0;iPlayLevelPos<PLAYLEVELBUFFER;iPlayLevelPos++)
			aiPlayLevel[iPlayLevelPos]=0;
		iPlayLevelPos=0;

		// Size of play-ahead requirement (at least 100ms + 1/4 beat at slowest tempo)
		DWORD dwPlayAhead=250;
		DWORD dwSixteenthms=15000 / pThis->GetSlowestTempo();
		if(dwSixteenthms > dwPlayAhead)
			dwPlayAhead=dwSixteenthms+100;

		while(TRUE)
		{
			isl.Lock();

			// Work out position of this block
			int iStartPos=(iSixteenth * pThis->GetBarLength(slLocation))/16;
			int iNextPos=((iSixteenth+1) * pThis->GetBarLength(slLocation))/16;
			int iBlockSize=iNextPos-iStartPos;

			// Make sure bar position is ok
			if(slLocation.Stop()) 
			{
				isl.Unlock();
				dsd.StopAfterBuffers();
				break;
			}

			// If it's still too long, then stop and start must be same? bail
			if(slLocation==pThis->GetEndLocation())
			{
				isl.Unlock();
				dsd.StopAfterBuffers();
				break;
			}

			// Create a block
			CBlock* pBlock=CBlock::Create(TRUE,iBlockSize);

			// Make the sample use this
			sample.Set(pBlock->Data(),iBlockSize);

			// Play a beat to the sample
			int iLevel=pThis->Play(sample,0,slLocation,iStartPos,iBlockSize,pThis->m_pl);

			isl.Unlock();

			aiPlayLevel[iPlayLevelPos++]=iLevel;
			if(iPlayLevelPos >= PLAYLEVELBUFFER) iPlayLevelPos=0;

			// Clear the sample so it doesn't attempt to delete
			sample.Detach();

			// Give the sample to the sound driver (which deletes it)
			dsd.PlayBuffer(pBlock);

			// See if we should wait to play next buffer
			if(dsd.GetPlayEndTime()!=0)
			{
				while(true)
				{
					DWORD 
						dwEndTime=dsd.GetPlayEndTime(),
						dwNowTime=GetTickCount();

					DWORD dwWait;
					if(dwEndTime<dwNowTime) 
						dwWait=0;
					else
						dwWait=dwEndTime-dwNowTime;

					if(dwWait>10000) 
					{
						CString strError="Error, wait too high.";
						throw strError;
					}

#ifdef DEBUG
					if(dwWait<50)
						TRACE("Wait delay dropped below 50ms.\n");
#endif //DEBUG

					DWORD dwSixteenthTime=(iBlockSize*10)/441;

					// Calculate the current playback position based on this
					isl.Lock();
					int iCurrentSixteenth=iSixteenth-dwWait/dwSixteenthTime;
					SongLocation slCurrentLocation=slLocation;
					while(iCurrentSixteenth<0)
					{
						iCurrentSixteenth+=16;

						if(slCurrentLocation == pThis->GetStartLocation())
							slCurrentLocation=pThis->GetBeforeEndLocation();
						else
							slCurrentLocation=pThis->GetPreviousBar(slCurrentLocation);
					}
					int iCurrentPlayLevelPos=(iPlayLevelPos-1)-dwWait/dwSixteenthTime;
					while(iCurrentPlayLevelPos<0)
						iCurrentPlayLevelPos+=PLAYLEVELBUFFER;

					pThis->SetCurrentPlayData(slCurrentLocation.iBar,iCurrentSixteenth,
						aiPlayLevel[iCurrentPlayLevelPos]);
					isl.Unlock();

					if(dwWait < dwPlayAhead) break;

					DWORD dwSleep=dwWait-dwPlayAhead;
					if(dwSleep>dwSixteenthTime/4)
						dwSleep=dwSixteenthTime/4;

					if(g_fQuitTrackPlayThread) break;
					Sleep(dwSleep);
				}
			}

			// Go to next sixteenth
			isl.Lock();
			iSixteenth++;
			if(iSixteenth>15)
			{
				iSixteenth=0; 
				if(slLocation==pThis->GetBeforeEndLocation())
				{
					if(pThis->IsLoop())
						slLocation=pThis->GetStartLocation();
					else
						slLocation.iBar=-1;
				}
				else
					slLocation=pThis->GetNextBar(slLocation);
			}
			isl.Unlock();

			if(g_fQuitTrackPlayThread)
			{
				// Stop immediately
				dsd.StopNow();
				break;
			}
		}
	}
	catch(CString strError)
	{
		AfxGetMainWnd()->MessageBox(
			strError,"Sound device open failure",
			MB_ICONEXCLAMATION|MB_OK);		
//		AfxGetMainWnd()->PostMessage(WM_COMMAND,
//			IDC_STOP | (BN_CLICKED<<16),0);
	}
	catch(char* strError)
	{
		AfxGetMainWnd()->MessageBox(
			strError,"Sound device error",
			MB_ICONEXCLAMATION|MB_OK);		
//		AfxGetMainWnd()->PostMessage(WM_COMMAND,
//			IDC_STOP | (BN_CLICKED<<16),0);
	}

	g_fQuitTrackPlayThread=FALSE;
	pThis->PlayingStopped();

#ifdef _DEBUG
	CString sTemp;
	sTemp.Format("Ended play thread: 0x%x\n",GetCurrentThreadId());
	TRACE(sTemp);
#endif
	return 0;
}

void CSong::Start()
{
	INTERNAL_SYNCHRONIZE;

	if(m_bPlaying) return;

	SetPriorityClass(GetCurrentProcess(),HIGH_PRIORITY_CLASS);
	m_bPlaying=TRUE;
	m_iCurrentMaxLevel=0; m_iCurrentBar=m_iStartBar; m_iCurrentSixteenth=0;
	CWinThread* pPlayThread=AfxBeginThread(&TrackPlayThread,this);
	pPlayThread->SetThreadPriority(THREAD_PRIORITY_NORMAL);

#ifdef _DEBUG
	CString sTemp;
	sTemp.Format("Started play thread: 0x%x\n",pPlayThread->m_nThreadID);
	TRACE(sTemp);
#endif
}

void CSong::Stop()
{
	if(!m_bPlaying) return;

	g_fQuitTrackPlayThread=TRUE;

	while(m_bPlaying) Sleep(20);
}

void CSong::Write(const char* pstrFilename,int iCount,int iWriteStyle,int iFormat)
{
	INTERNAL_SYNCHRONIZE;

	try
	{
		if(iWriteStyle==CSong_WRITEWHOLE)
		{
			CPlayLimits plAll;

			// Open WAV file
			CWavFile wavfile;
			switch(iFormat)
			{
			case WRITEFORMAT_WAV:
				if(wavfile.Save(pstrFilename)!=CWavFile::OK) throw FALSE;
				break;
			case WRITEFORMAT_OGGHIGH:
				if(wavfile.SaveVorbis(pstrFilename,0.6f)!=CWavFile::OK) throw FALSE;
				break;
			case WRITEFORMAT_OGGLOW:
				if(wavfile.SaveVorbis(pstrFilename,0.2f)!=CWavFile::OK) throw FALSE;
				break;
			}
			
			// Repeat number of times required
			for(int iRepeat=0;iRepeat<iCount;iRepeat++)
			{
				// Repeat through all bars and all beats			
				SongLocation 
					slLocation=GetStartLocation(),
					slStop=GetEndLocation();
				while(!slLocation.Stop() && slLocation!=slStop)
				{
					// Allocate buffer
					CSample sample;
					sample.Create(GetBarLength(slLocation));

					// Calculate the beat
					Play(sample,0,slLocation,0,sample.Size(),plAll);

					// Write out the beat
					if(wavfile.Write(sample.Data(),sample.Size())!=CWavFile::OK) throw FALSE;

					slLocation=GetNextBar(slLocation,FALSE);
				}
			}
		}
		else if(iWriteStyle==CSong_WRITESOUNDS)
		{
			CString sBase=pstrFilename;
			if(sBase.Right(4)==".wav" || sBase.Right(4)==".ogg") 
				sBase=sBase.Mid(0,sBase.GetLength()-4);

			for(int iSound=0;iSound<GetNumSounds();iSound++)
			{
				// Don't bother if the sound doesn't do anything
				CSound& s=GetSound(iSound);
				if(!s.HasSample()) continue;

				// Create play limits
				CPlayLimits plSound;
				plSound.m_viLines.Add(iSound);

				// Open WAV file
				CWavFile wavfile;
				CString sActualName=sBase+"-"+s.GetName();
				switch(iFormat)
				{
				case WRITEFORMAT_WAV:
					if(wavfile.Save(sActualName+".wav")!=CWavFile::OK) throw FALSE;
					break;
				case WRITEFORMAT_OGGHIGH:
					if(wavfile.SaveVorbis(sActualName+".ogg",0.6f)!=CWavFile::OK) throw FALSE;
					break;
				case WRITEFORMAT_OGGLOW:
					if(wavfile.SaveVorbis(sActualName+".ogg",0.2f)!=CWavFile::OK) throw FALSE;
					break;
				}
				
				// Repeat number of times required
				for(int iRepeat=0;iRepeat<iCount;iRepeat++)
				{
					// Repeat through all bars and all beats			
					SongLocation 
						slLocation=GetStartLocation(),
						slStop=GetEndLocation();
					while(!slLocation.Stop() && slLocation!=slStop)
					{
						// Allocate buffer
						CSample sample;
						sample.Create(GetBarLength(slLocation));

						// Calculate the beat
						Play(sample,0,slLocation,0,sample.Size(),plSound);

						// Write out the beat
						if(wavfile.Write(sample.Data(),sample.Size())!=CWavFile::OK) throw FALSE;

						slLocation=GetNextBar(slLocation,FALSE);
					}
				}
			}
		}
		else if(iWriteStyle==CSong_WRITETRACKS)
		{
			CString sBase=pstrFilename;
			if(sBase.Right(4)==".wav") sBase=sBase.Mid(0,sBase.GetLength()-4);

			for(int iTrack=0;iTrack<GetNumTracks();iTrack++)
			{
				// Don't bother if the track doesn't do anything
				if(!GetTrack(iTrack).Used()) continue;

				// Create play limits
				CPlayLimits plTrack;
				plTrack.m_viTracks.Add(iTrack);

				// Open WAV file
				CWavFile wavfile;
				CString sActualName;
				sActualName.Format("%s-%i",sBase,iTrack+1);
				switch(iFormat)
				{
				case WRITEFORMAT_WAV:
					if(wavfile.Save(sActualName+".wav")!=CWavFile::OK) throw FALSE;
					break;
				case WRITEFORMAT_OGGHIGH:
					if(wavfile.SaveVorbis(sActualName+".ogg",0.6f)!=CWavFile::OK) throw FALSE;
					break;
				case WRITEFORMAT_OGGLOW:
					if(wavfile.SaveVorbis(sActualName+".ogg",0.2f)!=CWavFile::OK) throw FALSE;
					break;
				}
				
				// Repeat number of times required
				for(int iRepeat=0;iRepeat<iCount;iRepeat++)
				{
					// Repeat through all bars and all beats			
					SongLocation 
						slLocation=GetStartLocation(),
						slStop=GetEndLocation();
					while(!slLocation.Stop() && slLocation!=slStop)
					{
						// Allocate buffer
						CSample sample;
						sample.Create(GetBarLength(slLocation));

						// Calculate the beat
						Play(sample,0,slLocation,0,sample.Size(),plTrack);

						// Write out the beat
						if(wavfile.Write(sample.Data(),sample.Size())!=CWavFile::OK) throw FALSE;

						slLocation=GetNextBar(slLocation,FALSE);
					}
				}
			}
		}
	}
	catch(BOOL)
	{
		AfxMessageBox("leafDrums could not write the file. Check that you "
			"have sufficient disk space on the appropriate drive.",
			MB_ICONEXCLAMATION|MB_OK);
	}
}

int CSong::GetWriteSamples()
{
	INTERNAL_SYNCHRONIZE;

	int iLength=0;

	// Repeat through all bars and all beats			
	SongLocation 
		slLocation=GetStartLocation(),
		slStop=GetEndLocation();
	while(!slLocation.Stop() && slLocation!=slStop)
	{
		iLength+=GetBarLength(slLocation);

		slLocation=GetNextBar(slLocation,FALSE);
	}

	return iLength;
}

// Callback when playing stops
void CSong::PlayingStopped()
{ 
	m_bPlaying=FALSE; 
	SetPriorityClass(GetCurrentProcess(),NORMAL_PRIORITY_CLASS);
	if(CLeafDrumsView::sm_pCurrent)
		CLeafDrumsView::sm_pCurrent->PostMessage(WM_PLAYSTOPPED,0,0);
}

// Method CALLED FROM OTHER THREAD
void CSong::SetCurrentPlayData(int iBar,int iSixteenth,int iMaxLevel)
{ 
	if(m_iCurrentBar!=iBar || m_iCurrentSixteenth!=iSixteenth)
	{
		m_iCurrentBar=iBar; 
		m_iCurrentSixteenth=iSixteenth; 
		m_iCurrentMaxLevel=iMaxLevel; 
		CLeafDrumsView::sm_pCurrent->PostMessage(WM_PLAYPOSITIONCHANGED,0,0);
	}
}

int CSong::GetBarCount() 
{
	INTERNAL_SYNCHRONIZE;

	// Get greatest length of any track
	int iLength=0;
	for(int iTrack=0;iTrack<m_vtTracks.Size();iTrack++)
	{
		int iThisLength=GetTrack(iTrack).GetBarCount();
		if(iThisLength > iLength) iLength=iThisLength;
	}

	return iLength;
}

CSound& CSong::AddSound()
{
	INTERNAL_SYNCHRONIZE;

	CSound* pSound=new CSound();
	m_vsSounds.Add(pSound);
	return *pSound;
}

void CSong::RemoveSound(CSound& s)
{
	INTERNAL_SYNCHRONIZE;

	for(int i=0;i<m_vsSounds.Size();i++)
		if(&m_vsSounds[i] == &s)
	{
		m_vsSounds.RemoveAt(i);
		return;
	}

	// Why is this assert here?
	//ASSERT(FALSE);
}

int CSong::GetNumUsedSounds()
{
	INTERNAL_SYNCHRONIZE;

	for(int i=m_vsSounds.Size()-1;i>=0;i--)
	{
		if(m_vsSounds[i].HasSample()) return i+1;
	}
	return 0;
}


CPattern& CSong::AddPattern()
{
	INTERNAL_SYNCHRONIZE;

	CPattern* pPattern=new CPattern(this);
	m_vpPatterns.Add(pPattern);
	return *pPattern;
}

void CSong::RemovePattern(CPattern& s)
{
	INTERNAL_SYNCHRONIZE;

	for(int iTrack=0;iTrack<m_vtTracks.Size();iTrack++)
	{
		m_vtTracks[iTrack].RemovePattern(&s);
	}

	for(int i=0;i<m_vpPatterns.Size();i++)
		if(&m_vpPatterns[i] == &s)
	{
		m_vpPatterns.RemoveAt(i);
		return;
	}

	m_pbClipboard.RemovePattern(&s);
}

CTrack& CSong::AddTrack()
{
	INTERNAL_SYNCHRONIZE;

	CTrack* pTrack=new CTrack(this);
	m_vtTracks.Add(pTrack);
	return *pTrack;
}

void CSong::RemoveTrack(CTrack& s)
{
	INTERNAL_SYNCHRONIZE;

	for(int i=0;i<m_vtTracks.Size();i++)
		if(&m_vtTracks[i] == &s)
	{
		m_vtTracks.RemoveAt(i);
		return;
	}

	// There was an assert false here - WHY?!
	//ASSERT(FALSE);
}

int CSong::GetNumUsedTracks()
{
	INTERNAL_SYNCHRONIZE;

	for(int i=m_vtTracks.Size()-1;i>=0;i--)
	{
		if(m_vtTracks[i].Used()) return i+1;
	}
	return 0;
}

/*
How tempo change used to work!

void CSong::SetTempo(int iTempo) 
{ 
	INTERNAL_SYNCHRONIZE;

	m_iTempo=iTempo; 
	for(int i=0;i<m_vsSounds.Size();i++)
		m_vsSounds[i].SetTempo(iTempo);
}
*/

// Insert a bar at given location, shifting others along
void CSong::InsertBar(int iBar)
{
	INTERNAL_SYNCHRONIZE;

	for(int i=0;i<m_vtTracks.Size();i++)
		m_vtTracks[i].InsertBar(iBar);
	if(m_vcCoda.Size()>iBar)
	{
		m_vcCoda.InsertAt(iBar,new CCoda(this));
		CodaChanged();
	}
	if(m_vtcTime.Size()>iBar)
	{
		// Make sure there's still a timesig on bar 0
		m_vtcTime.InsertAt(iBar==0 ? 1 : iBar,new CTimeChange(this));
		TimeChanged();
	}
}

// Delete a bar at given location, shifting others along
void CSong::DeleteBar(int iBar)
{
	INTERNAL_SYNCHRONIZE;

	for(int i=0;i<m_vtTracks.Size();i++)
		m_vtTracks[i].DeleteBar(iBar);

	if(m_vcCoda.Size()>iBar)
	{
		m_vcCoda.RemoveAt(iBar);
		CodaChanged();
	}
	if(m_vtcTime.Size()>iBar)
	{
		// Make sure there's still a timesig on bar 0
		if(iBar>0 || m_vtcTime.Size()>1 && !m_vtcTime[1].IsEmpty())
			m_vtcTime.RemoveAt(iBar);
		else if(m_vtcTime.Size()>1)
			m_vtcTime.RemoveAt(1);
		
		TimeChanged();
	}
}

// Obtain coda information for a bar
CCoda& CSong::GetCoda(int iBar)
{
	INTERNAL_SYNCHRONIZE;

	// If we haven't made a coda for that...
	if(iBar>=m_vcCoda.Size())
	{
		// Set coda up to desired place
		for(int i=m_vcCoda.Size();i<=iBar;i++)
		{
			m_vcCoda.Set(i,new CCoda(this));

			CString sTrace;
			sTrace.Format("Set coda %i to %x\n",i,&m_vcCoda[i]);
			TRACE(sTrace);
		}
	}

	// Return the desired coda
	return m_vcCoda[iBar];
}

// Obtain time change information for a bar
CTimeChange& CSong::GetTimeChange(int iBar)
{
	INTERNAL_SYNCHRONIZE;

	// If we haven't made a timechange for that...
	if(iBar>=m_vtcTime.Size())
	{
		// Set timechange up to desired place
		for(int i=m_vtcTime.Size();i<=iBar;i++)
		{
			m_vtcTime.Set(i,new CTimeChange(this));
		}
	}

	// Return the desired coda
	return m_vtcTime[iBar];
}

// Returns index for a given song location or -1 if no decent match
int CSong::FindLocationIndex(const SongLocation& sl)
{
	INTERNAL_SYNCHRONIZE;

	// Build linear song if required
	if(m_vslLinear.Size()==0) CodaChanged();

	// If nothing in there, bail
	if(m_vslLinear.Size()==0)
		return -1;

	// If linearisation hasn't changed, we know the current index
	if(m_vslLinear.Size()>sl.iIndex && sl.iIndex>=0)
	{
		if(m_vslLinear[sl.iIndex] == sl)
		{
			return sl.iIndex;
		}
	}

	// Linearisation *has* changed, bugger. Search through list from start
	// to find a match for given bar with best possible repeatcount
	int iCurrentIndex=-1;
	for(int i=0;i<m_vslLinear.Size();i++)
	{
		if(m_vslLinear[i].iBar == sl.iBar)
		{
			if(m_vslLinear[i].iRepeat <= sl.iRepeat)
				iCurrentIndex=i;
			else
				break;
		}
	}

	return iCurrentIndex;
}

// Return the previous bar in the song 
SongLocation CSong::GetPreviousBar(SongLocation sl,BOOL bAllowLoop)
{
	INTERNAL_SYNCHRONIZE;

	// Find current location
	int iCurrentIndex=FindLocationIndex(sl);

	// That bar no longer exists, or it's at the start anyway - return stop
	if(iCurrentIndex==-1)
		return SongLocation(-1);

	// Return the previous bar unless we're past the start
	if(iCurrentIndex==0)
	{
		if(bAllowLoop && IsLoop())
			return m_vslLinear[m_vslLinear.Size()-1];
		else
			return SongLocation(-1);
	}
	else
		return m_vslLinear[iCurrentIndex-1];	
/*
	// Trivial implementation, duplicates existing functionality
	sl.iBar--;
	if(sl.iBar<0)
	{
		if(IsLoop()) sl.iBar=GetLength()-1;
		else sl.iBar=-1;
	}
	return sl;
	*/
}

// Returns next bar in song
SongLocation CSong::GetNextBar(SongLocation sl,BOOL bAllowLoop)
{
	INTERNAL_SYNCHRONIZE;

	// Find current location
	int iCurrentIndex=FindLocationIndex(sl);

	// That bar no longer exists, or it's at the end anyway - return stop
	if(iCurrentIndex==-1)
		return SongLocation(-1);

	// Return the following bar unless we're past the end
	if(iCurrentIndex==m_vslLinear.Size()-1)
	{
		if(bAllowLoop && IsLoop())
			return m_vslLinear[0];
		else
			return SongLocation(-1);
	}
	else
		return m_vslLinear[iCurrentIndex+1];	
}

// Returns the location at which to begin playback
SongLocation CSong::GetStartLocation()
{
	INTERNAL_SYNCHRONIZE;

	int i=FindLocationIndex(SongLocation(GetStartBar()));
	if(i==-1)
		return SongLocation(-1);
	else return m_vslLinear[i];
}

// Returns the location at which to begin playback
SongLocation CSong::GetEndLocation()
{
	INTERNAL_SYNCHRONIZE;

	int i=FindLocationIndex(SongLocation(GetStopBar()));
	if(i==-1)
		return SongLocation(-1);
	else return m_vslLinear[i];
}

// Returns the location at which to begin playback
SongLocation CSong::GetBeforeEndLocation()
{
	INTERNAL_SYNCHRONIZE;

	int i=FindLocationIndex(SongLocation(GetStopBar()-1));
	if(i==-1)
	{
		return SongLocation(-1);
	}		
	else return m_vslLinear[i];
}

// Called when a track length changes
void CSong::LengthChanged()
{
	CodaChanged();
}

void CSong::TimeChanged()
{
	CodaChanged();
}

void CSong::CodaChanged()
{
	INTERNAL_SYNCHRONIZE;

	// Clear linear song
	m_vslLinear.Clear();

	// Initialise marker counts
	int iMarkerCount[CCoda_MAXMARKERS];
	for(int iMarker=0;iMarker<CCoda_MAXMARKERS;iMarker++)
		iMarkerCount[iMarker]=0;

	// Initialise repeat counts
	int* piRepeat=new int[GetBarCount()];
	ZeroMemory(piRepeat,sizeof(int)*GetBarCount());

	// Track all tempos
	vec<int> viTempos;

	// Initialise start location
	SongLocation slCurrent;
	while(slCurrent.iBar < GetBarCount())
	{
		// Check if there's a time change at this point and if so, update time details
		CTimeChange& tc=GetTimeChange(slCurrent.iBar);
		if(!tc.IsEmpty())
		{
			slCurrent.iTempo=tc.GetTempo();
			slCurrent.iTimeSignature=tc.GetTimeSignature();

			if(viTempos.Find(slCurrent.iTempo)==-1)
				viTempos.Add(slCurrent.iTempo);
		}

		// Add this position to the list
		m_vslLinear.Add(slCurrent);

		// Update repeat count for bar
		piRepeat[slCurrent.iBar]++;

		// Update index
		slCurrent.iIndex++;

		// See if this bar (that we just added) has a repeat
		CCoda& c=GetCoda(slCurrent.iBar);
		int iJumpBar=-1;
		for(iMarker=CCoda_MAXMARKERS-1;iMarker>=0;iMarker--)
		{
			if(c.HasRepeat(iMarker) && iMarkerCount[iMarker]<c.GetRepeat(iMarker))
			{
				// Look for matching flag
				for(int iFlagBar=slCurrent.iBar;iFlagBar>=0;iFlagBar--)
					if(GetCoda(iFlagBar).HasFlag(iMarker)) break;

				// If there is no matching flag, ignore this repeat, try the next one
				if(iFlagBar==-1) continue;

				// Ok, found matching flag.  Increment count and jump to bar
				iMarkerCount[iMarker]++;
				iJumpBar=iFlagBar;
				break;
			}
		}

		// Go to next bar or jump
		int iAllowedFlags;
		if(iJumpBar!=-1)
		{
			slCurrent.iBar=iJumpBar;
			iAllowedFlags=iMarker+1;
		}
		else
		{
			slCurrent.iBar++;
			iAllowedFlags=0;
		}

		// Set repeat count
		slCurrent.iRepeat=piRepeat[slCurrent.iBar];

		// See if this bar has any flags that we are allowed to consider (the allowed
		// bit is so that it works if we have two flags, inner and outer, on the same
		// bar for different repeats)
		
		CCoda& c2=GetCoda(slCurrent.iBar);
		for(iMarker=iAllowedFlags;iMarker<CCoda_MAXMARKERS;iMarker++)
		{
			if(c2.HasFlag(iMarker))
				iMarkerCount[iMarker]=0;
		}


	}

	// Clear repeat buffer
	delete piRepeat;

	// Inform all sounds of available tempos so they can throw away anything unnecessary
	for(int i=0;i<m_vsSounds.Size();i++)
	{
		m_vsSounds[i].InformTempos(viTempos);
	}
}

int CSong::GetStartTempo() const
{
	return m_vtcTime[0].GetTempo();
}
int CSong::GetStartTimeSignature() const
{
	if(m_vtcTime.Size()==0) return 4; // Can happen while loading
	return m_vtcTime[0].GetTimeSignature();
}


int CSong::GetSoundNumber(int iOriginalSound)
{
	if(iOriginalSound>=m_viSoundMap.Size()) return iOriginalSound;

	return m_viSoundMap[iOriginalSound];
}

// Returns time signature of bar, or -1 if there are multiple
int CSong::GetUniqueTimeSignature(int iBar) 
{
	// Build linear song if required
	if(m_vslLinear.Size()==0) CodaChanged();

	int iFound=-1;
	for(int i=0;i<m_vslLinear.Size();i++)
	{
		if(m_vslLinear[i].iBar==iBar)
		{
			int iThis=m_vslLinear[i].iTimeSignature;
			if(iFound==-1) 
				iFound=iThis;
			else if(iFound!=iThis) return -1;
		}
	}

	return iFound;
}


// Coda methods
///////////////

void CCoda::Init()
{
	for(int i=0;i<CCoda_MAXMARKERS;i++)
	{
		m_bFlag[i]=FALSE;
		m_iRepeat[i]=0;
	}
	m_psOwner=NULL;
	iSpecial=42424242;
}

// Save to disk
void CCoda::Serialize(CArchive& a)
{
#if CCoda_MAXMARKERS!=3
#error Must change coda version number if the number of markers changes
#endif
	int iVersion=1;
	if(a.IsStoring())
	{
		a << iVersion;

		for(int iMarker=0;iMarker<CCoda_MAXMARKERS;iMarker++)
		{
			a << m_bFlag[iMarker];
			a << m_iRepeat[iMarker];
		}
	}
	else
	{
		a >> iVersion;
		if(iVersion!=1)
			throw "File format not supported. Make sure this is a valid file and you are "
			  "using the latest version of leafDrums.";

		for(int iMarker=0;iMarker<CCoda_MAXMARKERS;iMarker++)
		{
			a >> m_bFlag[iMarker];
			a >> m_iRepeat[iMarker];
		}
	}
}

// Check if empty
BOOL CCoda::IsEmpty() const
{
	// Return false if anything was set
	for(int i=0;i<CCoda_MAXMARKERS;i++)
	{
		if(HasFlag(i) || HasRepeat(i)) return FALSE;
	}
	return TRUE;
}

void CCoda::SetFlag(int iMarker,BOOL bExists)
{ 
	if(m_bFlag[iMarker]==bExists) return;

	m_bFlag[iMarker]=bExists; 
	m_psOwner->CodaChanged();
}

void CCoda::SetRepeat(int iMarker,int iCount)
{ 
	if(m_iRepeat[iMarker]==iCount) return;

	m_iRepeat[iMarker]=iCount; 
	m_psOwner->CodaChanged();
}

CCoda& CCoda::operator=(const CCoda& src)
{
	for(int i=0;i<CCoda_MAXMARKERS;i++)
	{
		m_bFlag[i]=src.m_bFlag[i];
		m_iRepeat[i]=src.m_iRepeat[i];
	}
	m_psOwner=src.m_psOwner;
	return *this;
}

CPatternBlock::CPatternBlock()
{
	m_iWidth=m_iHeight=0;
}

void CPatternBlock::Set(CSong& s,int iFromBar,int iFromTrack,int iToBar,int iToTrack)
{
	// Get basic data
	int iBarStart=min(iFromBar,iToBar),iTrackStart=min(iFromTrack,iToTrack);
	m_iWidth=max(iFromBar,iToBar)-min(iFromBar,iToBar)+1;
	m_iHeight=max(iFromTrack,iToTrack)-min(iFromTrack,iToTrack)+1;

	// Store patterns
	for(int iTrack=iTrackStart;iTrack<iTrackStart+m_iHeight;iTrack++)
	{
		CTrack& t=s.GetTrack(iTrack);
		for(int iBar=iBarStart;iBar<iBarStart+m_iWidth;iBar++)
		{
			m_vppData.Set(iTrack*m_iWidth+(iBar-iBarStart),(CPattern*)t.GetBar(iBar));
		}
	}
}

void CPatternBlock::RemovePattern(CPattern* pp)
{
	for(int i=0;i<m_iWidth*m_iHeight;i++)
	{
		if(m_vppData[i]==pp) m_vppData[i]=(CPattern*)NULL;
	}
}

void CPatternBlock::Paste(CSong& s,int iTargetBar,int iTargetTrack,BOOL bInsert)
{
	if(bInsert)
	{
		for(int i=0;i<m_iWidth;i++)
			s.InsertBar(iTargetBar);
	}

	for(int iTrack=0;iTrack<m_iHeight;iTrack++)
	{
		CTrack& t=s.GetTrack(iTrack+iTargetTrack);
		for(int iBar=0;iBar<m_iWidth;iBar++)
		{
			if(m_vppData.IsNonNull(iTrack*m_iWidth+iBar))
				t.SetBar(s,iBar+iTargetBar,m_vppData[iTrack*m_iWidth+iBar]);
			else
				t.SetBar(s,iBar+iTargetBar,NULL);
		}
	}
}

void CSong::SwapSounds(int iSound1,int iSound2)
{
	// Fill in sound map if needed
	int iMapMax=max(iSound1,iSound2);
	if(iMapMax>=m_viSoundMap.Size())
	{
		for(int i=m_viSoundMap.Size();i<=iMapMax;i++)
		{
			m_viSoundMap.Set(i,i);
		}
	}

	// Find the two sounds
	int 
		iPos1=m_viSoundMap.Find(iSound1),
		iPos2=m_viSoundMap.Find(iSound2);
	ASSERT(iPos1!=-1 && iPos2!=-1);

	m_viSoundMap[iPos1]=iSound2;
	m_viSoundMap[iPos2]=iSound1;
}



void CTimeChange::Init()
{
	m_psOwner=NULL;
	m_iTempo=0;
	m_iTimeSignature=0;
}

CTimeChange& CTimeChange::operator=(const CTimeChange& src)
{
	m_psOwner=src.m_psOwner;
	m_iTempo=src.m_iTempo;
	m_iTimeSignature=src.m_iTimeSignature;
	return *this;
}

// Save to disk
void CTimeChange::Serialize(CArchive& a)
{
	int iVersion=1;
	if(a.IsStoring())
	{
		a << iVersion;
		a << m_iTempo;
		a << m_iTimeSignature;
	}
	else
	{
		a >> iVersion;
		if(iVersion!=1)
			throw "File format not supported. Make sure this is a valid file and you are "
			  "using the latest version of leafDrums.";

		a >> m_iTempo;
		a >> m_iTimeSignature;
	}
}

void CTimeChange::SetTempo(int iTempo)
{ 
	if(m_iTempo==iTempo) return;

	m_iTempo=iTempo;
	m_psOwner->TimeChanged();
}

void CTimeChange::SetTimeSignature(int iTimeSignature)
{ 
	if(m_iTimeSignature==iTimeSignature) return;

	m_iTimeSignature=iTimeSignature;
	m_psOwner->TimeChanged();
}

void CTimeChange::SetEmpty()
{
	if(IsEmpty()) return;

	m_iTimeSignature=0;
	m_iTempo=0;
	m_psOwner->TimeChanged();
}

