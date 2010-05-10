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
#include "Pattern.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CPattern::CPattern(CSong* psParent)
{
	m_psParent=psParent;
	m_iTimeSignature=psParent->GetStartTimeSignature();
	INTERNAL_DEFERREDSYNCHINIT(m_psParent);
}

// Plays a section of the bar, beginning at sample offset and 
// for length given, into the target sample, mixing onto anything that's
// already there, doing a level ramp, within the given track context
void CPattern::Play(CMixBuffer& mbTarget,const int iTargetOffset,
	const int iOffset,const int iLength,
	CSong& sContext,
	const int iLevelLStart,const int iLevelLEnd,
	const int iLevelRStart,const int iLevelREnd,
	CPlayLimits& pl,const int iTempo,const int iTimeSignature)
{
	INTERNAL_SYNCHRONIZE; 

	// Work out level ramp
	int 
		iLevelLIncrement=((iLevelLEnd-iLevelLStart)<<16)/iLength,
		iLevelRIncrement=((iLevelREnd-iLevelRStart)<<16)/iLength;

	// Play all lines
	for(int iLine=0;iLine<m_vlLines.Size();iLine++)
	{
		if(pl.PlayLine(iLine))
			m_vlLines[iLine].Play(mbTarget,iTargetOffset,
				iOffset,iLength,
				sContext,sContext.GetSound(iLine),
				iLevelLStart<<16,iLevelLIncrement,
				iLevelRStart<<16,iLevelRIncrement,
				iTempo,iTimeSignature);
	}
}

// Save/load
void CPattern::Serialize(CArchive& a)
{
	INTERNAL_SYNCHRONIZE; 

	int iVersion=2;

	if(a.IsStoring())
	{
		a << iVersion;
		a << m_strName;

		a << m_vlLines.Size();		
		for(int i=0;i<m_vlLines.Size();i++)
		  m_vlLines[i].Serialize(a);

		a << m_iTimeSignature;
	}
	else
	{
		a >> iVersion;
		if(iVersion<1 && iVersion>2)
			throw "File format not supported. Make sure this is a valid file and you are "
			  "using the latest version of leafDrums.";

		a >> m_strName;

		int iSize;
		a >> iSize;

		for(int i=0;i<iSize;i++)
		{
			CLine* pl=new CLine(this);
			pl->Serialize(a);
			m_vlLines.Add(pl);
		}

		if(iVersion>=2)
		{
			a >> m_iTimeSignature;
		}
		else
		{
			// Use parent timesig
			m_iTimeSignature=m_psParent->GetStartTimeSignature();
		}

	}	
}

// Checks that sufficient lines exist as specified in song
void CPattern::CheckLines(CSong& sContext)
{
	INTERNAL_SYNCHRONIZE; 

	while(m_vlLines.Size() < sContext.GetNumSounds())
		m_vlLines.Add(new CLine(sContext,this));
}


// Works out max length of pattern (max length of any line in pattern)
int CPattern::GetMaxLength()
{
	INTERNAL_SYNCHRONIZE; 

	int iMaxLength=0;
	for(int iLine=0;iLine<m_vlLines.Size();iLine++)
	{
		if(m_vlLines[iLine].GetMaxLength() > iMaxLength)
			iMaxLength=m_vlLines[iLine].GetMaxLength();
	}
	return iMaxLength;
}

// Gets number of pattern (used in save)
int CPattern::GetIndex(CSong& sContext)
{
	INTERNAL_SYNCHRONIZE; 

	// Loop through all patterns
	for(int i=0;i<sContext.GetNumPatterns();i++)
	{
		if(&(sContext.GetPattern(i)) == this) return i;
	}

	ASSERT(FALSE);
	return -1;
}

CPattern& CPattern::operator= (const CPattern& src)
{
	m_strName=src.m_strName;
	m_psParent=src.m_psParent;
	m_vlLines=src.m_vlLines;
	m_iTimeSignature=src.m_iTimeSignature;
	for(int i=0;i<m_vlLines.Size();i++)
		m_vlLines[i].SetParent(this);
	return *this;
}

// Sets new time signature
void CPattern::SetTimeSignature(int iTimeSignature)
{
	INTERNAL_SYNCHRONIZE;

	if(m_iTimeSignature==iTimeSignature) return;

	m_iTimeSignature=iTimeSignature;
	for(int i=0;i<m_vlLines.Size();i++)
		m_vlLines[i].UpdateTimeSignature();
}

