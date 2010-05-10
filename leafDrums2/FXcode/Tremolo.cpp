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
#include "Tremolo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_SERIAL(CTremolo,CEffect,1)

#define DELAYBUFFER	88200*2

#define FADERES	32768

CTremolo::CTremolo()
{
	AddParam("Depth",0,FADERES);
	AddParam("Speed",1,2048);
	AddParam("Effect mix",0,256);
	AddParam("Tremolo type",0,1);
	AddValueName(3,"Mono");
	AddValueName(3,"Stereo");
	AddParam("Waveform",0,1);
	AddValueName(4,"Sine");
	AddValueName(4,"Triangle");

	m_iCurrentWavePos=0;
}

CTremolo::~CTremolo()
{
}

void CTremolo::ProcessBlock(CBlock* &pBlock)
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

	int iWet=GetParam(2);
	int iDry=256-iWet;

	int iDepth=GetParam(0);
	int iSpeed=GetParam(1);

	int* piWave;
	switch(GetParam(4))
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

	BOOL fStereo=GetParam(3);

	int iEffect;
	for(int i=0;i<pBlock->Size()*2;)
	{
		// Work out wave position
		int iWave=piWave[m_iCurrentWavePos>>8];
		int iShift=(iWave*iDepth)>>16;
		int iFadeL=FADERES-iShift;
		int iFadeR;
		if(fStereo)
			iFadeR=FADERES-(iDepth-iShift);
		else
			iFadeR=FADERES-iShift;

		m_iCurrentWavePos+=iSpeed;
		m_iCurrentWavePos&=0xffffff;

		// Left
		iEffect=(int)psData[i];
		iEffect=(iEffect*iFadeL)/FADERES;

		psData[i]=(iDry*psData[i]+iWet*iEffect)/256;

		i++;

		// Right
		iEffect=(int)psData[i];
		iEffect=(iEffect*iFadeR)/FADERES;

		psData[i]=(iDry*psData[i]+iWet*iEffect)/256;

		i++;
	}

}

void CTremolo::Reset()
{
	m_iCurrentWavePos=0;
}

CString CTremolo::Name()
{
	CString strTemp="Tremolo";
	return strTemp;
}

void CTremolo::Serialize(CArchive& archive)
{
	CEffect::Serialize(archive);
}