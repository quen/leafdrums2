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
#include "Flanger.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Define so that feedback adds onto a 100% straight sound rather
// than mixing with sound
//#define FEEDBACK_EXTRA

// Define so that feedback mixing reduces sound, but not as much
//#define FEEDBACK_SOME_EXTRA

#ifdef FEEDBACK_EXTRA
#define NEED_CLIP
#endif

#ifdef FEEDBACK_SOME_EXTRA
#define NEED_CLIP
#endif

#ifdef NEED_CLIP
#define CLIP(x) ( (x < -32768) ? -32768 : ((x>32767) ? 32767 : x))
#else
#define CLIP(x) (x)
#endif


IMPLEMENT_SERIAL(CFlanger,CEffect,1)

#define DELAYBUFFER	88200*2

CFlanger::CFlanger()
{
	AddParam("Pre-delay",0,DELAYBUFFER/4-100);
	AddParam("Feedback",0,256);
	AddParam("Depth",0,512);
	AddParam("Speed",1,2048);
	AddParam("Effect mix",0,256);
	AddParam("Flanger type",0,1);
	AddValueName(5,"Mono");
	AddValueName(5,"Stereo");
	AddParam("Waveform",0,1);
	AddValueName(6,"Sine");
	AddValueName(6,"Triangle");
	AddParam("Feedback",0,1);
	AddValueName(7,"Normal");
	AddValueName(7,"Phase-inverted");


	m_psData=new short[DELAYBUFFER];
	FillMemory((BYTE*)m_psData,
		DELAYBUFFER*sizeof(short),0);
	m_psEffect=new short[DELAYBUFFER];
	FillMemory((BYTE*)m_psEffect,
		DELAYBUFFER*sizeof(short),0);
	m_iBufferPos=0;

	m_iCurrentWavePos=0;
}

CFlanger::~CFlanger()
{
	delete m_psData;
	delete m_psEffect;
}

int CFlanger::EffectedSize(int iSize)
{
	int iPreDelay=GetParam(0);
	int iDepth=GetParam(2);

	return iSize+iPreDelay+iDepth;
}

void CFlanger::ProcessBlock(CBlock* &pBlock)
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

	int iPreDelay=GetParam(0);

	int iFeedback=GetParam(1);

#ifndef FEEDBACK_EXTRA

#ifdef FEEDBACK_SOME_EXTRA
	int iStraight=256-iFeedback/2;
#else
	int iStraight=256-iFeedback;
#endif

#endif

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
	BOOL fInvFeed=GetParam(7);
	int iInv=fInvFeed? -1 : 1;

	int iEffect;
	for(int i=0;i<pBlock->Size()*2;)
	{
		// Work out wave position
		int iWave=piWave[m_iCurrentWavePos>>8];
		int iShift=(iWave*iDepth)>>16;
		if(iShift<1) iShift=1;

		int iDelayL=iPreDelay+iShift;
		int iDelayR;
		if(fStereo)
			iDelayR=iPreDelay+(iDepth-iShift);
		else
			iDelayR=iPreDelay+iShift;

		m_iCurrentWavePos+=iSpeed;
		m_iCurrentWavePos&=0xffffff;

		// Left
		m_psData[m_iBufferPos]=psData[i];

		int iCopyPosL=m_iBufferPos-iDelayL*2;
		if(iCopyPosL<0) iCopyPosL+=DELAYBUFFER;
		int iFeedbackPosL=m_iBufferPos-iShift*2;
		if(iFeedbackPosL<0) iFeedbackPosL+=DELAYBUFFER;

#ifndef FEEDBACK_EXTRA
		iEffect=(((int)m_psData[iCopyPosL])*iStraight+
			((int)m_psEffect[iFeedbackPosL]*iInv)*iFeedback)/256;
#else
		iEffect=(int)m_psData[iCopyPosL]+
			((((int)m_psEffect[iFeedbackPosL]*iInv)*iFeedback)/256);
#endif

		iCopyPosL+=2;
		iCopyPosL%=DELAYBUFFER;

		psData[i]=(iDry*psData[i]+iWet*iEffect)/256;

		m_psEffect[m_iBufferPos]=CLIP(iEffect);

		m_iBufferPos++; m_iBufferPos%=DELAYBUFFER; i++;

		// Right
		m_psData[m_iBufferPos]=psData[i];

		int iCopyPosR=m_iBufferPos-iDelayR*2;
		if(iCopyPosR<0) iCopyPosR+=DELAYBUFFER;
		int iFeedbackPosR=m_iBufferPos-(iDepth-iShift)*2;
		if(iFeedbackPosR<0) iFeedbackPosR+=DELAYBUFFER;

#ifndef FEEDBACK_EXTRA
		iEffect=(((int)m_psData[iCopyPosR])*iStraight+
			((int)m_psEffect[iFeedbackPosR]*iInv)*iFeedback)/256;
#else
		iEffect=(int)m_psData[iCopyPosR]+
			((((int)m_psEffect[iFeedbackPosR]*iInv)*iFeedback)/256);
#endif

		iCopyPosR+=2;
		iCopyPosR%=DELAYBUFFER;

		psData[i]=(iDry*psData[i]+iWet*iEffect)/256;
		m_psEffect[m_iBufferPos]=CLIP(iEffect);

		m_iBufferPos++; m_iBufferPos%=DELAYBUFFER; i++;
	}

}

void CFlanger::Reset()
{
	FillMemory((BYTE*)m_psData,
		DELAYBUFFER*sizeof(short),0);
	FillMemory((BYTE*)m_psEffect,
		DELAYBUFFER*sizeof(short),0);

	m_iBufferPos=0;
	m_iCurrentWavePos=0;
}

CString CFlanger::Name()
{
	CString strTemp="Flanger";
	return strTemp;
}

void CFlanger::Serialize(CArchive& archive)
{
	CEffect::Serialize(archive);
}