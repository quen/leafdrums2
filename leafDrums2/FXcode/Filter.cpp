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
#include "Filter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_SERIAL(CFilter,CEffect,1)

CFilter::CFilter()
{
	AddParam("L Freq.",0,256);
	AddParam("R Freq.",0,256);
	AddParam("Effect mix",0,256);
	AddParam("Channels",0,1);
	AddValueName(3,"Mono, ignore R");
	AddValueName(3,"Stereo");
	AddParam("Filter type",0,1);
	AddValueName(4,"Low-pass");
	AddValueName(4,"High-pass");

	m_iLastInL=m_iLastOutL=0;
	m_iLastInR=m_iLastOutR=0;
}

void CFilter::ProcessBlock(CBlock* &pBlock)
{
	int iWords=pBlock->Size();
	if(pBlock->Stereo()) iWords*=2;

	short* psData=pBlock->Data();

	int iLFreq=GetParam(0);
	int iLInvFreq=256-iLFreq;

	int iRFreq=GetParam(1);
	int iRInvFreq=256-iRFreq;

	// If mono, make parameters equal
	if(!GetParam(3))
	{
		iRFreq=iLFreq;
		iRInvFreq=iLInvFreq;
	}

	int iWet=GetParam(2);
	int iDry=256-iWet;

	BOOL fHigh=GetParam(4);

	for(int i=0;i<iWords;)
	{
		int iEffect=((m_iLastInL*iLFreq)+(m_iLastOutL*iLInvFreq))/256;
		m_iLastOutL=iEffect;
		m_iLastInL=psData[i];
		
		if(fHigh) iEffect=psData[i]-iEffect;

		psData[i]=(iDry*psData[i]+iWet*iEffect)/256;
		i++;

		iEffect=((m_iLastInR*iRFreq)+(m_iLastOutR*iRInvFreq))/256;
		m_iLastOutR=iEffect;
		m_iLastInR=psData[i];

		if(fHigh) iEffect=psData[i]-iEffect;
		
		psData[i]=(iDry*psData[i]+iWet*iEffect)/256;
		i++;
	}
}

void CFilter::Reset()
{
	m_iLastInL=m_iLastOutL=0;
	m_iLastInR=m_iLastOutR=0;
}

CString CFilter::Name()
{
	CString strTemp="Filter (low/high)";
	return strTemp;
}

void CFilter::Serialize(CArchive& archive)
{
	CEffect::Serialize(archive);
}