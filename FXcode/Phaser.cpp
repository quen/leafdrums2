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
#include "Phaser.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Define so that feedback adds onto a 100% straight sound rather
// than mixing with sound
#define FEEDBACK_EXTRA

#ifdef FEEDBACK_EXTRA
#define CLIP(x) ( (x < -32768) ? -32768 : (x>32767) ? 32767 : x)
#else
#define CLIP(x) (x)
#endif

IMPLEMENT_SERIAL(CPhaser,CEffect,1)

#define ACCURACY 256
#define FILTERRATE 64	// Filter-rate: how often the filter pos is recalced

CPhaser::CPhaser()
{
	AddParam("Depth",1,2*256);
	AddParam("Speed",1,50*256);
	AddParam("Feedback",0,230);
	AddParam("Base freq.",100,3000);
	AddParam("Effect mix",0,256);
	AddParam("Phaser type",0,1);
	AddValueName(5,"Mono");
	AddValueName(5,"Stereo");
	AddParam("Feedback",0,1);
	AddValueName(6,"Normal");
	AddValueName(6,"Phase-inverted");

	Reset();
}

void CPhaser::ProcessBlock(CBlock* &pBlock)
{
	int iWords=pBlock->Size();
	if(pBlock->Stereo()) iWords*=2;

	short* psData=pBlock->Data();

	double dDepth=((double)GetParam(0))/256;
	double dSpeed=((double)GetParam(1))/256;
	int iFeedback=GetParam(2);

#ifndef FEEDBACK_EXTRA
	int iStraight=256-iFeedback;
#endif

	double dBase=((double)GetParam(3));

	int iWet=GetParam(4);
	int iDry=256-iWet;

	BOOL fStereo=GetParam(5);
	BOOL fInvert=GetParam(6);
	if(fInvert) iFeedback=-iFeedback;

	// Calculate min and max wavepos
	double dRange=pow(2.0,dDepth);
	double dMinWave=(PI*dBase)/(double)44100;
	double dMaxWave=(PI*dBase*dRange)/(double)44100;
	double dRate=pow(dRange,dSpeed/((double)(44100/FILTERRATE)/2));

	int iCookie=GetParam(0)+GetParam(1)+GetParam(2)+GetParam(3)+
		GetParam(4)+GetParam(5)+GetParam(6);
	
	int iEffect;

	if(iCookie!=m_iCookie)
	{
		m_iCookie=iCookie;

		m_dLFactor=dRate;
		m_dLWavePos=dMinWave;

		if(fStereo)
		{
			m_dRFactor=1.0/dRate;
			m_dRWavePos=dMaxWave;
		}
		else
		{
			m_dRFactor=dRate;
			m_dRWavePos=dMinWave;
		}

		m_iRecalcCount=0;
	}

	int iX1;

	for(int i=0;i<iWords;)
	{
		if(m_iRecalcCount==0)
		{
			// Left
			///////

			// Adjust filter frequencies
			m_dLWavePos=m_dLWavePos*m_dLFactor;
			if(m_dLWavePos>dMaxWave)
				m_dLFactor=1.0/dRate;
			else if(m_dLWavePos<dMinWave)
				m_dLFactor=dRate;

			// Recalc coefficient
			m_iLCoefficient=(int)
				(ACCURACY*((1.0-m_dLWavePos) / (1.0+m_dLWavePos)));

			// Right
			////////

			// Adjust filter frequencies
			m_dRWavePos=m_dRWavePos*m_dRFactor;
			if(m_dRWavePos>dMaxWave)
				m_dRFactor=1.0/dRate;
			else if(m_dRWavePos<dMinWave)
				m_dRFactor=dRate;

			// Recalc coefficient
			m_iRCoefficient=(int)
				(ACCURACY*((1.0-m_dRWavePos) / (1.0+m_dRWavePos)));

			m_iRecalcCount=FILTERRATE-1;
		}
		else m_iRecalcCount--;

		// Left
		///////

#ifndef FEEDBACK_EXTRA
		iX1=((int)psData[i]*iStraight+m_iLY4*iFeedback)/256;
#else
		iX1=(int)psData[i] + ((m_iLY4*iFeedback)/256);
#endif

		m_iLY1=(m_iLCoefficient * (m_iLY1+iX1) - m_iLX1*ACCURACY)/ACCURACY;
		m_iLX1=iX1;

		m_iLY2=(m_iLCoefficient * (m_iLY2+m_iLY1) - m_iLX2*ACCURACY)/ACCURACY;
		m_iLX2=m_iLY1;
		
		m_iLY3=(m_iLCoefficient * (m_iLY3+m_iLY2) - m_iLX3*ACCURACY)/ACCURACY;
		m_iLX3=m_iLY2;

		m_iLY4=(m_iLCoefficient * (m_iLY4+m_iLY3) - m_iLX4*ACCURACY)/ACCURACY;
		m_iLX4=m_iLY3;

		iEffect=m_iLY4;

		// Write output
		psData[i]=CLIP((iDry*psData[i]+iWet*iEffect)/256);

		i++;

		// Right
		////////

#ifndef FEEDBACK_EXTRA
		iX1=((int)psData[i]*iStraight+m_iRY4*iFeedback)/256;
#else
		iX1=(int)psData[i] + ((m_iRY4*iFeedback)/256);
#endif

		m_iRY1=(m_iRCoefficient * (m_iRY1+iX1) - m_iRX1*ACCURACY)/ACCURACY;
		m_iRX1=iX1;

		m_iRY2=(m_iRCoefficient * (m_iRY2+m_iRY1) - m_iRX2*ACCURACY)/ACCURACY;
		m_iRX2=m_iRY1;
		
		m_iRY3=(m_iRCoefficient * (m_iRY3+m_iRY2) - m_iRX3*ACCURACY)/ACCURACY;
		m_iRX3=m_iRY2;

		m_iRY4=(m_iRCoefficient * (m_iRY4+m_iRY3) - m_iRX4*ACCURACY)/ACCURACY;
		m_iRX4=m_iRY3;

		iEffect=m_iRY4;

		// Write output
		psData[i]=CLIP((iDry*psData[i]+iWet*iEffect)/256);

		i++;
	}
}

void CPhaser::Reset()
{
	m_dLWavePos=0;
	m_dLFactor=0;
	m_iLX1=m_iLX2=m_iLX3=m_iLX4=0;
	m_iLY1=m_iLY2=m_iLY3=m_iLY4=0;
	m_dRWavePos=0;
	m_dRFactor=0;
	m_iRX1=m_iRX2=m_iRX3=m_iRX4=0;
	m_iRY1=m_iRY2=m_iRY3=m_iRY4=0;

	m_iLCoefficient=m_iRCoefficient=m_iRecalcCount=0;

	m_iCookie=0;
}

CString CPhaser::Name()
{
	CString strTemp="Phaser";
	return strTemp;
}

void CPhaser::Serialize(CArchive& archive)
{
	CEffect::Serialize(archive);
}