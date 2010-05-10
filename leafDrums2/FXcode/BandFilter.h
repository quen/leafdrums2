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
#ifndef BANDFILTER_H
#define BANDFILTER_H

#include "Effect.h"

class CBandFilter : public CEffect
{
	DECLARE_SERIAL(CBandFilter)

	int m_iLastInL1,m_iLastInL2,m_iLastOutL1;
	int m_iLastInR1,m_iLastInR2,m_iLastOutR1;
	int m_iLastOutL2;
	int m_iLastOutR2;

public:
	CBandFilter();
	
	virtual void ProcessBlock(CBlock* &pBlock);
	virtual void Reset();
	virtual CString Name();

	void Serialize(CArchive& archive);
};

#endif // BANDFILTER_H