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
#ifndef LINE_H
#define LINE_H

#include "vec.h"
#include "Beat.h"
#include "Song.h"
#include "Synchronization.h"

class CLine
{
private:
	// Placeholder containing the (usually four) beats of the bar
	CBeat* m_pbBeatContainer;

	// Maximum length (in samples) of line
	int m_iMaxLength;

	// Tempo for which the beat times were calculated
	int m_iValidTempo;

	// Sound length for which max length was calculated
	int m_iValidSoundLength;

	// Parent
	CPattern* m_ppParent;

	// Synch
	INTERNAL_DEFERREDSYNCH;	

public:
	// Construct empty, for loading
	CLine(CPattern* ppParent) 
	{ 
		m_pbBeatContainer=new CBeat(this); 
		m_iMaxLength=-1; m_iValidTempo=-1; m_iValidSoundLength=-1; 
		m_ppParent=ppParent;
		INTERNAL_DEFERREDSYNCHINIT(m_ppParent);
	}
	~CLine() { delete m_pbBeatContainer; }

	// Copy & constructor
	CLine(const CLine& src) { *this=src; }
	CLine& operator= (const CLine& src);
	void SetParent(CPattern* ppParent)
	{
		m_ppParent=ppParent;
		INTERNAL_DEFERREDSYNCHINIT(m_ppParent);
	}

	// Construct based on format of song
	CLine(CSong& s,CPattern* ppParent);

	// Return the beats
	CBeat& GetBeatContainer() { return *m_pbBeatContainer; }

	// Update the time signature from pattern
	void UpdateTimeSignature();

	// Get max length
	int GetMaxLength() const { ASSERT(m_iMaxLength!=-1); return m_iMaxLength; }

	// Plays a section of the bar, beginning at sample offset (which may be more 
	// than end of bar, or less than 0) and for length given, into the target sample, 
	// mixing onto anything that's already there, doing a level ramp.
	void Play(CMixBuffer& mbTarget,const int iTargetOffset,
		const int iOffset,const int iLength,
		CSong& sSong,CSound& sSound,
		// The following levels are stored in 16:16 fixed point format
		const int iLevelLStart,const int iLevelLIncrement, 
		const int iLevelRStart,const int iLevelRIncrement,
		const int iTempo,const int iTimeSignature);

	// Save/load
	void Serialize(CArchive& archive);

	// Update beat times (and max length) if necessary
	void CheckBeatTimes(CSong& sSong,CSound& sSound,const int iTempo);

	// Invalidate beat times (if you make something a triplet, etc.)
	// This doesn't need to be called if you just change tempo or sound
	void InvalidateBeatTimes() { INTERNAL_SYNCHRONIZE; m_iMaxLength=-1; m_iValidTempo=-1; m_iValidSoundLength=-1; }
};

#endif // LINE_H
