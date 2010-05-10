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
#include "Delay.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_SERIAL(CDelay,CEffect,1)

// Define so that feedback adds onto a 100% straight sound rather
// than mixing with sound
#define FEEDBACK_EXTRA

#define DELAYBUFFER	(65536*2)
#define DELAYMASK (DELAYBUFFER-1)
#define TWODELAYMASK ((DELAYBUFFER*2)-1)

CDelay::CDelay()
{
	AddParam("Delay L",10,DELAYBUFFER/2-100);
	AddParam("Delay R",10,DELAYBUFFER/2-100);
	AddParam("Feedback",0,230);
	AddParam("Effect mix",0,256);
	AddParam("Delay type",0,1);
	AddValueName(4,"Normal");
	AddValueName(4,"Crossed");
	AddParam("Phase",0,1);
	AddValueName(5,"Normal");
	AddValueName(5,"Inverted");
	AddParam("Feedback",0,1);
	AddValueName(6,"Normal");
	AddValueName(6,"Phase-inverted");

	m_psData=new short[DELAYBUFFER];
	FillMemory((BYTE*)m_psData,
		DELAYBUFFER*sizeof(short),0);
	m_psEffect=new short[DELAYBUFFER];
	FillMemory((BYTE*)m_psEffect,
		DELAYBUFFER*sizeof(short),0);
	m_iBufferPos=0;
}

CDelay::~CDelay()
{
	delete m_psData;
	delete m_psEffect;
}

int CDelay::EffectedSize(int iSize)
{
	int iDelayL=GetParam(0),iDelayR=GetParam(1);
	int iFeedback=GetParam(2);

	// Maximum delay is the longest between two delay times
	int iMaxDelay=(iDelayL<iDelayR) ? iDelayR : iDelayL;

	// Feedback when sound is silent(ish) but was previously at max level
	// so every maxdelay samples, the sound is reduced by factor 
	// feedback/256
	int iFeedbackDelay=(iFeedback==0) ? 0 : 
		(int)(((double(-5.54517744448)/log(double(iFeedback)/256))*iMaxDelay));

	return iSize+iMaxDelay+iFeedbackDelay;
}

void CDelay::ProcessBlock(CBlock* &pBlock)
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

	int iDelayL=GetParam(0),iDelayR=GetParam(1);

	int iFeedback=GetParam(2);

#ifndef FEEDBACK_EXTRA
	int iStraight=256-iFeedback;
#endif

	int iWet=GetParam(3);
	int iDry=256-iWet;

	int iCopyPosL=m_iBufferPos-iDelayL*2;
	if(iCopyPosL<0) iCopyPosL+=DELAYBUFFER;
	int iCopyPosR=m_iBufferPos-iDelayR*2+1;
	if(iCopyPosR<0) iCopyPosR+=DELAYBUFFER;

	// If cross delay, make delay take its source from other side
	BOOL fCross=GetParam(4);
	if(fCross)
	{
		iCopyPosL++;
		iCopyPosR--;
	}

	BOOL fInvert=GetParam(5);

	BOOL fInvFeed=GetParam(6);

