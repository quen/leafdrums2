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
#include "EQ.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_SERIAL(CEQ,CEffect,1)

CEQ::CEQ()
{
	AddParam("L. Centre",0,256);
	AddParam("L. Tone",0,256);
	AddParam("R. Centre",0,256);
	AddParam("R. Tone",0,256);
	AddParam("Effect mix",0,256);
	AddParam("Channels",0,1);
	AddValueName(5,"Mono, ignore R");
	AddValueName(5,"Stereo");

	m_iLastInL=m_iLastOutL=0;
	m_iLastInR=m_iLastOutR=0;
}

void CEQ::ProcessBlock(CBlock* &pBlock)
{
	int iWords=pBlock->Size();
	if(pBlock->Stereo()) iWords*=2;

	short* psData=pBlock->Data();

	int iLFreq=GetParam(0);
	int iLInvFreq=256-iLFreq;

	int iLTone=GetParam(1);
	int iLInvTone=256-iLTone;

	int iRFreq=GetParam(2);
	int iRInvFreq=256-iRFreq;

	int iRTone=GetParam(3);
	int iRInvTone=256-iRTone;

	// If mono, make parameters equal
	if(!GetParam(5))
	{
		iRFreq=iLFreq;
		iRInvFreq=iLInvFreq;
		iRTone=iLTone;
		iRInvTone=iLInvTone;
	}

	int iWet=GetParam(4);
	int iDry=256-iWet;

	int iLow,iHigh,iEffect;
	for(int i=0;i<iWords;)
	{
		// Left
		///////

		// Do low-pass filter
		iLow=((m_iLastInL*iLFreq)+(m_iLastOutL*iLInvFreq))/256;
		m_iLastOutL=iLow;
		m_iLastInL=psData[i];
		
		// Convert result to high-pass
		iHigh=psData[i]-iLow;

		// Balance between two filters
		iEffect=((iLTone*iHigh)+(iLInvTone*iLow))/256;

		// Set output
		psData[i]=(iDry*psData[i]+iWet*iEffect)/256;
		i++;

		// Right
		////////

		// Do low-pass filter
		iLow=((m_iLastInR*iRFreq)+(m_iLastOutR*iRInvFreq))/256;
		m_iLastOutR=iLow;
		m_iLastInR=psData[i];
		
		// Convert result to high-pass
		iHigh=psData[i]-iLow;

		// Balance between two filters
		iEffect=((iRTone*iHigh)+(iRInvTone*iLow))/256;

		// Set output
		psData[i]=(iDry*psData[i]+iWet*iEffect)/256;
		i++;
	}
}

void CEQ::Reset()
{
	m_iLastInL=m_iLastOutL=0;
	m_iLastInR=m_iLastOutR=0;
}

CString CEQ::Name()
{
	CString strTemp="EQ";
	return strTemp;
}

void CEQ::Serialize(CArchive& archive)
{
	CEffect::Serialize(archive);
}