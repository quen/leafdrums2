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
#include "Line.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Construct based on format of song
CLine::CLine(CSong& s,CPattern* ppParent)
{
	m_iMaxLength=-1; m_iValidTempo=-1; m_iValidSoundLength=-1; 
	m_ppParent=ppParent;
	INTERNAL_DEFERREDSYNCHINIT(m_ppParent);

	m_pbBeatContainer=new CBeat(this); 

	for(int iBeat=0;iBeat<m_ppParent->GetTimeSignature();iBeat++)
	{
		// Create main beat
		CBeat* pbBeat=new CBeat(this);
		m_pbBeatContainer->AddSubBeat(pbBeat);

		// Give it two levels of children
		pbBeat->CreateChildren(2);
	}
}

// Plays a section of the bar, beginning at sample offset (which may be more 
// than end of bar, or less than 0) and for length given, into the target sample, 
// mixing onto anything that's already there, doing a level ramp.
void CLine::Play(CMixBuffer& mbTarget,const int iTargetOffset,
	const int iOffset,const int iLength,
	CSong& sSong,CSound& sSound,
	// The following levels are stored in 16:16 fixed point format
	const int iLevelLStart,const int iLevelLIncrement, 
	const int iLevelRStart,const int iLevelRIncrement,
	const int iTempo,const int iTimeSignature)
{
	INTERNAL_SYNCHRONIZE; 

	// Check that beat times are calculated
	CheckBeatTimes(sSong,sSound,iTempo);

	// Check it's within our area
	if(iOffset>m_iMaxLength || iOffset+iLength < 0) return;

	// Play
	m_pbBeatContainer->Play(mbTarget,iTargetOffset,
		iOffset,iLength,
		sSound,
		iLevelLStart,iLevelLIncrement, 
		iLevelRStart,iLevelRIncrement,
		iTempo,(iTimeSignature*60*44100)/iTempo  );
}

// Save/load
void CLine::Serialize(CArchive& a)
{
	INTERNAL_SYNCHRONIZE; 

	m_pbBeatContainer->Serialize(a);
}

// Update beat times (and max length) if necessary
void CLine::CheckBeatTimes(CSong& sSong,CSound& sSound,const int iTempo)
{
	INTERNAL_SYNCHRONIZE; 

	// Not necessary, stop
	if(iTempo == m_iValidTempo && 
		sSound.GetFinalLength(iTempo) == m_iValidSoundLength) return;

	// Build times
	m_pbBeatContainer->SetTimeOffsets(0,CSong::GetBarLength(iTempo,m_ppParent->GetTimeSignature()));

	// Get sound length
	m_iValidSoundLength=sSound.GetFinalLength(iTempo);

	// Work out max length
	m_iMaxLength=m_pbBeatContainer->GetLastTime() + m_iValidSoundLength;

	// Set valid
	m_iValidTempo=iTempo;
}


// Copy
CLine& CLine::operator= (const CLine& src)
{
	m_iMaxLength=src.m_iMaxLength;
	m_iValidTempo=src.m_iValidTempo;
	m_iValidSoundLength=src.m_iValidSoundLength;
	m_pbBeatContainer=new CBeat(*src.m_pbBeatContainer);
	m_pbBeatContainer->SetParent(this);
	m_ppParent=src.m_ppParent;
	INTERNAL_DEFERREDSYNCHINIT(m_ppParent);
	return *this;
}

// Update the time signature from pattern
void CLine::UpdateTimeSignature()
{
	INTERNAL_SYNCHRONIZE; 

	int iNew=m_ppParent->GetTimeSignature(),iCurrent=m_pbBeatContainer->GetNumSubBeats();

	for(;iCurrent<iNew;iCurrent++)
	{
		// Create main beat
		CBeat* pbBeat=new CBeat(this);
		m_pbBeatContainer->AddSubBeat(pbBeat);

		// Give it two levels of children
		pbBeat->CreateChildren(2);
	}
	for(;iCurrent>iNew;iCurrent--)
	{
		m_pbBeatContainer->RemoveSubBeat();
	}

	InvalidateBeatTimes();
}
