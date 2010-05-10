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
#ifndef PATTERN_H
#define PATTERN_H

#include "vec.h"
#include "MixBuffer.h"
#include "Synchronization.h"
#include "PlayLimits.h"

class CLine;
class CSong;
class CTrack;

class CPattern
{
private:
	vec<CLine> m_vlLines;
	CString m_strName;

	int m_iTimeSignature;

	// Owner
	CSong* m_psParent;

	INTERNAL_DEFERREDSYNCH;

public:
	CPattern(CSong* ptParent);

	// Copy & constructor
	CPattern(const CPattern& src) { *this=src; }
	CPattern& operator= (const CPattern& src);

	// Gets number of pattern in song (used in save)
	int GetIndex(CSong& sContext);

	// Get/set name (displayed to user)
	CString GetName() const { return m_strName; }
	void SetName(const char* pstrName) { INTERNAL_SYNCHRONIZE; m_strName=pstrName; }

	// Returns a line
	CLine& GetLine(int iLine) const { return m_vlLines[iLine]; }

	// Checks that sufficient lines exist as specified in song
	void CheckLines(CSong& sContext);

	// Works out max length of pattern (max length of any line in pattern)
	int GetMaxLength();

	// Returns time signature of pattern
	int GetTimeSignature() const { return m_iTimeSignature; }

	// Sets new time signature
	void SetTimeSignature(int iTimeSignature);

	// Plays a section of the bar, beginning at sample offset and 
	// for length given, into the target sample, mixing onto anything that's
	// already there, doing a level ramp, within the given track context
	void Play(CMixBuffer& mbTarget,const int iTargetOffset,
		const int iOffset,const int iLength,
		CSong& sContext,
		const int iLevelLStart,const int iLevelLEnd,
		const int iLevelRStart,const int iLevelREnd,
		CPlayLimits& pl,const int iTempo,const int iTimeSignature);

	// Save/load
	void Serialize(CArchive& archive);
};

#include "Line.h"
#include "Song.h"
#include "Track.h"

#endif // PATTERN_H
