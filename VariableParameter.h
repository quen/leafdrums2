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
#ifndef VARIABLEPARAMETER_H
#define VARIABLEPARAMETER_H

#include "vec.h"

class CVariableParameter : public CObject
{	
private:
	// Bar number of each point
	vec<int> m_viPointBar;

	// Proportional offset of each point within bar (0-255)
	vec<int> m_viPointFraction;

	// Actual value of each point
	vec<int> m_viPointValue;

	// Default value to be assumed when no points
	int m_iDefaultValue;

public:
	// Construct with default value
	CVariableParameter(int iDefaultValue) { m_iDefaultValue=iDefaultValue; }

	// Returns value of parameter at the given position (fraction is a
	// fractional bar position, 0-255)
	int GetValueAt(int iBar,int iFraction) const;

	// Returns index of last point *before* or *on* the given position, 
	// or -1 if no point before it
	int GetPreviousPoint(int iBar,int iFraction) const;

	// Returns index of next point *after* the given position, or -1 if none
	int GetNextPoint(int iBar,int iFraction) const;

	// Adds a point
	void AddPoint(int iBar,int iFraction,int iValue);

	// Removes a point
	void RemovePoint(int iBar,int iFraction);

	// Obtains point information
	int GetNumPoints() const { return m_viPointValue.Size(); }
	int GetPointBar(int iPoint) const { return m_viPointBar[iPoint]; }
	int GetPointFraction(int iPoint) const { return m_viPointFraction[iPoint]; }
	int GetPointValue(int iPoint) const { return m_viPointValue[iPoint]; }

	// Save/load
	void Serialize(CArchive& archive);
};

#endif // VARIABLEPARAMETER_H