//	for(int i=0;i<pBlock->Size()*2;)
//	{
	DWORD dwSize=pBlock->Size()*2*2 + (DWORD)psData;
	DWORD dwBuffer=(DWORD)(short*)m_psData;
	DWORD dwEffectBuffer=(DWORD)(short*)m_psEffect;
	DWORD dwBufferPos=m_iBufferPos*2;

	DWORD dwFeedbackInvXOR=fInvFeed ? 0xffffffff : 0;
	DWORD dwFeedbackInvADD=fInvFeed ? 1 : 0;

	DWORD dwOverallInvXOR=fInvert ? 0xffffffff : 0;
	DWORD dwOverallInvADD=fInvert ? 1 :0;

	DWORD dwCopyPosL=iCopyPosL*2;
	DWORD dwCopyPosR=iCopyPosR*2;

	_asm
	{
		push esi			// Save ESI etc
		push edi

		mov esi,psData		// ESI points to psData[i]
		
		delay_start:

////////// Left

//		m_psData[m_iBufferPos]=psData[i];

		// Note: in the ASM we copy left and right together
		mov edx,dwBuffer	// EDX points to m_psData
		add edx,dwBufferPos	// EDX points to m_psData[m_iBufferPos]
		
		mov eax,[esi]		// Get psData[i] and psData[i+1]
		mov [edx],eax		// Store in m_psData[m_iBufferPos] (and +1)

#ifndef FEEDBACK_EXTRA
//		iEffect=(((int)m_psData[iCopyPosL])*iStraight+

		mov ebx,dwBuffer	// EBX points to m_psData
		add ebx,dwCopyPosL	// EBX points to m_psData[iCopyPosL]
		mov ax,[ebx]		// AX contains m_psData[iCopyPosL]
		cwde				// Convert AX to EAX
		mov edx,iStraight	// EDX contains straight-number
		imul edx			// Multiply to get straight part in EAX, junk in EDX
		mov ecx,eax			// ECX = Straight part of calculation

//			((int)m_psEffect[iCopyPosL]*iInv)*iFeedback)/256;
		
		mov ebx,dwEffectBuffer	// EBX points to m_psEffect
		add ebx,dwCopyPosL	// EBX points to m_psEffect[iCopyPosL]
		mov ax,[ebx]		// AX contains m_psEffect[iCopyPosL]
		cwde				// Convert AX to EAX

		// Invert effect if necessary
		xor eax,dwFeedbackInvXOR
		add eax,dwFeedbackInvADD

		mov edx,iFeedback	// EDX contains feedback amount
		imul edx			// Multiply so EAX contains feedback part, EDX is junk
		add eax,ecx			// add straight part (ECX) to get result in EAX

		sar eax,8			// Convert to correct number range
#else
//		iEffect=((int)m_psEffect[iCopyPosL]*iInv)*iFeedback)/256+(int)m_psData[iCopyPosL];

		mov ebx,dwBuffer	// EBX points to m_psData
		add ebx,dwCopyPosL	// EBX points to m_psData[iCopyPosL]
		mov ax,[ebx]		// AX contains m_psData[iCopyPosL]
		cwde				// Convert AX to EAX
		mov ecx,eax			// ECX = Straight part of calculation

		mov ebx,dwEffectBuffer	// EBX points to m_psEffect
		add ebx,dwCopyPosL	// EBX points to m_psEffect[iCopyPosL]
		mov ax,[ebx]		// AX contains m_psEffect[iCopyPosL]
		cwde				// Convert AX to EAX

		// Invert effect if necessary
		xor eax,dwFeedbackInvXOR
		add eax,dwFeedbackInvADD

		mov edx,iFeedback	// EDX contains feedback amount
		imul edx			// Multiply so EAX contains feedback part, EDX is junk

		sar eax,8			// Convert to correct number range

		add eax,ecx			// Add straight part
#endif // FEEDBACK_EXTRA

//		iCopyPosL+=2;
//		iCopyPosL%=DELAYBUFFER;

		add dwCopyPosL,4
		and dwCopyPosL,TWODELAYMASK
		
//		if(fInvert)iEffect=-iEffect;

		// Invert effect if necessary
		xor eax,dwOverallInvXOR
		add eax,dwOverallInvADD
		mov edi,eax		// Store effect in EDI

//		psData[i]=(iDry*psData[i]+iWet*iEffect)/256;
		
		mov ax,[esi]	// AX holds data at psData[i]
		cwde			// EAX holds valid data
		mov edx,iDry	// EDX holds dry-number
		imul edx		// Multiply to get dry part in EAX (junk in EDX)
		mov ecx,eax		// Store dry part in ECX
		mov eax,iWet	// EAX holds wet
		imul edi		// Multiply by effect to get wet part in EAX (junk in EDX)
		add eax,ecx		// Add wet and dry
		sar eax,8		// Divide by 256 to get result...

#ifdef FEEDBACK_EXTRA
//		Clip the data
		cmp eax,-32768
		jl L_ClipLow

		cmp eax,32767
		jg L_ClipHigh
		
L_ClipFinish:
#endif

		mov [esi],ax	// Store AX at psData[i]

//		m_psEffect[m_iBufferPos]=iEffect;

		mov ebx,dwEffectBuffer
		add ebx,dwBufferPos
		mov [ebx],ax

//		m_iBufferPos++; m_iBufferPos%=DELAYBUFFER; i++;

		add dwBufferPos,2
		and dwBufferPos,TWODELAYMASK
		add esi,2

////////// Right

//		m_psData[m_iBufferPos]=psData[i];
// No need to do that because both parts were done at once at start

#ifndef FEEDBACK_EXTRA
//		iEffect=(((int)m_psData[iCopyPosR])*iStraight+

		mov ebx,dwBuffer	// EBX points to m_psData
		add ebx,dwCopyPosR	// EBX points to m_psData[iCopyPosR]
		mov ax,[ebx]		// AX contains m_psData[iCopyPosR]
		cwde				// Convert AX to EAX
		mov edx,iStraight	// EDX contains straight-number
		imul edx			// Multiply to get straight part in EAX, junk in EDX
		mov ecx,eax			// ECX = Straight part of calculation

//			((int)m_psEffect[iCopyPosR]*iInv)*iFeedback)/256;
		
		mov ebx,dwEffectBuffer	// EBX points to m_psEffect
		add ebx,dwCopyPosR	// EBX points to m_psEffect[iCopyPosR]
		mov ax,[ebx]		// AX contains m_psEffect[iCopyPosR]
		cwde				// Convert AX to EAX

		// Invert effect if necessary
		xor eax,dwFeedbackInvXOR
		add eax,dwFeedbackInvADD

		mov edx,iFeedback	// EDX contains feedback amount
		imul edx			// Multiply so EAX contains feedback part, EDX is junk
		add eax,ecx			// add straight part (ECX) to get result in EAX

		sar eax,8			// Convert to correct number range
#else
//		iEffect=((int)m_psEffect[iCopyPosR]*iInv)*iFeedback)/256+(int)m_psData[iCopyPosR];

		mov ebx,dwBuffer	// EBX points to m_psData
		add ebx,dwCopyPosR	// EBX points to m_psData[iCopyPosR]
		mov ax,[ebx]		// AX contains m_psData[iCopyPosR]
		cwde				// Convert AX to EAX
		mov ecx,eax			// ECX = Straight part of calculation

		mov ebx,dwEffectBuffer	// EBX points to m_psEffect
		add ebx,dwCopyPosR	// EBX points to m_psEffect[iCopyPosR]
		mov ax,[ebx]		// AX contains m_psEffect[iCopyPosR]
		cwde				// Convert AX to EAX

		// Invert effect if necessary
		xor eax,dwFeedbackInvXOR
		add eax,dwFeedbackInvADD

		mov edx,iFeedback	// EDX contains feedback amount
		imul edx			// Multiply so EAX contains feedback part, EDX is junk

		sar eax,8			// Convert to correct number range

		add eax,ecx			// Add straight part
#endif // FEEDBACK_EXTRA

//		iCopyPosR+=2;
//		iCopyPosR%=DELAYBUFFER;
	
		add dwCopyPosR,4
		and dwCopyPosR,TWODELAYMASK

//		if(fInvert)iEffect=-iEffect;

		// Invert effect if necessary
		xor eax,dwOverallInvXOR
		add eax,dwOverallInvADD
		mov edi,eax		// Store effect in EDI

//		psData[i]=(iDry*psData[i]+iWet*iEffect)/256;

		mov ax,[esi]	// AX holds data at psData[i]
		cwde			// EAX holds valid data
		mov edx,iDry	// EDX holds dry-number
		imul edx		// Multiply to get dry part in EAX (junk in EDX)
		mov ecx,eax		// Store dry part in ECX
		mov eax,iWet	// EAX holds wet
		imul edi		// Multiply by effect to get wet part in EAX (junk in EDX)
		add eax,ecx		// Add wet and dry
		sar eax,8		// Divide by 256 to get result...

#ifdef FEEDBACK_EXTRA
//		Clip the data
		cmp eax,-32768
		jl R_ClipLow

		cmp eax,32767
		jg R_ClipHigh
		
R_ClipFinish:
#endif

		mov [esi],ax	// Store AX at psData[i]

//		m_psEffect[m_iBufferPos]=iEffect;

		mov ebx,dwEffectBuffer
		add ebx,dwBufferPos
		mov [ebx],ax

//		m_iBufferPos++; m_iBufferPos%=DELAYBUFFER; i++;
		add dwBufferPos,2
		and dwBufferPos,TWODELAYMASK
		add esi,2

		cmp esi,dwSize
		jl delay_start

		pop edi
		pop esi		// Restore ESI etc

		jmp Stop

L_ClipLow:
		mov eax,-32678
		jmp L_ClipFinish

L_ClipHigh:
		mov eax,32767
		jmp L_ClipFinish

R_ClipLow:
		mov eax,-32678
		jmp R_ClipFinish

R_ClipHigh:
		mov eax,32767
		jmp R_ClipFinish

Stop:
	}

	m_iBufferPos=dwBufferPos/2;
}

void CDelay::Reset()
{
	FillMemory((BYTE*)m_psData,
		DELAYBUFFER*sizeof(short),0);
	FillMemory((BYTE*)m_psEffect,
		DELAYBUFFER*sizeof(short),0);

	m_iBufferPos=0;
}

CString CDelay::Name()
{
	CString strTemp="Delay";
	return strTemp;
}

void CDelay::Serialize(CArchive& archive)
{
	CEffect::Serialize(archive);
}