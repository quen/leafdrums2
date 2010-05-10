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
#include "BandFilter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_SERIAL(CBandFilter,CEffect,1)

CBandFilter::CBandFilter()
{
	AddParam("Min freq.",0,256);
	AddParam("Max freq",0,256);
	AddParam("Effect mix",0,256);

	Reset();
}

void CBandFilter::ProcessBlock(CBlock* &pBlock)
{
	int iWords=pBlock->Size();
	if(pBlock->Stereo()) iWords*=2;

	short* psData=pBlock->Data();

	int iMinFreq=GetParam(0);
	int iMinInvFreq=256-iMinFreq;

	int iMaxFreq=GetParam(1);
	int iMaxInvFreq=256-iMaxFreq;

	int iWet=GetParam(2);
	int iDry=256-iWet;

	int iLow,iHigh,iEffect;
	for(int i=0;i<iWords;)
	{
		// Left
		///////

		// Do low-pass filter (1) with iMaxFreq, on input
		iLow=((m_iLastInL1*iMaxFreq)+(m_iLastOutL1*iMaxInvFreq))/256;
		m_iLastOutL1=iLow;
		m_iLastInL1=psData[i];
		
		// Do a high-pass filter (2) with iMinFreq, on iLow output
		iHigh=((m_iLastInL2*iMinFreq)+(m_iLastOutL2*iMinInvFreq))/256;
		m_iLastOutL2=iHigh;
		m_iLastInL2=iLow;
		iHigh=m_iLastInL2-iHigh;	// convert low-pass to high-pass

		// Fix to get effect
		iEffect=iHigh;

		// Store effect
		psData[i]=(iDry*psData[i]+iWet*iEffect)/256;
		i++;

		// Right
		////////

		// Do low-pass filter (1) with iMaxFreq, on input
		iLow=((m_iLastInR1*iMaxFreq)+(m_iLastOutR1*iMaxInvFreq))/256;
		m_iLastOutR1=iLow;
		m_iLastInR1=psData[i];
		
		// Do a high-pass filter (2) with iMinFreq, on iLow output
		iHigh=((m_iLastInR2*iMinFreq)+(m_iLastOutR2*iMinInvFreq))/256;
		m_iLastOutR2=iHigh;
		m_iLastInR2=iLow;
		iHigh=m_iLastInR2-iHigh;	// convert low-pass to high-pass

		// Fix to get effect
		iEffect=iHigh;

		// Store effect
		psData[i]=(iDry*psData[i]+iWet*iEffect)/256;
		i++;

	}
}

void CBandFilter::Reset()
{
	m_iLastInL1=m_iLastInL2=0;
	m_iLastInR1=m_iLastInR2=0;
	m_iLastOutL1=m_iLastOutL2=0;
	m_iLastOutR1=m_iLastOutR2=0;
}

CString CBandFilter::Name()
{
	CString strTemp="Filter (band-pass)";
	return strTemp;
}

void CBandFilter::Serialize(CArchive& archive)
{
	CEffect::Serialize(archive);
}