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
#ifndef QUIETEN_H
#define QUIETEN_H

#include "Effect.h"

class CQuieten : public CEffect
{
	DECLARE_SERIAL(CQuieten)

public:
	CQuieten();
	
	virtual void ProcessBlock(CBlock* &pBlock);
	virtual void Reset();
	virtual CString Name();

	void Serialize(CArchive& archive);
};

#endif // DISTORTION_H