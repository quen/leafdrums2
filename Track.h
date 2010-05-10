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
#ifndef TRACK_H
#define TRACK_H

#include "vec.h"
#include "Synchronization.h"

#include "VariableParameter.h"
#include "MixBuffer.h"
#include "PlayLimits.h"

class CPattern;
class CSong;
class SongLocation;

class CTrack : public CObject
{
private:
	// Which pattern (or null) is used in a given bar
	vec<CPattern*> m_vppBars;

	// Level (0-256)
	int m_iLevel;

	// Pan (0-256)
	int m_iPan;

	// Variable parameter for level 
	CVariableParameter m_vpLevel;

	// Owner
	CSong* m_psOwner;

	INTERNAL_DEFERREDSYNCH;

public:
	CTrack(CSong* psOwner);

	// Returns true if any bars at all have patterns in
	BOOL Used() const;

	// Gets the length of the track
	int GetBarCount() const;

	// Gets basic data
	int GetLevel() const { return m_iLevel; }
	int GetPan() const { return m_iPan; }

	// Get variable parameter access
	CVariableParameter& GetLevelEnvelope() { return m_vpLevel; }

	// Sets basic data
	void SetLevel(int iLevel) { INTERNAL_SYNCHRONIZE; m_iLevel=iLevel; }
	void SetPan(int iPan) { INTERNAL_SYNCHRONIZE; m_iPan=iPan; }

	// Returns the desired bar pattern
	CPattern* GetBar(int iBar) const 
	{ 
		if(iBar < m_vppBars.Size())
			return m_vppBars[iBar]; 
		else 
			return NULL;
	}

	// Sets a bar to a given pattern
	void SetBar(CSong& sContext,int iBar,CPattern* pPattern);

	// Plays a section of the given bar, beginning at sample offset and 
	// for length given, into the target sample, mixing onto anything that's
	// already there
	void Play(CMixBuffer& mbTarget,int iTargetOffset,
		SongLocation sl,int iOffset,int iLength,
		CSong& sContext,
		int iLevelL,int iLevelR,
		CPlayLimits& pl);

	// Save/load
	void Serialize(CArchive& archive,CSong& sContext);

	// Remove a pattern (used when pattern was deleted)
	void RemovePattern(CPattern* p);

	// Insert a bar at given location, shifting others along
	void InsertBar(int iBar);

	// Delete a bar at given location, shifting others along
	void DeleteBar(int iBar);
};

#include "Pattern.h"
#include "Song.h"

#endif // TRACK_H
