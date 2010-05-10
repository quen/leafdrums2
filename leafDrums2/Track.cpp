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
#include "Track.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CTrack::CTrack(CSong* psOwner) : m_vpLevel(256)
{
	m_iLevel=128;
	m_iPan=128;
	m_psOwner=psOwner;
	INTERNAL_DEFERREDSYNCHINIT(psOwner);
}

// Plays a section of the given bar, beginning at sample offset and 
// for length given, into the target sample, mixing onto anything that's
// already there
void CTrack::Play(CMixBuffer& mbTarget,int iTargetOffset,
	SongLocation sl,int iOffset,int iLength,
	CSong& sContext,
	int iLevelL,int iLevelR,
	CPlayLimits& pl) 
{
	INTERNAL_SYNCHRONIZE; 

	int iBar=sl.iBar;

	// Get base sound levels
	int 
		iBaseLeft=((256-m_iPan)*m_iLevel*iLevelL)>>15,
		iBaseRight=((m_iPan)*m_iLevel*iLevelR)>>15;

	// Get first conversion level from variables
	int iStartFraction=(iOffset * 256)/sContext.GetBarLength(sl);
	int iFinalFraction=((iOffset+iLength)*256)/sContext.GetBarLength(sl);

	int iStartLevel=m_vpLevel.GetValueAt(iBar,iStartFraction);
	int iEndLevel;
	int iRemainingLength=iLength;

	while(TRUE)
	{
		// Find next point
		int iNextPoint=m_vpLevel.GetNextPoint(iBar,iStartFraction);
		int iEndFraction;

		// If there was a point...
		if(iNextPoint!=-1)
		{
			// Check to see if it's within the length
			if(m_vpLevel.GetPointBar(iNextPoint)>iBar ||
				m_vpLevel.GetPointFraction(iNextPoint) > iFinalFraction)
				// Nope.
				iNextPoint=-1;
		}

		if(iNextPoint==-1)
		{
			// No more points, do all the rest
			iEndFraction=iFinalFraction;
			iLength=iRemainingLength;
		}
		else
		{
			// Get position of next point
			iEndFraction=m_vpLevel.GetPointFraction(iNextPoint);
			iLength=(iEndFraction*sContext.GetBarLength(sl))/256 - iOffset;
		}

		if(iLength>0)
		{
			// Get actual end level of this run
			iEndLevel=m_vpLevel.GetValueAt(iBar,iEndFraction);

			// Now render actual sound (check up to 5 bars back) for this run
			int iActualOffset=iOffset;
			SongLocation slActual=sl;
			for(int iTraceback=0;iTraceback<10;iTraceback++)
			{
				if(GetBar(slActual.iBar))
					GetBar(slActual.iBar)->Play(mbTarget,iTargetOffset,
						iActualOffset,iLength,
						sContext,
						(iStartLevel*iBaseLeft)>>8,(iEndLevel*iBaseLeft)>>8,
						(iStartLevel*iBaseRight)>>8,(iEndLevel*iBaseRight)>>8,
						pl,sl.iTempo,sl.iTimeSignature
						);

				// Go back a bar
				slActual=sContext.GetPreviousBar(slActual);
				if(slActual.iBar==-1) break;

				iActualOffset+=sContext.GetBarLength(sl);
			}
		}

		// If there are no more points we're done
		if(iNextPoint==-1) break;

		// Move offset along
		iOffset+=iLength;
		iTargetOffset+=iLength;
		iRemainingLength-=iLength;
		iStartFraction=iEndFraction;
	}
}

// Save/load
void CTrack::Serialize(CArchive& a,CSong& sContext)
{
	INTERNAL_SYNCHRONIZE; 

	int iVersion=1;

	if(a.IsStoring())
	{
		a << iVersion;

		a << m_iLevel;
		a << m_iPan;

		m_vpLevel.Serialize(a);

		a << m_vppBars.Size();
		for(int i=0;i<m_vppBars.Size();i++)
		{
			if(m_vppBars[i]==NULL) 
				a << -1;
			else
				a << m_vppBars[i]->GetIndex(sContext);
		}
	}
	else
	{
		a >> iVersion;
		if(iVersion!=1)
			throw "File format not supported. Make sure this is a valid file and you are "
			  "using the latest version of leafDrums.";

		a >> m_iLevel;
		a >> m_iPan;

		m_vpLevel.Serialize(a);

		int iSize;
		a >> iSize;

		for(int i=0;i<iSize;i++)
		{
			int iIndex;
			a >> iIndex;
			if(iIndex==-1)
				m_vppBars.Add((CPattern*)NULL);
			else
				m_vppBars.Add(&(sContext.GetPattern(iIndex)));
		}
	}	
}

// Gets the length of the track
int CTrack::GetBarCount() const
{
	// Find last position that contains something
	for(int iLast=m_vppBars.Size()-1;iLast>0 && m_vppBars[iLast]==NULL;iLast--) ;
	return iLast+1;
}

// Returns true if any bars at all have patterns in
BOOL CTrack::Used() const
{
	for(int iLast=0;iLast<m_vppBars.Size();iLast++)
		if(m_vppBars[iLast]!=NULL) return TRUE;
	return FALSE;
}


// Remove a pattern (used when pattern was deleted)
void CTrack::RemovePattern(CPattern* p)
{
	INTERNAL_SYNCHRONIZE; 

	for(int i=0;i<m_vppBars.Size();i++)
	{
		if(m_vppBars[i]==p) m_vppBars[i]=NULL;
	}
}

// Insert a bar at given location, shifting others along
void CTrack::InsertBar(int iBar)
{
	INTERNAL_SYNCHRONIZE; 

	if(m_vppBars.Size() > iBar)
		m_vppBars.InsertAt(iBar,(CPattern*)NULL);
}

// Delete a bar at given location, shifting others along
void CTrack::DeleteBar(int iBar)
{
	INTERNAL_SYNCHRONIZE; 

	if(m_vppBars.Size() > iBar)
		m_vppBars.RemoveAt(iBar);
}

// Sets a bar to a given pattern
void CTrack::SetBar(CSong& sContext,int iBar,CPattern* pPattern)
{
	INTERNAL_SYNCHRONIZE; 

	int iOldLength=GetBarCount();
	for(int iClear=m_vppBars.Size();iClear<iBar;iClear++)
		m_vppBars.Set(iClear,(CPattern*)NULL);
	m_vppBars.Set(iBar,pPattern);
	if(GetBarCount()!=iOldLength)
		sContext.LengthChanged();
}
