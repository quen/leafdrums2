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
#ifndef PHASER_H
#define PHASER_H

#include "Effect.h"

class CPhaser : public CEffect
{
	DECLARE_SERIAL(CPhaser)

	int m_iCookie;

	double m_dLWavePos;
	double m_dLFactor;
	int m_iLCoefficient;
	int m_iLX1,m_iLX2,m_iLX3,m_iLX4;
	int m_iLY1,m_iLY2,m_iLY3,m_iLY4;
	double m_dRWavePos;
	double m_dRFactor;
	int m_iRCoefficient;
	int m_iRX1,m_iRX2,m_iRX3,m_iRX4;
	int m_iRY1,m_iRY2,m_iRY3,m_iRY4;

	int m_iRecalcCount;

public:
	CPhaser();
	
	virtual void ProcessBlock(CBlock* &pBlock);
	virtual void Reset();
	virtual CString Name();

	void Serialize(CArchive& archive);
};

#endif // PHASER_H