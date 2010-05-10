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
#include "Chorus.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_SERIAL(CChorus,CEffect,1)

#define DELAYBUFFER	88200*2

CChorus::CChorus()
{
	AddParam("Pre-delay L",0,DELAYBUFFER/4-100);
	AddParam("Pre-delay R",0,DELAYBUFFER/4-100);
	AddParam("Depth",0,512);
	AddParam("Speed",1,2048);
	AddParam("Effect mix",0,256);
	AddParam("Chorus type",0,1);
	AddValueName(5,"Mono");
	AddValueName(5,"Stereo");
	AddParam("Waveform",0,1);
	AddValueName(6,"Sine");
	AddValueName(6,"Triangle");

	m_psData=new short[DELAYBUFFER];
	FillMemory((BYTE*)m_psData,
		DELAYBUFFER*sizeof(short),0);
	m_iBufferPos=0;

	m_iCurrentWavePos=0;
}

CChorus::~CChorus()
{
	delete m_psData;
}

int CChorus::EffectedSize(int iSize)
{
	int iPreDelayL=GetParam(0),iPreDelayR=GetParam(1);
	int iDepth=GetParam(2);

	// Maximum delay is the longest between two delay times
	int iMaxDelay=(iPreDelayL<iPreDelayR) ? iPreDelayR : iPreDelayL;

	return iSize+iMaxDelay+iDepth;
}

void CChorus::ProcessBlock(CBlock* &pBlock)
{
	// If mono...
	if(!pBlock->Stereo())
	{
		// Create new stereo block
		CBlock* pTemp=pBlock;
		pBlock=CBlock::Create(pTemp->Size(),TRUE);

		// Unpack data block into stereo in our storage
		short* psSrc=pTemp->Data();
		short* psTrg=pBlock->Data();
		for(int i=0;i<pTemp->Size();i++)
		{
			psTrg[i*2]=psSrc[i];
			psTrg[i*2+1]=psSrc[i];
		}

		// Delete old block
		delete pTemp;
	}

	// Extract data from block...
	short* psData=pBlock->Data();

	int iPreDelayL=GetParam(0),iPreDelayR=GetParam(1);

	int iWet=GetParam(4);
	int iDry=256-iWet;

	int iDepth=GetParam(2);
	int iSpeed=GetParam(3);

	int* piWave;
	switch(GetParam(6))
	{
	case 0:
		piWave=sg_iSine;
		break;
	case 1:
		piWave=sg_iTriangle;
		break;
	default:
		ASSERT(FALSE);
		break;
	}

	BOOL fStereo=GetParam(5);

	int iEffect;
	for(int i=0;i<pBlock->Size()*2;)
	{
		// Work out wave position
		int iWave=piWave[m_iCurrentWavePos>>8];
		int iShift=(iWave*iDepth)>>16;
		int iDelayL=iPreDelayL+iShift;
		int iDelayR;
		if(fStereo)
			iDelayR=iPreDelayR+(iDepth-iShift);
		else
			iDelayR=iPreDelayR+iShift;

		m_iCurrentWavePos+=iSpeed;
		m_iCurrentWavePos&=0xffffff;

		// Left
		m_psData[m_iBufferPos]=psData[i];

		int iCopyPosL=m_iBufferPos-iDelayL*2;
		if(iCopyPosL<0) iCopyPosL+=DELAYBUFFER;
		iEffect=(int)m_psData[iCopyPosL];
		iCopyPosL+=2;
		iCopyPosL%=DELAYBUFFER;

		psData[i]=(iDry*psData[i]+iWet*iEffect)/256;

		m_iBufferPos++; m_iBufferPos%=DELAYBUFFER; i++;

		// Right
		m_psData[m_iBufferPos]=psData[i];

		int iCopyPosR=m_iBufferPos-iDelayR*2;
		if(iCopyPosR<0) iCopyPosR+=DELAYBUFFER;
		iEffect=(int)m_psData[iCopyPosR];
		iCopyPosR+=2;
		iCopyPosR%=DELAYBUFFER;

		psData[i]=(iDry*psData[i]+iWet*iEffect)/256;

		m_iBufferPos++; m_iBufferPos%=DELAYBUFFER; i++;
	}

}

void CChorus::Reset()
{
	FillMemory((BYTE*)m_psData,
		DELAYBUFFER*sizeof(short),0);

	m_iBufferPos=0;
	m_iCurrentWavePos=0;
}

CString CChorus::Name()
{
	CString strTemp="Chorus";
	return strTemp;
}

void CChorus::Serialize(CArchive& archive)
{
	CEffect::Serialize(archive);
}