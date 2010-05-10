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
#ifndef BEAT_H
#define BEAT_H

#include "vec.h"
#include "MixBuffer.h"
#include "Synchronization.h"

class CSound;
class CLine;

class CBeat : public CObject
{
private:
	// Is there a hit here?
	BOOL m_bHit,m_bAccent;

	// Is this part of a triplet?
	BOOL m_bTriplet;

	// Any sub-beats
	vec<CBeat> m_vbSubBeats;

	// Time offset (in samples) from parent beat
	int m_iTimeOffset;

	// Line that owns beat
	CLine* m_plParent;

	// Synch
	INTERNAL_DEFERREDSYNCH;	

public:
	// Construct
	CBeat(CLine* plParent);

	// Copy & constructor
	CBeat(const CBeat& src) { *this=src; }
	CBeat& operator= (const CBeat& src);
	void SetParent(CLine* plParent);

	// Create levels of children (in fours)
	void CreateChildren(int iLevels);

	// Set time offset of this beat, and calculate offsets of sub-beats
	void SetTimeOffsets(const int iThisOffset,const int iThisSize);

	// Get lists of information about screen positions
	void GetScreenData(
		vec<CBeat*> &vpbBeats,vec<int>& viX,vec<int>& viImportance,
			// Output beats, X co-ords, and 'importance' (1==beat, 2==sub-beat, 3==fractional)
		int iX,int iWidth,int iDepth/*=0*/,
			// About this beat (position, width, depth in tree)
		int iImportance/*=999*/
			// Importance of this beat
		);

	// Get time offset
	int GetTimeOffset() { INTERNAL_SYNCHRONIZE; ASSERT(m_iTimeOffset!=-1); return m_iTimeOffset; }

	// True if a 'leaf' (no sub-beats)
	BOOL IsLeaf() { INTERNAL_SYNCHRONIZE; return m_vbSubBeats.Size()==0; }

	// Work out height of beat above leaves
	int GetHeight();

	// Get/set information on hit
	void SetHit(const BOOL bHit) { INTERNAL_SYNCHRONIZE; ASSERT(IsLeaf()); m_bHit=bHit;  }
	BOOL IsHit() { INTERNAL_SYNCHRONIZE; ASSERT(IsLeaf()); return m_bHit; }
	void SetAccent(const BOOL bAccent) { INTERNAL_SYNCHRONIZE; ASSERT(IsLeaf()); m_bAccent=bAccent; }
	BOOL IsAccent() { INTERNAL_SYNCHRONIZE; ASSERT(IsLeaf()); return m_bAccent; }
	BOOL IsTriplet() { INTERNAL_SYNCHRONIZE; return m_bTriplet; }

	// Get information on sub-beats
	int GetNumSubBeats() { INTERNAL_SYNCHRONIZE; return m_vbSubBeats.Size(); }
	CBeat& GetSubBeat(const int iIndex) { INTERNAL_SYNCHRONIZE; return m_vbSubBeats[iIndex]; }

	// Add a sub-beat
	void AddSubBeat(CBeat* pb) { INTERNAL_SYNCHRONIZE; m_vbSubBeats.Add(pb); }

	// Remove last sub-beat
	void RemoveSubBeat();

	// Make a triplet beginning from the given sub-beat
	void MakeTriplet(CBeat* pbFirstBeat);

	// Remove a triplet which begins from the given sub-beat
	void RemoveTriplet(CBeat* pbFirstBeat);

	// Get the last time of any sub-beat or this
	int GetLastTime();

	// Find parent of given beat (which obviously must be a child of this)
	CBeat* GetParent(CBeat* pbChild);

	// Find parent of this beat at desired height above this
	CBeat* GetThisParent(CBeat* pbBeatContainer,int iHeight);

	// Plays the beat or its sub-beats, taking into account the beat's own time offset
	// (offsets as parameters are from start of function)
	void Play(CMixBuffer& mbTarget,const int iTargetOffset,
		const int iOffset,const int iLength,
		CSound& s,
		// The following levels are stored in 16:16 fixed point format
		const int iLevelLStart,const int iLevelLIncrement, 
		const int iLevelRStart,const int iLevelRIncrement,
		const int iTempo,const int iMaxSubBeats=-1);

	// Save/load
	void Serialize(CArchive& archive);
};

#include "Sound.h"
#include "Line.h"

#endif // BEAT_H
