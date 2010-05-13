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
#include "Beat.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Construct/destruct
CBeat::CBeat(CLine* plParent)
{
	m_plParent=plParent;
	m_bHit=FALSE;
	m_bAccent=FALSE;
	m_bTriplet=FALSE;
	m_iTimeOffset=-1;

	INTERNAL_DEFERREDSYNCHINIT(m_plParent);
}

// Set time offset of this beat, and calculate offsets of sub-beats
void CBeat::SetTimeOffsets(const int iThisOffset,const int iThisSize)
{
	INTERNAL_SYNCHRONIZE;

	m_iTimeOffset=iThisOffset;

	// Count sub-beats; each normal beat counts 3, triplet counts 2
	int i3Count=0;
	for(int i=0;i<m_vbSubBeats.Size();i++)
	{
		if(m_vbSubBeats[i].IsTriplet()) i3Count+=2;
		else i3Count+=3;
	}

	int iPosition=0;
	int iNextOffset=0;
	for(i=0;i<m_vbSubBeats.Size();i++)
	{
		// Copy offset from whatever was worked out last time
		int iOffset=iNextOffset;

		// Work out position after this beat
		iPosition+=(m_vbSubBeats[i].IsTriplet() ? 2 : 3);

		// Work out offset after it
		iNextOffset=(iThisSize*iPosition)/i3Count;

		// Now play this beat, using the distance between two offsets as length
		m_vbSubBeats[i].SetTimeOffsets(iOffset+iThisOffset,iNextOffset-iOffset);
	}
}

// Get lists of information about screen positions
void CBeat::GetScreenData(
	vec<CBeat*> &vpbBeats,vec<int>& viX,vec<int>& viImportance,
		// Output beats, X co-ords, and 'importance' (1==beat, 2==sub-beat, 3==fractional)
	int iX,int iWidth,int iDepth,
		// About this beat (position, width, depth in tree)
	int iImportance
		// Importance of this beat
	)
{
	INTERNAL_SYNCHRONIZE;

	if(IsLeaf())
	{
		// Add beat to list
		vpbBeats.Add(this);
		viX.Add(iX);
		if(iImportance==-1) iImportance=3;
		viImportance.Add(iImportance);
		return;
	}

	// Count sub-beats; each normal beat counts 3, triplet counts 2
	int i3Count=0;
	for(int i=0;i<m_vbSubBeats.Size();i++)
	{
		if(m_vbSubBeats[i].IsTriplet()) i3Count+=2;
		else i3Count+=3;
	}

	// Handle sub-beats
	int iPosition=0;
	int iNextOffset=0;
	for(i=0;i<m_vbSubBeats.Size();i++)
	{
		// Copy offset from whatever was worked out last time
		int iOffset=iNextOffset;

		// Work out position after this beat
		iPosition+=(m_vbSubBeats[i].IsTriplet() ? 2 : 3);

		// Work out offset after it
		iNextOffset=(iWidth*iPosition)/i3Count;

		// Now get data for this beat, using the distance between two offsets as length
		m_vbSubBeats[i].GetScreenData(
			vpbBeats,viX,viImportance,
			iOffset+iX,iNextOffset-iOffset,
			iDepth+1,
			(i==0 ? iImportance : iDepth+1)
			);
	}
}

// Get the last time of any sub-beat or this
int CBeat::GetLastTime()
{
	INTERNAL_SYNCHRONIZE;
	int iLast=m_iTimeOffset;

	for(int i=0;i<m_vbSubBeats.Size();i++)
	{
		int iTime=m_vbSubBeats[i].GetLastTime();
		if(iTime>iLast) iLast=iTime;
	}
	return iLast;
}

// Remove last sub-beat
void CBeat::RemoveSubBeat() 
{ 
	INTERNAL_SYNCHRONIZE;
	ASSERT(!IsLeaf());
	m_vbSubBeats.RemoveAt(m_vbSubBeats.Size()-1); 
}

// Save/load
void CBeat::Serialize(CArchive& a)
{
	INTERNAL_SYNCHRONIZE;

	int iVersion=1;

	if(a.IsStoring())
	{
		a << iVersion;
		a << m_bTriplet;
		a << m_vbSubBeats.Size();

		if(IsLeaf())
		{
			a << m_bHit;
			a << m_bAccent;
		}
		else
		{
			for(int i=0;i<m_vbSubBeats.Size();i++)
			  m_vbSubBeats[i].Serialize(a);
		}
	}
	else
	{
		a >> iVersion;
		if(iVersion!=1)
			throw "File format not supported. Make sure this is a valid file and you are "
			  "using the latest version of leafDrums.";

		a >> m_bTriplet;

		int iSize;
		a >> iSize;

		if(iSize==0)
		{
			a >> m_bHit;
			a >> m_bAccent;
		}
		else
		{
			for(int i=0;i<iSize;i++)
			{
				CBeat* pb=new CBeat(m_plParent);
				pb->Serialize(a);
				AddSubBeat(pb);
			}
		}
	}	
}

