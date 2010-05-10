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
	m_iTempo=120;
	m_iLevel=128;
	m_iTimeSignature=4;
	m_bLoop=TRUE;
	m_bPlaying=FALSE;
	
	m_iStartBar=0;
	m_iStopBar=1;

	// Default to one pattern, track, and sound initially
	AddPattern();
	GetPattern(0).SetName("1");
	AddTrack();
	AddSound();
}

// Plays a section of the given bar, beginning at sample offset and 
// for length given, into the target sample, overwriting anything that's
// already there. Returns max playback level
int CSong::Play(CSample& sTarget,int iTargetOffset,
	SongLocation sl,int iOffset,int iLength) 
{
	// Create buffer
	CMixBuffer mb;
	mb.Create(iLength);

	// Mix all tracks into buffer
	for(int iTrack=0;iTrack<m_vtTracks.Size();iTrack++)
	{
		m_vtTracks[iTrack].Play(mb,0,sl,iOffset,iLength,
			*this,m_iLevel,m_iLevel);
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
	int iVersion=2;

	if(a.IsStoring())
	{
		a << iVersion;

		a << m_iTempo;
		a << m_iLevel;
		a << m_iTimeSignature;
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
	}
	else
	{
		a >> iVersion;
		if(iVersion!=1 && iVersion!=2)
			throw "File format not supported. Make sure this is a valid file and you are "
			  "using the latest version of leafDrums.";

		a >> m_iTempo;
		a >> m_iLevel;
		a >> m_iTimeSignature;
		a >> m_bLoop;
		a >> m_iStartBar;
		a >> m_iStopBar;

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
			CPattern* pPattern=new CPattern();
			pPattern->Serialize(a);
			m_vpPatterns.Add(pPattern);
		}
		a >> iCount;
		m_vtTracks.Clear();
		for(int iTrack=0;iTrack<iCount;iTrack++)
		{
			CTrack* pTrack=new CTrack();
			pTrack->Serialize(a,*this);
			m_vtTracks.Add(pTrack);
		}

		m_vcCoda.Clear();
		if(iVersion==2)
		{
			a >> iCount;
			for(int iCoda=0;iCoda<iCount;iCoda++)
			{
				CCoda* pCoda=new CCoda(this);
				pCoda->Serialize(a);
				m_vcCoda.Add(pCoda);
			}
		}

		m_vslLinear.Clear();
	}	
}

BOOL g_fQuitTrackPlayThread;

