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
#include "VariableParameter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Returns value of parameter at the given position (fraction is a
// fractional bar position, 0-255)
int CVariableParameter::GetValueAt(int iBar,int iFraction) const 
{
	int 
		iBefore=GetPreviousPoint(iBar,iFraction),
		iAfter=GetNextPoint(iBar,iFraction);

	// If before any points, return default/first value
	if(iBefore==-1)
	{
		if(iAfter==-1)
			return m_iDefaultValue;
		else
			return GetPointValue(iAfter);
	}

	// If after all points, return last value
	if(iAfter==-1)
		return GetPointValue(iBefore);

	// Calculate relative position
	int iSize = 
		(GetPointBar(iAfter) * 256 + GetPointFraction(iAfter)) -
		(GetPointBar(iBefore) * 256 + GetPointFraction(iBefore));
	int iOffset =
		(iBar * 256 + iFraction) -
		(GetPointBar(iBefore) * 256 + GetPointFraction(iBefore));

	// Get proportional distance (0-256)
	int iDistance=(iOffset*256)/iSize;

	// Return scaled value
	return
		(GetPointValue(iBefore) * (256-iDistance) +
		GetPointValue(iAfter) * iDistance) / 256;
}

// Returns index of last point *before* or *on* the given position, 
// or -1 if no point before it
int CVariableParameter::GetPreviousPoint(int iBar,int iFraction) const 
{
	for(int iPoint=0;iPoint<GetNumPoints();iPoint++)
	{
		if((GetPointBar(iPoint) > iBar) ||
		   (GetPointBar(iPoint) == iBar && GetPointFraction(iPoint) > iFraction))
			return iPoint-1;
		if(GetPointBar(iPoint) == iBar && GetPointFraction(iPoint) == iFraction)
			return iPoint;
	}

	// Didn't find a later, so give the last point
	return GetNumPoints()-1;
}

// Returns index of next point *after* the given position, or -1 if none
int CVariableParameter::GetNextPoint(int iBar,int iFraction) const 
{
	for(int iPoint=0;iPoint<GetNumPoints();iPoint++)
	{
		if((GetPointBar(iPoint) > iBar) ||
			(GetPointBar(iPoint) == iBar && GetPointFraction(iPoint) > iFraction))
			return iPoint;
	}
	return -1;
}

// Adds a point
void CVariableParameter::AddPoint(int iBar,int iFraction,int iValue)
{
	// Find correct place to add it in order
	for(int iPoint=0;iPoint<GetNumPoints();iPoint++)
	{
		if((GetPointBar(iPoint) > iBar) ||
			(GetPointBar(iPoint) == iBar && GetPointFraction(iPoint) > iFraction))
			break;
		if(GetPointBar(iPoint) == iBar && GetPointFraction(iPoint) == iFraction)
		{
			m_viPointBar.RemoveAt(iPoint);
			m_viPointFraction.RemoveAt(iPoint);
			m_viPointValue.RemoveAt(iPoint);
			break;
		}
	}

	m_viPointBar.InsertAt(iPoint,iBar);
	m_viPointFraction.InsertAt(iPoint,iFraction);
	m_viPointValue.InsertAt(iPoint,iValue);
}

// Removes a point
void CVariableParameter::RemovePoint(int iBar,int iFraction)
{
	for(int iPoint=0;iPoint<GetNumPoints();iPoint++)
	{
		if(GetPointBar(iPoint) == iBar && GetPointFraction(iPoint) == iFraction)
		{
			m_viPointBar.RemoveAt(iPoint);
			m_viPointFraction.RemoveAt(iPoint);
			m_viPointValue.RemoveAt(iPoint);
			return;
		}
	}
	ASSERT(FALSE);
}

// Save/load
void CVariableParameter::Serialize(CArchive& a)
{
	int iVersion=1;

	if(a.IsStoring())
	{
		a << iVersion;
		a << GetNumPoints();

		for(int iPoint=0;iPoint<GetNumPoints();iPoint++)
		{
			a << GetPointBar(iPoint);
			a << GetPointFraction(iPoint);
			a << GetPointValue(iPoint);
		}
	}
	else
	{
		a >> iVersion;
		if(iVersion!=1)
			throw "File format not supported. Make sure this is a valid file and you are "
			  "using the latest version of leafDrums.";

		int iSize;
		a >> iSize;

		for(int iPoint=0;iPoint<iSize;iPoint++)
		{
			int iBar,iFraction,iValue;
			a >> iBar;
			a >> iFraction;
			a >> iValue;
			m_viPointBar.Add(iBar);
			m_viPointFraction.Add(iFraction);
			m_viPointValue.Add(iValue);
		}
	}
}