// Plays the beat or its sub-beats, taking into account the beat's own time offset
// (offsets as parameters are from start of function)
void CBeat::Play(CMixBuffer& mbTarget,const int iTargetOffset,
	const int iOffset,const int iLength,
	CSound& s,
	// The following levels are stored in 16:16 fixed point format
	const int iLevelLStart,const int iLevelLIncrement, 
	const int iLevelRStart,const int iLevelRIncrement,
	const int iTempo,const int iMaxOffset)
{
	INTERNAL_SYNCHRONIZE;

	if(IsLeaf())
	{
		int iTimeOffset=GetTimeOffset();
		if(m_bHit && iOffset+iLength > iTimeOffset && iTimeOffset<iMaxOffset)
		{
			s.Play(mbTarget,iTargetOffset,
				iOffset-GetTimeOffset(),iLength/*-GetTimeOffset()*/,
				iLevelLStart,iLevelLIncrement, 
				iLevelRStart,iLevelRIncrement,m_bAccent,iTempo);
		}
	}
	else
	{
		for(int i=0;i<m_vbSubBeats.Size();i++)
		{
			m_vbSubBeats[i].Play(mbTarget,iTargetOffset,
				iOffset,iLength,
				s,
				iLevelLStart,iLevelLIncrement, 
				iLevelRStart,iLevelRIncrement,iTempo,iMaxOffset);
		}
	}
}

// Make a triplet beginning from the given sub-beat
void CBeat::MakeTriplet(CBeat* pbFirstBeat)
{
	INTERNAL_SYNCHRONIZE;

	// Find beat
	for(int iBeat=0;iBeat<m_vbSubBeats.Size();iBeat++)
	{
	  if(&m_vbSubBeats[iBeat] == pbFirstBeat) break;
	}
	ASSERT(iBeat!=m_vbSubBeats.Size());
	
	// Mark this and the following beat as triplets
	m_vbSubBeats[iBeat].m_bTriplet=TRUE;
	m_vbSubBeats[iBeat+1].m_bTriplet=TRUE;

	// Create a new third triplet beat, inserted after the other two
	CBeat* pbNew=new CBeat(m_plParent);
	pbNew->m_bTriplet=TRUE;
	m_vbSubBeats.InsertAt(iBeat+2,pbNew);

	// Create structure of new beat as appropriate for this depth
	pbNew->CreateChildren(m_vbSubBeats[iBeat].GetHeight());

	// Note that this makes all the time calculations wrong, so they must
	// be marked invalid by whoever calls this
}

// Remove a triplet which begins from the given sub-beat
void CBeat::RemoveTriplet(CBeat* pbFirstBeat)
{
	INTERNAL_SYNCHRONIZE;

	// Find beat
	for(int iBeat=0;iBeat<m_vbSubBeats.Size();iBeat++)
	{
	  if(&m_vbSubBeats[iBeat] == pbFirstBeat) break;
	}
	ASSERT(iBeat!=m_vbSubBeats.Size());

	// Make sure it is a triplet and so are the following two
	ASSERT(m_vbSubBeats[iBeat].m_bTriplet);
	ASSERT(m_vbSubBeats[iBeat+1].m_bTriplet);
	ASSERT(m_vbSubBeats[iBeat+2].m_bTriplet);

	// Set them not triplet
	m_vbSubBeats[iBeat].m_bTriplet=FALSE;
	m_vbSubBeats[iBeat+1].m_bTriplet=FALSE;

	// And delete the spare beat
	m_vbSubBeats.RemoveAt(iBeat+2);

	// Note that this makes all the time calculations wrong, so they must
	// be marked invalid by whoever calls this
}

// Work out height of beat above leaves
int CBeat::GetHeight() 
{
	INTERNAL_SYNCHRONIZE;

	if(IsLeaf()) return 0;
	return GetSubBeat(0).GetHeight()+1;
}

// Create levels of children (in fours)
void CBeat::CreateChildren(int iLevels)
{
	INTERNAL_SYNCHRONIZE;

	if(iLevels==0) return;

	// Create four children
	for(int iChild=0;iChild<4;iChild++)
	{
		CBeat* pbChild=new CBeat(m_plParent);
		AddSubBeat(pbChild);
		pbChild->CreateChildren(iLevels-1);
	}
}

// Copy
CBeat& CBeat::operator= (const CBeat& src)
{
	m_bHit=src.m_bHit;
	m_bAccent=src.m_bAccent;
	m_bTriplet=src.m_bTriplet;
	m_vbSubBeats=src.m_vbSubBeats;
	m_iTimeOffset=src.m_iTimeOffset;
	m_plParent=src.m_plParent;
	INTERNAL_DEFERREDSYNCHINIT(m_plParent);
	return *this;
}
void CBeat::SetParent(CLine* plParent)
{
	m_plParent=plParent;
	INTERNAL_DEFERREDSYNCHINIT(m_plParent);
	for(int i=0;i<m_vbSubBeats.Size();i++)
		m_vbSubBeats[i].SetParent(plParent);
}


// Find parent of given beat (which obviously must be a child of this)
CBeat* CBeat::GetParent(CBeat* pbChild)
{
	INTERNAL_SYNCHRONIZE;

	// See if it's a child of this
	for(int iBeat=0;iBeat<m_vbSubBeats.Size();iBeat++)
	  if(&m_vbSubBeats[iBeat] == pbChild) return this;

	// See if it's a child of any sub-beats
 	for(iBeat=0;iBeat<m_vbSubBeats.Size();iBeat++)
	{
		CBeat* pParent=m_vbSubBeats[iBeat].GetParent(pbChild);
		if(pParent!=NULL) return pParent;
	}

	// Nope, couldn't find it
	return NULL;
}

CBeat* CBeat::GetThisParent(CBeat* pbBeatContainer,int iHeight)
{
	INTERNAL_SYNCHRONIZE;

	CBeat* pbReturn=this;

	// But go up levels if necessary
	for(int i=0;i<iHeight;i++)
	{
		pbReturn=pbBeatContainer->GetParent(pbReturn);
	}

	// Return
	return pbReturn;
}