UINT TrackPlayThread(LPVOID lpvParam)
{
	CSong* pThis=(CSong*)lpvParam;

	CSingleLock sl(&(pThis->m_cs),FALSE);

	try
	{
		// Create sound device
		CDSoundPlayDevice dsd;

		// Data for track position
		int iSixteenth=0;
		SongLocation slLocation; 

		// Ought to find location properly
		slLocation.iBar=pThis->GetStartBar();

		// Data to hold beats
		CSample sample;

		// Hold previous play levels (circular buffer)
		const int PLAYLEVELBUFFER=64;
		int aiPlayLevel[PLAYLEVELBUFFER];
		for(int iPlayLevelPos=0;iPlayLevelPos<PLAYLEVELBUFFER;iPlayLevelPos++)
			aiPlayLevel[iPlayLevelPos]=0;
		iPlayLevelPos=0;

		// Size of play-ahead requirement (at least 100ms > 1/16 bar)
		DWORD dwPlayAhead=250;
		DWORD dwSixteenthms=(pThis->GetBarLength()*10)/(44100*16);
		if(dwSixteenthms > dwPlayAhead)
			dwPlayAhead=dwSixteenthms+100;

		while(TRUE)
		{
			sl.Lock();

			// Work out position of this block
			int iStartPos=(iSixteenth * pThis->GetBarLength())/16;
			int iNextPos=((iSixteenth+1) * pThis->GetBarLength())/16;
			int iBlockSize=iNextPos-iStartPos;

			// Make sure bar position is ok
			if(slLocation.iBar>=pThis->GetLength()) 
			{
				if(pThis->m_bLoop)
					slLocation.iBar=pThis->GetStartBar();
				else
				{
					sl.Unlock();
					dsd.StopAfterBuffers();
					break;
				}
			}
			else
			{
				if(slLocation.iBar>=pThis->GetStopBar())
					slLocation.iBar=pThis->GetStartBar();
			}
			// If it's still too long, then stop and start must be same? bail
			if(slLocation.iBar>=pThis->GetLength()) 
			{
				sl.Unlock();
				dsd.StopAfterBuffers();
				break;
			}

			// Create a block
			CBlock* pBlock=CBlock::Create(TRUE,iBlockSize);

			// Make the sample use this
			sample.Set(pBlock->Data(),iBlockSize);

			// Play a beat to the sample
			int iLevel=pThis->Play(sample,0,slLocation,iStartPos,iBlockSize);

			sl.Unlock();

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
					int iCurrentSixteenth=iSixteenth-dwWait/dwSixteenthTime;
					int iCurrentBar=slLocation.iBar;
					while(iCurrentSixteenth<0)
					{
						iCurrentSixteenth+=16;
						iCurrentBar--;
						if(iCurrentBar<pThis->GetStartBar())
						{
							iCurrentBar=pThis->GetStopBar()-1;
						}
						if(iCurrentBar>=pThis->GetLength())
							iCurrentBar=pThis->GetLength()-1;
					}
					int iCurrentPlayLevelPos=(iPlayLevelPos-1)-dwWait/dwSixteenthTime;
					while(iCurrentPlayLevelPos<0)
						iCurrentPlayLevelPos+=PLAYLEVELBUFFER;

					pThis->SetCurrentPlayData(iCurrentBar,iCurrentSixteenth,
						aiPlayLevel[iCurrentPlayLevelPos]);

					if(dwWait < dwPlayAhead) break;

					DWORD dwSleep=dwWait-dwPlayAhead;
					if(dwSleep>dwSixteenthTime/4)
						dwSleep=dwSixteenthTime/4;

					if(g_fQuitTrackPlayThread) break;
					Sleep(dwSleep);
				}
			}

			// Go to next sixteenth
			iSixteenth++;
			if(iSixteenth>15)
			{
				iSixteenth=0; slLocation.iBar++;
			}

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

void CSong::Write(const char* pstrFilename,int iCount)
{
	try
	{
		// Open WAV file
		CWavFile wavfile;
		if(wavfile.Save(pstrFilename)!=CWavFile::OK) throw FALSE;
		
		// Allocate buffer
		CSample sample;
		sample.Create(GetBarLength());

		// Repeat number of times required
		for(int iRepeat=0;iRepeat<iCount;iRepeat++)
		{
			// Repeat through all bars and all beats			
			SongLocation 
				slLocation=GetStartLocation(),
				slStop=GetStopLocation();
			while(!slLocation.Stop() && slLocation!=slStop)
			{
				// Calculate the beat
				Play(sample,0,slLocation,0,GetBarLength());

				// Write out the beat
				if(wavfile.Write(sample.Data(),sample.Size())!=CWavFile::OK) throw FALSE;

				slLocation=GetNextBar(slLocation,FALSE);
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

int CSong::GetBarCount() const
{
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
	CSound* pSound=new CSound();
	m_vsSounds.Add(pSound);
	return *pSound;
}

void CSong::RemoveSound(CSound& s)
{
	for(int i=0;i<m_vsSounds.Size();i++)
		if(&m_vsSounds[i] == &s)
	{
		m_vsSounds.RemoveAt(i);
		return;
	}

	ASSERT(FALSE);
}

CPattern& CSong::AddPattern()
{
	CPattern* pPattern=new CPattern();
	m_vpPatterns.Add(pPattern);
	return *pPattern;
}

void CSong::RemovePattern(CPattern& s)
{
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

	ASSERT(FALSE);
}

CTrack& CSong::AddTrack()
{
	CTrack* pTrack=new CTrack();
	m_vtTracks.Add(pTrack);
	return *pTrack;
}

void CSong::RemoveTrack(CTrack& s)
{
	for(int i=0;i<m_vtTracks.Size();i++)
		if(&m_vtTracks[i] == &s)
	{
		m_vtTracks.RemoveAt(i);
		return;
	}

	ASSERT(FALSE);
}

void CSong::SetTempo(int iTempo) 
{ 
	m_iTempo=iTempo; 
	for(int i=0;i<m_vsSounds.Size();i++)
		m_vsSounds[i].SetTempo(iTempo);
}

// Insert a bar at given location, shifting others along
void CSong::InsertBar(int iBar)
{
	for(int i=0;i<m_vtTracks.Size();i++)
		m_vtTracks[i].InsertBar(iBar);
}

// Delete a bar at given location, shifting others along
void CSong::DeleteBar(int iBar)
{
	for(int i=0;i<m_vtTracks.Size();i++)
		m_vtTracks[i].DeleteBar(iBar);
}

// Obtain coda information for a bar
CCoda& CSong::GetCoda(int iBar)
{
	// If we haven't made a coda for that...
	if(iBar>m_vcCoda.Size())
	{
		// Set coda up to desired place
		CCoda c(this);
		for(int i=m_vcCoda.Size();i<=iBar;i++)
			m_vcCoda.Set(i,c);
	}

	// Return the desired coda
	return m_vcCoda[iBar];
}

// Returns index for a given song location or -1 if no decent match
int CSong::FindLocationIndex(const SongLocation& sl)
{
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
SongLocation CSong::GetPreviousBar(SongLocation sl)
{
	// Find current location
	int iCurrentIndex=FindLocationIndex(sl);

	// That bar no longer exists, or it's at the start anyway - return stop
	if(iCurrentIndex==-1 || (iCurrentIndex==0 && !IsLoop()))
		return SongLocation(-1);

	// Return the previous bar unless we're past the start
	if(iCurrentIndex==0)
		return m_vslLinear[m_vslLinear.Size()-1];
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
SongLocation CSong::GetNextBar(SongLocation sl)
{
	// Find current location
	int iCurrentIndex=FindLocationIndex(sl);

	// That bar no longer exists, or it's at the end anyway - return stop
	if(iCurrentIndex==-1 || (iCurrentIndex==m_vslLinear.Size()-1 && !IsLoop()))
		return SongLocation(-1);

	// Return the following bar unless we're past the end
	if(iCurrentIndex==m_vslLinear.Size()-1)
		return m_vslLinear[0];
	else
		return m_vslLinear[iCurrentIndex+1];	
}

// Returns the location at which to begin playback
SongLocation CSong::GetStartLocation()
{
	int i=FindLocationIndex(SongLocation(GetStartBar()));
	if(i==-1)
		return SongLocation(-1);
	else return m_vslLinear[i];
}

// Returns the location at which to begin playback
SongLocation CSong::GetEndLocation()
{
	int i=FindLocationIndex(SongLocation(GetEndBar()));
	if(i==-1)
		return SongLocation(-1);
	else return m_vslLinear[i];
}

void CSong::CodaChanged()
{
	// Clear linear song
	m_vslLinear.Clear();

	// Initialise marker counts
	int iMarkerCount[CCoda_MAXMARKERS];
	for(int iMarker=0;iMarker<CCoda_MAXMARKERS;iMarker++)
		iMarkerCount[iMarker]=0;

	// Initialise repeat counts
	int* piRepeat=new int[GetLength()];
	ZeroMemory(piRepeat,sizeof(int)*GetLength());

	// Initialise start location
	SongLocation slCurrent;
	while(slCurrent.iBar < GetLength())
	{
		// Add this position to the list
		m_vslLinear.Add(slCurrent);

		// Update repeat count for bar
		piRepeat[slCurrent.iBar]++;

		// Update index
		slCurrent.iIndex++;

		// See if this bar (that we just added) has a repeat
		CCoda& c=GetCoda(slCurrent.iBar);
		int iJumpBar=-1;
		for(iMarker=0;iMarker<CCoda_MAXMARKERS;iMarker++)
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
			iAllowedFlags=iMarker;
		}
		else
		{
			slCurrent.iBar++;
			iAllowedFlags=CCoda_MAXMARKERS;
		}

		// Set repeat count
		slCurrent.iRepeat=piRepeat[slCurrent.iBar];

		// See if this bar has any flags that we are allowed to consider (the allowed
		// bit is so that it works if we have two flags, inner and outer, on the same
		// bar for different repeats)
		c=GetCoda(slCurrent.iBar);
		for(iMarker=0;iMarker<iAllowedFlags;iMarker++)
		{
			if(c.HasFlag(iMarker))
				iMarkerCount[iMarker]=0;
		}


	}

	// Clear repeat buffer
	delete piRepeat;
}


// Coda methods
///////////////

CCoda::CCoda()
{
	for(int i=0;i<CCoda_MAXMARKERS;i++)
	{
		m_bFlag[i]=FALSE;
		m_iRepeat[i]=0;
	}
	m_psOwner=NULL;
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
			a << m_bFlag[iMarker];
			a << m_iRepeat[iMarker];
		}
	}
}

// Check if empty
BOOL CCoda::IsEmpty()
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
