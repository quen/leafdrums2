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
#ifndef EFFECT_H
#define EFFECT_H

#include "..\Block.h"

#define PI 3.141592654

class CEffect : public CObject
{
	DECLARE_SERIAL(CEffect)

private:
	// Parameter data
	int m_iParameters;
	CString** m_ppstrParamName;
	int* m_piParamMin;
	int* m_piParamMax;
	int* m_piParamValue;

	int* m_piParamValueNames;
	CString*** m_pppstrValueName;

public:
	CEffect();
	virtual ~CEffect();

	// Take block and apply effect to it (if block needs
	// reallocating, that can be done)
	virtual void ProcessBlock(CBlock* &pBlock);

	// Clear any buffers that might be in use for delay etc.
	virtual void Reset();

	// Return the name of the effect
	virtual CString Name();

	virtual int EffectedSize(int iSize) { return iSize; }

	// Code for information about parameters
	int Parameters();
	CString ParamName(int iParam);
	void ParamLimits(int iParam,int &iMin,int &iMax);
	int GetParam(int iParam);
	void SetParam(int iParam,int iValue);
	BOOL ValueNames(int iParam);
	CString ValueName(int iParam,int iValue);

	void Serialize(CArchive& archive);

	// Used in cloning; sets all the parameters of this effect 
	// from the given effect
	void SetFrom(CEffect* pEffect);

protected:
	void AddParam(CString strName,int iMin,int iMax);
	void AddValueName(int iParam,CString strName);
};

extern int sg_iSine[65536],sg_iTriangle[65536];
void CalcWaveBuffers();

#endif EFFECT_H