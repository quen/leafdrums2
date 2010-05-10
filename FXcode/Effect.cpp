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
#include "math.h"
#include "Effect.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_SERIAL(CEffect,CObject,1)

CEffect::CEffect()
{
	m_iParameters=0;
	m_ppstrParamName=NULL;
	m_piParamMin=NULL;
	m_piParamMax=NULL;
	m_piParamValue=NULL;

	m_piParamValueNames=NULL;
	m_pppstrValueName=NULL;
}

CEffect::~CEffect()
{
	for(int iParam=0;iParam<m_iParameters;iParam++)
	{
		delete m_ppstrParamName[iParam];
		for(int iName=0;iName<m_piParamValueNames[iParam];iName++)
			delete m_pppstrValueName[iParam][iName];
		if(m_pppstrValueName[iParam]!=NULL)
			delete m_pppstrValueName[iParam];
	}

	if(m_ppstrParamName) delete m_ppstrParamName;
	if(m_piParamMin) delete m_piParamMin;
	if(m_piParamMax) delete m_piParamMax;
	if(m_piParamValue) delete m_piParamValue;
	if(m_piParamValueNames) delete m_piParamValueNames;
	if(m_pppstrValueName) delete m_pppstrValueName;
}

void CEffect::SetFrom(CEffect* pEffect)
{
	ASSERT(pEffect->Name()==Name());

	for(int iParam=0;iParam<Parameters();iParam++)
	{
		SetParam(iParam,pEffect->GetParam(iParam));
	}
}

void CEffect::Serialize(CArchive& archive)
{
	CObject::Serialize(archive);

	if(archive.IsStoring())
	{
		archive << m_iParameters;
		for(int iParam=0;iParam<m_iParameters;iParam++)
		{
			archive << m_piParamMin[iParam];
			archive << m_piParamMax[iParam];
			archive << m_piParamValue[iParam];
			archive << (*m_ppstrParamName[iParam]);

			archive << m_piParamValueNames[iParam];
			for(int iName=0;iName<m_piParamValueNames[iParam];iName++)
				archive << (*m_pppstrValueName[iParam][iName]);
		}
	}
	else
	{
		archive >> m_iParameters;
		m_ppstrParamName=new CString*[m_iParameters];
		m_piParamMin=new int[m_iParameters];
		m_piParamMax=new int[m_iParameters];
		m_piParamValue=new int[m_iParameters];
		m_piParamValueNames=new int[m_iParameters];
		m_pppstrValueName=new CString**[m_iParameters];

		for(int iParam=0;iParam<m_iParameters;iParam++)
		{
			archive >> m_piParamMin[iParam];
			archive >> m_piParamMax[iParam];
			archive >> m_piParamValue[iParam];
			m_ppstrParamName[iParam]=new CString;
			archive >> (*m_ppstrParamName[iParam]);

			archive >> m_piParamValueNames[iParam];
			m_pppstrValueName[iParam]=new CString*[m_piParamValueNames[iParam]];
			for(int iName=0;iName<m_piParamValueNames[iParam];iName++)
			{
				m_pppstrValueName[iParam][iName]=new CString;
				archive >> (*m_pppstrValueName[iParam][iName]);
			}
		}
	}
}

void CEffect::ProcessBlock(CBlock* &pBlock)
{
	ASSERT(FALSE);
}

void CEffect::Reset()
{
	ASSERT(FALSE);
}

int CEffect::Parameters()
{
	return m_iParameters;
}

CString CEffect::ParamName(int iParam)
{
	ASSERT(iParam<m_iParameters);
	ASSERT(iParam>=0);

	return *m_ppstrParamName[iParam];
}

void CEffect::ParamLimits(int iParam,int& iMin,int &iMax)
{
	ASSERT(iParam<m_iParameters);
	ASSERT(iParam>=0);

	iMin=m_piParamMin[iParam];
	iMax=m_piParamMax[iParam];
}

int CEffect::GetParam(int iParam)
{
	ASSERT(iParam<m_iParameters);
	ASSERT(iParam>=0);

	return m_piParamValue[iParam];
}

void CEffect::SetParam(int iParam,int iValue)
{
	ASSERT(iParam<m_iParameters);
	ASSERT(iParam>=0);

	ASSERT(iValue>=m_piParamMin[iParam]);
	ASSERT(iValue<=m_piParamMax[iParam]);

	m_piParamValue[iParam]=iValue;
}

BOOL CEffect::ValueNames(int iParam)
{
	ASSERT(iParam<m_iParameters);
	ASSERT(iParam>=0);

	return m_piParamValueNames[iParam];
}

CString CEffect::ValueName(int iParam,int iValue)
{
	ASSERT(iParam<m_iParameters);
	ASSERT(iParam>=0);

	ASSERT(iValue>=0);
	ASSERT(iValue<m_piParamValueNames[iParam]);

	return *(m_pppstrValueName[iParam][iValue]);
}

void CEffect::AddParam(CString strName,int iMin,int iMax)
{
	m_iParameters++;
	m_ppstrParamName=(CString**)realloc(
		m_ppstrParamName,sizeof(CString*)*m_iParameters);
	m_piParamMin=(int*)realloc(m_piParamMin,
		sizeof(int)*m_iParameters);
	m_piParamMax=(int*)realloc(m_piParamMax,
		sizeof(int)*m_iParameters);
	m_piParamValue=(int*)realloc(m_piParamValue,
		sizeof(int)*m_iParameters);
	m_piParamValueNames=(int*)realloc(m_piParamValueNames,
		sizeof(int)*m_iParameters);
	m_pppstrValueName=(CString***)realloc(m_pppstrValueName,
		sizeof(CString**)*m_iParameters);

	m_ppstrParamName[m_iParameters-1]=new CString(strName);
	m_piParamMin[m_iParameters-1]=iMin;
	m_piParamMax[m_iParameters-1]=iMax;
	m_piParamValue[m_iParameters-1]=iMin;
	m_piParamValueNames[m_iParameters-1]=0;
	m_pppstrValueName[m_iParameters-1]=NULL;
}

void CEffect::AddValueName(int iParam,CString strName)
{
	ASSERT(iParam<m_iParameters);
	ASSERT(iParam>=0);

	m_piParamValueNames[iParam]++;
	m_pppstrValueName[iParam]=(CString**)realloc(m_pppstrValueName[iParam],
		sizeof(CString*)*m_piParamValueNames[iParam]);

	m_pppstrValueName[iParam][m_piParamValueNames[iParam]-1]=
		new CString(strName);
}

CString CEffect::Name()
{
	CString strTemp="Effect Base Class";
	return strTemp;
}

/////////////////////////////////////////////////////////////////////////////
// Sine and triangle buffers for use in various effects (16-bit res, 16-bit steps)

int sg_iSine[65536],sg_iTriangle[65536];

void CalcWaveBuffers()
{
	for(int i=0;i<65536;i++)
	{
		double f=(double)i;
		f=f*(2*3.141592654)/65536;
		f=(1+sin(f))*32767;
		sg_iSine[i]=(int)f;

		if(i<65536/4)
			sg_iTriangle[i]=i*2+32768;
		else if(i<65536*3/4)
			sg_iTriangle[i]=65535-((i-65536/4)*2);
		else
			sg_iTriangle[i]=(i-(65536*3/4))*2;
	}
}


