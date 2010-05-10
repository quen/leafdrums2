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
#include "Gate.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_SERIAL(CGate,CEffect,1)

CGate::CGate()
{
	AddParam("Open level",0,16536);
	AddParam("Close level",0,16536);
	AddParam("In speed",1,8192);
	AddParam("Out speed",1,8192);
	AddParam("Effect mix",0,256);

	m_iFade=65536;	// Start with sound on
	m_iFadeSmooth=0;
	m_fOn=TRUE;

	m_iTotalLevel=0;// Counts for averaging
	m_iCount=0;
}

void CGate::ProcessBlock(CBlock* &pBlock)
{
	int iWords=pBlock->Size();
	if(pBlock->Stereo()) iWords*=2;

	short* psData=pBlock->Data();

	int iOpen=GetParam(0);
	int iClose=GetParam(1);
	int iWet=GetParam(4);
	int iDry=256-iWet;

	int iFadeIn=GetParam(2);
	int iFadeOut=GetParam(3);

	for(int i=0;i<iWords;i++)
	{
		int iLevel=psData[i];
		if(iLevel<0) iLevel=-iLevel;

		m_iTotalLevel+=iLevel;
		m_iCount++;
		if(m_iCount==256)
		{
			m_iTotalLevel>>=8;
			if(m_fOn && m_iTotalLevel<iClose)
				m_fOn=FALSE;
			else if(!m_fOn && m_iTotalLevel>iOpen)
				m_fOn=TRUE;

			m_iCount=0;
			m_iTotalLevel=0;
		}

		if(m_fOn && m_iFade!=65536)
		{
			m_iFadeSmooth+=iFadeIn;
			if(m_iFadeSmooth>256)
			{
				m_iFade+=m_iFadeSmooth/256;
				m_iFadeSmooth&=0xff;
			}
			if(m_iFade>65536) m_iFade=65536;
		}
		else if(!m_fOn && m_iFade!=0)
		{
			m_iFadeSmooth-=iFadeOut;
			if(m_iFadeSmooth<0)
			{
				int iTemp=(-m_iFadeSmooth)/256+1;
				m_iFade-=iTemp;
				m_iFadeSmooth+=iTemp*256;
			}
			if(m_iFade<0) m_iFade=0;
		}

		int iEffect=psData[i];
		iEffect=(iEffect*m_iFade)/65536;

		psData[i]=(iDry*psData[i]+iWet*iEffect)/256;
	}
}

void CGate::Reset()
{
	m_iFade=65536;
	m_iFadeSmooth=0;
	m_fOn=TRUE;
	m_iTotalLevel=0;		
	m_iCount=0;
}

CString CGate::Name()
{
	CString strTemp="Gate";
	return strTemp;
}

void CGate::Serialize(CArchive& archive)
{
	CEffect::Serialize(archive);
}