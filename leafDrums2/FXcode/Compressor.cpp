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
#include "Compressor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_SERIAL(CCompressor,CEffect,1)

CCompressor::CCompressor()
{
	AddParam("Lower level",0,16536);
	AddParam("Upper level",0,16536);
	AddParam("Vol. at max",0,65536);
	AddParam("Fade speed",1,256);
	AddParam("Effect mix",0,256);

	m_iFade=65536;	// Start with sound normal
	m_iFadeTarget=65536;

	m_iTotalLevel=0;// Counts for averaging
	m_iCount=0;
}

void CCompressor::ProcessBlock(CBlock* &pBlock)
{
	int iWords=pBlock->Size();
	if(pBlock->Stereo()) iWords*=2;

	short* psData=pBlock->Data();

	int iLow=GetParam(0);
	int iHigh=GetParam(1);
	int iOnVol=GetParam(2);
	int iFade=GetParam(3);
	int iWet=GetParam(4);
	int iDry=256-iWet;

	for(int i=0;i<iWords;i++)
	{
		int iLevel=psData[i];
		if(iLevel<0) iLevel=-iLevel;

		m_iTotalLevel+=iLevel;
		m_iCount++;
		if(m_iCount==256)
		{
			m_iTotalLevel>>=8;
			if(m_iTotalLevel<iLow) m_iFadeTarget=65536;
			else if(m_iTotalLevel<iHigh)
			{
				int iPos=m_iTotalLevel-iLow;
				int iDiff=iHigh-iLow;
				m_iFadeTarget=(65536*iLow+iOnVol*(iDiff-iLow))/iDiff;
			}
			else m_iFadeTarget=iOnVol;

			m_iCount=0;
			m_iTotalLevel=0;
		}

		if(m_iFade<m_iFadeTarget)
		{
			m_iFade+=iFade;
			if(m_iFade>m_iFadeTarget) m_iFade=m_iFadeTarget;
		}
		else if(m_iFade>m_iFadeTarget)
		{
			m_iFade-=iFade;
			if(m_iFade<m_iFadeTarget) m_iFade=m_iFadeTarget;
		}

		int iEffect=psData[i];
		iEffect=(iEffect*m_iFade)/65536;
		if(iEffect>32767) iEffect=32767;
		else if(iEffect<-32768) iEffect=-32768;

		psData[i]=(iDry*psData[i]+iWet*iEffect)/256;
	}
}

void CCompressor::Reset()
{
	m_iFade=65536;
	m_iFadeTarget=65536;
	m_iTotalLevel=0;		
	m_iCount=0;
}

CString CCompressor::Name()
{
	CString strTemp="Compressor";
	return strTemp;
}

void CCompressor::Serialize(CArchive& archive)
{
	CEffect::Serialize(archive);
}