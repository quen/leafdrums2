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
#ifndef GATE_H
#define GATE_H

#include "Effect.h"

class CGate : public CEffect
{
	DECLARE_SERIAL(CGate)

	int m_iFade,m_iFadeSmooth;
	BOOL m_fOn;

	int m_iCount,m_iTotalLevel;

public:
	CGate();
	
	virtual void ProcessBlock(CBlock* &pBlock);
	virtual void Reset();
	virtual CString Name();

	void Serialize(CArchive& archive);
};

#endif // GATE_H