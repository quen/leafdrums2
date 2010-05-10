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
#include <math.h>
#include "Sound.h"

#include "FileSystem.h"

//#include "LeafDrumsView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_SERIAL(CSound,CObject,1)

#define MAXEFFECTEDLENGTH 4

CSound::CSound()
{
	for(int i=0;i<NUMEFFECTS;i++)
		m_pEffect[i]=NULL;

	m_bReverse=FALSE;

	m_bFit=FALSE;
	m_iFitLength=16;
	m_iFitTrimStart=0;
	m_iFitTrimLength=16;

	m_bTrimFade=FALSE;
	m_iTrimStart=0;
	m_iTrimLength=0;
	m_iFadeIn=0;
	m_iFadeOut=0;

	m_bTune=FALSE;
	m_iTune=0;

	m_iLevel=128; m_iAccent=384; m_iPan=128;
	m_bMute=FALSE;
}

BOOL CSound::HasSample()
{
	return(m_smpOrig.Size()>0);
}

void CSound::Clear()
{
	// Clear names
	m_strFilename=m_strFolder="";

	// Clear samples
	m_smpOrig.Set(NULL,0);
	m_vsmpMod.Clear();
	m_vsmpFX.Clear();
	m_viSampleTempos.Clear();

	// Clear variables
	for(int i=0;i<NUMEFFECTS;i++)
	{
		if(m_pEffect[i]) delete m_pEffect[i];
		m_pEffect[i]=NULL;
	}

	m_bReverse=FALSE;

	m_bFit=FALSE;
	m_iFitLength=16;
	m_iFitTrimStart=0;
	m_iFitTrimLength=16;

	m_bTrimFade=FALSE;
	m_iTrimStart=0;
	m_iTrimLength=0;
	m_iFadeIn=0;
	m_iFadeOut=0;

	m_bTune=FALSE;
	m_iTune=0;

	m_iLevel=128; m_iAccent=384; m_iPan=128;
	m_bMute=FALSE;
}

CSound::~CSound()
{
	for(int i=0;i<NUMEFFECTS;i++)
		if(m_pEffect[i]) delete m_pEffect[i];
}


int CSound::GetSampleIndex(int iTempo)
{
	int iDesiredTempo=GetDesiredTempo(iTempo);
	int iIndex=m_viSampleTempos.Find(iDesiredTempo);
	if(iIndex==-1)
	{
		iIndex=m_viSampleTempos.Size();
		m_viSampleTempos.Add(iDesiredTempo);
		m_vsmpMod.Add(new CSample());
		m_vsmpFX.Add(new CSample());
		ApplyMods(iIndex,FALSE);
		ApplyFX(iIndex,FALSE);
	}
	return iIndex;
}

// Mix the given portion of the sample into the given buffer, 
// adjusting level as required
void CSound::Play(CMixBuffer& mixTarget,int iTargetOffset,
	int iOffset,int iLength,
	int iLevelLStart,const int iLevelLIncrement, 
	int iLevelRStart,const int iLevelRIncrement,
	const BOOL fAccent,const int iTempo)
{
	CSingleLock sl(&m_cs,TRUE);

	// Don't play if muted
	if(m_bMute) return;

	// Handle negative offsets
	if(iOffset<0)
	{
		iTargetOffset+=-iOffset;
		iLength-=-iOffset;
		iLevelLStart+=(-iOffset)*iLevelLIncrement;
		iLevelRStart+=(-iOffset)*iLevelRIncrement;
		iOffset=0;
	}

	// See if we've got the sample for this tempo
	int iIndex=GetSampleIndex(iTempo);

	// Obtain the buffer addresses
	short* psSource=&( ((short*)(m_vsmpFX[iIndex].Data())) [iOffset*2] );
	int* piTarget=&( ((int*)(mixTarget.Data())) [iTargetOffset*2] ); 

	// Obtain number of samples remaining to do
	int iSamplesLeft=m_vsmpFX[iIndex].Size()-iOffset;
	if(iSamplesLeft > iLength) iSamplesLeft=iLength;
	if(iSamplesLeft<=0) return;

	// Obtain base mixing levels
	DWORD 
		dwBase=(fAccent ? ((m_iAccent * m_iLevel) >> 8) : m_iLevel);

	// See if this is static in level (to use faster copy code)
	if(iLevelLIncrement==0 && iLevelRIncrement==0)
	{
		// Obtain external mixing levels
		DWORD 
			dwExtLeft=iLevelLStart>>16,
			dwExtRight=iLevelRStart>>16;

		DWORD 
			dwLeft=((256-m_iPan)*dwBase*dwExtLeft)>>15,
			dwRight=((m_iPan)*dwBase*dwExtRight)>>15;

		_asm
		{
			push esi
			push edi
			push ebp

			// Obtain variables
			mov esi,psSource
			mov edi,piTarget
			mov ebx,dwLeft
			mov ecx,dwRight
			mov eax,0
			mov ebp,iSamplesLeft
			
		SampleMixLoop:

			// Left
			mov ax,[esi]
			cwde
			imul ebx
			sar eax,8
			add [edi],eax

			// Right
			mov ax,[esi+2]
			cwde
			imul ecx
			sar eax,8
			add [edi+4],eax

			add edi,8
			add esi,4

			dec ebp
			jnz SampleMixLoop

			pop ebp
			pop edi
			pop esi
		}
	}
	else
	{
		// Ok, copy data
		while(iSamplesLeft>0)
		{
			int iSamplesThisRun=
				(iSamplesLeft<256) ? iSamplesLeft : 256;

			// Obtain external levels
			if(iLevelLStart<0) iLevelLStart=0;
			if(iLevelRStart<0) iLevelRStart=0;
			DWORD 
				dwExtLeft=iLevelLStart>>16,
				dwExtRight=iLevelRStart>>16;

			DWORD 
				dwLeft=((256-m_iPan)*dwBase*dwExtLeft)>>15,
				dwRight=((m_iPan)*dwBase*dwExtRight)>>15;

			// Copy data
			_asm
			{
				push esi
				push edi
				push ebp

				// Obtain variables
				mov esi,psSource
				mov edi,piTarget
				mov ebx,dwLeft
				mov ecx,dwRight
				mov eax,0
				mov ebp,iSamplesThisRun
				
			SampleMixLoop2:

				// Left
				mov ax,[esi]
				cwde
				imul ebx
				sar eax,8
				add [edi],eax

				// Right
				mov ax,[esi+2]
				cwde
				imul ecx
				sar eax,8
				add [edi+4],eax

				add edi,8
				add esi,4

				dec ebp
				jnz SampleMixLoop2

				pop ebp
				pop edi
				pop esi
			}

			// Adjust pointers
			psSource=&psSource[2*iSamplesThisRun];
			piTarget=&piTarget[2*iSamplesThisRun];
			iSamplesLeft-=iSamplesThisRun;

			// Adjust levels
			iLevelLStart+=256*iLevelLIncrement;
			iLevelRStart+=256*iLevelRIncrement;
		}


/*
		DWORD 
			dwExtLeftOverflow=iLevelLStart&0xffff,
			dwExtRightOverflow=iLevelRStart&0xffff;

		// Ok, copy data
		while(iSamplesLeft>0)
		{
			// Left
			piTarget[0] = ((int)psSource[0] * dwLeft * dwExtLeft) >> 8;

			// Right
			piTarget[1] = ((int)psSource[1] * dwRight * dwExtRight) >> 8;		

			// Move on pointers
			psSource=&psSource[2];
			piTarget=&piTarget[2];

			// Update external level
			dwExtLeftOverflow+=iLevelLIncrement;
			if(dwExtLeftOverflow>>16 !=0)
			{
				dwExtLeft+=dwExtLeftOverflow>>16;
				dwExtLeftOverflow&=0xffff;
			}

			dwExtRightOverflow+=iLevelRIncrement;
			if(dwExtRightOverflow>>16 !=0)
			{
				dwExtRight+=dwExtRightOverflow>>16;
				dwExtRightOverflow&=0xffff;
			}

			iSamplesLeft--;
		}*/
	}
}


// Control for sample and type-specific data
void CSound::SetFile(CString strFilename,CString strFolder,
	BOOL bClearSettings)
{
	CFileSystem::NormaliseInternalFolder(strFolder);
	if(strFilename==m_strFilename && strFolder==m_strFolder) return;

	CSingleLock sl(&m_cs,TRUE);

	if(strFolder=="" && strFilename=="")
	{
		Clear();
		return;
	}

	if(CFileSystem::sm_pCurrent->IsInMemory(strFolder,strFilename))
	{
		int iLength=CFileSystem::sm_pCurrent->GetDataLength(strFolder,strFilename);
		short* psData=new short[iLength*2];
		CopyMemory(
			psData,
			CFileSystem::sm_pCurrent->GetData(strFolder,strFilename),
			iLength*2*sizeof(short));

		m_smpOrig.Set(psData,iLength);
	}
	else if(CFileSystem::sm_pCurrent->IsFile(strFolder,strFilename))
		m_smpOrig.Load(
			CFileSystem::sm_pCurrent->GetFile(strFolder,strFilename));
	else 
	{
		CString strError=
			"Could not find desired file: "+strFolder+strFilename;
		throw strError;
	}

	// Initialise settings

	m_bReverse=FALSE;

	// Envelope is always cleared
	m_bTrimFade=FALSE;
	m_iTrimStart=0;
	m_iTrimLength=m_smpOrig.Size();
	m_iFadeIn=0;
	m_iFadeOut=0;

	if(bClearSettings)
	{
		m_bFit=FALSE;
		m_iFitLength=16;
		m_iFitTrimStart=0;
		m_iFitTrimLength=16;

		m_bTune=FALSE;
		m_iTune=0;
	}

	m_strFilename=strFilename;
	m_strFolder=strFolder;

	ClearRendered();
}

// Resample from part of source to all of target, using the specified
// starting position and increment in the source sample, and wrap to
// indicate whether it wraps at ends (only important in VERY minor case)
void CSound::Resample(CSample& sSource,CSample& sTarget,
	double dOrigPos,double dOrigPlus,BOOL bWrap)
{
	// Loop through the modified sample, sampling its points...
	for(int i=0;i<sTarget.Size();i++)
	{
		int iPos=int(dOrigPos),iNextPos=iPos+1;
		if(iNextPos>=sSource.Size()) 
		{
			if(bWrap)
			{
				iNextPos-=sSource.Size();
				if(iPos>=sSource.Size())
					iPos-=sSource.Size();
			}
			else
			{
				iNextPos=sSource.Size()-1;
				if(iPos>=sSource.Size())
					iPos=sSource.Size()-1;
			}
		}

		double fAmount=dOrigPos-double(int(dOrigPos));

		sTarget.Data()[i*2]=short(
			double(sSource.Data()[iPos*2])*(1-fAmount)+
			double(sSource.Data()[iNextPos*2])*fAmount);
		sTarget.Data()[i*2+1]=short(
			double(sSource.Data()[iPos*2+1])*(1-fAmount)+
			double(sSource.Data()[iNextPos*2+1])*fAmount);

		dOrigPos+=dOrigPlus;
	}
}

void CSound::ApplyMods(int iIndex,BOOL fLockCS /*=TRUE*/)
{
	CSingleLock sl(&m_cs,fLockCS);

	CSample& smpMod=m_vsmpMod[iIndex];
	int iTempo=m_viSampleTempos[iIndex];

	// Handle fit-to-beat-length
	if(m_bFit)
	{
		ASSERT(iTempo>0 && m_iFitLength>0);

		// Work out target sizes
		int 
			iFullSize=m_iFitLength*44100*60/(iTempo*4),
			iTrimmedSize=m_iFitTrimLength*44100*60/(iTempo*4);

		// Create the modified sample at the right size
		smpMod.Create(iTrimmedSize);

		// Work out starting position and increment within original sample
		double 
			dOrigPos=double(m_smpOrig.Size())*double(m_iFitTrimStart)/double(m_iFitLength),
			dOrigPlus=double(m_smpOrig.Size()) / double(iFullSize);

		// Resample
		Resample(m_smpOrig,smpMod,dOrigPos,dOrigPlus,TRUE);
	}
	else 
	{
		// Handle trim/fade
		if(m_bTrimFade)
		{
			// Create the modified sample, cut to length
			ASSERT(m_iTrimStart+m_iTrimLength<=m_smpOrig.Size());
			smpMod.Create(m_iTrimLength);
			CopyMemory(smpMod.Data(),&(m_smpOrig.Data()[m_iTrimStart*2]),
				m_iTrimLength*2*sizeof(short));

			// Ensure fades in and out are safe
			if(m_iFadeIn>smpMod.Size()) m_iFadeIn=smpMod.Size();
			if(m_iFadeOut>smpMod.Size()) m_iFadeOut=smpMod.Size();

			// Apply the fade-in
			float fLevel=float(0.0),fLevelInc=float(1.0)/float(m_iFadeIn);
			for(int iPos=0;iPos<m_iFadeIn;iPos++)
			{
				smpMod.Data()[iPos*2]=
					short(float(smpMod.Data()[iPos*2])*fLevel);
				smpMod.Data()[iPos*2+1]=
					short(float(smpMod.Data()[iPos*2+1])*fLevel);

				fLevel+=fLevelInc;
			}

			// Apply the fade-out
			fLevel=float(1.0); fLevelInc=float(1.0)/float(m_iFadeOut);
			for(iPos=m_iTrimLength-m_iFadeOut;iPos<m_iTrimLength;iPos++)
			{
				smpMod.Data()[iPos*2]=
					short(float(smpMod.Data()[iPos*2])*fLevel);
				smpMod.Data()[iPos*2+1]=
					short(float(smpMod.Data()[iPos*2+1])*fLevel);

				fLevel-=fLevelInc;
			}
		}
		else // Clear mod sample
			smpMod.Create(0);

		if(m_bTune)
		{
			double dTune=(double)m_iTune;

			// Calculate wavelength ratio for tuning
			double dLengthRatio=440/exp(log(440)+(log(880)-log(440))*(dTune/(24*12)));

			CSample* psSrc=&m_vsmpMod[iIndex];
			if(psSrc->Size()==0) psSrc=&m_smpOrig;

			// Create spare sample for copy
			CSample smpTuned;
			smpTuned.Create((int)(dLengthRatio * psSrc->Size()));

			// Resample into spare
			Resample(*psSrc,smpTuned,0.0,1.0/dLengthRatio,FALSE);

			// Move this data from spare and delete spare
			smpMod.Set(smpTuned.Data(),smpTuned.Size());
			smpTuned.Detach();
		}
	}

}

void CSound::ApplyFX(int iIndex,BOOL fLockCS /*=TRUE*/)
{
	CSingleLock sl(&m_cs,fLockCS);

	CSample* psSrc=&m_vsmpMod[iIndex];
	if(psSrc->Size()==0) psSrc=&m_smpOrig;

	// Find longest effect-reported size
	int iMaxSize=psSrc->Size();
	for(int i=0;i<NUMEFFECTS;i++)
		if(m_pEffect[i])
	{
		int iSize=m_pEffect[i]->EffectedSize(iMaxSize);
		if(iSize>iMaxSize) iMaxSize=iSize;
	}

	// Modify size if too long, to the larger of orig. size *2 
	// and MAXEFFECTEDLENGTH seconds
	BOOL fFadeEnd=FALSE;
	if(iMaxSize>psSrc->Size()*2 && iMaxSize>MAXEFFECTEDLENGTH*44100)
	{
		fFadeEnd=TRUE;
		iMaxSize=psSrc->Size()*2;
		if(iMaxSize<MAXEFFECTEDLENGTH*44100) iMaxSize=MAXEFFECTEDLENGTH*44100;
	}

	// Create block for sound
	CBlock* pBlock=CBlock::Create(TRUE,iMaxSize);

	// Copy sound into block
	CopyMemory(pBlock->Data(),psSrc->Data(),psSrc->Size()*4);
	FillMemory(&pBlock->Data()[psSrc->Size()*2],
		(iMaxSize-psSrc->Size())*4,0);

	if(fLockCS)
		sl.Unlock();

	// Apply effects to block
	for(i=0;i<NUMEFFECTS;i++)
		if(m_pEffect[i])
	{
		m_pEffect[i]->Reset();
		m_pEffect[i]->ProcessBlock(pBlock);
	}

	// Fade block if necessary
	if(fFadeEnd)
	{
		DWORD dwTarget=(DWORD)(pBlock->Data());
		dwTarget+=(pBlock->Size()-4095)*4;
		_asm
		{
			push edi
			mov edi,dwTarget
			mov ebx,4095
			mov eax,0

		loopFadeFX:
			mov ax,[edi]	// Get data word (L)
			cwde			// Convert to dword
			imul ebx		// Multiply by level
			sar eax,12		// Divide by 4096
			mov [edi],ax	// Save again

			mov ax,[edi+2]	// Get data word (R)
			cwde			// Convert to dword
			imul ebx		// Multiply by level
			sar eax,12		// Divide by 4096
			mov [edi+2],ax	// Save again

			add edi,4
			dec ebx
			jnz loopFadeFX

			pop edi
		}

	}

	if(fLockCS)
		sl.Lock();

	// Copy block to "fx" version  and delete it
	CSample& smpFX=m_vsmpFX[iIndex];
	smpFX.Create(pBlock->Size());
	CopyMemory(smpFX.Data(),pBlock->Data(),pBlock->Size()*4);
	delete pBlock;
}

CString CSound::GetName() const
{
	CString strRet;
	
	if(m_strFilename=="") strRet="";
	else strRet=m_strFilename.Left(m_strFilename.GetLength()-4);

	return strRet;
}

void CSound::SetFit(int iFitLength,int iFitTrimStart,int iFitTrimLength)
{
	if(m_bFit && m_iFitLength==iFitLength && 
		m_iFitTrimStart==iFitTrimStart && m_iFitTrimLength==iFitTrimLength) return;

	CSingleLock sl(&m_cs,TRUE);

	m_bFit=TRUE;
	m_iFitLength=iFitLength;
	m_iFitTrimStart=iFitTrimStart;
	m_iFitTrimLength=iFitTrimLength;

	ClearRendered();
}

void CSound::ClearRendered()
{
	m_viSampleTempos.Clear();
	m_vsmpMod.Clear();
	m_vsmpFX.Clear();
}

// Discard any unnecessary tempos
void CSound::InformTempos(const vec<int> viTempos)
{
	if(m_bFit)
	{
		for(int i=m_viSampleTempos.Size()-1;i>=0;i--)
		{
			if(viTempos.Find(m_viSampleTempos[i])==-1)
			{
				m_viSampleTempos.RemoveAt(i);
				m_vsmpMod.RemoveAt(i);
				m_vsmpFX.RemoveAt(i);
			}
		}
	}
}

void CSound::ClearFit()
{
	if(!m_bFit) return;

	CSingleLock sl(&m_cs,TRUE);

	m_bFit=FALSE;

	ClearRendered();
}

void CSound::SetTuned(int iTune)
{
	if(m_bTune && m_iTune==iTune) return;

	CSingleLock sl(&m_cs,TRUE);

	m_bTune=TRUE;
	m_iTune=iTune;

	ClearRendered();
}

void CSound::ClearTuned()
{
	if(!m_bTune) return;

	CSingleLock sl(&m_cs,TRUE);

	m_bTune=FALSE;

	ClearRendered();
}

void CSound::SetTrimFade(int iTrimStart,int iTrimLength,int iFadeIn,int iFadeOut)
{
	if(m_bTrimFade && m_iFadeIn==iFadeIn && m_iFadeOut==iFadeOut &&
		m_iTrimStart==iTrimStart && m_iTrimLength==iTrimLength) return;

	CSingleLock sl(&m_cs,TRUE);

	m_bTrimFade=TRUE;
	m_iFadeIn=iFadeIn;
	m_iFadeOut=iFadeOut;
	m_iTrimStart=iTrimStart;
	m_iTrimLength=iTrimLength;

	ClearRendered();
}

void CSound::ClearTrimFade()
{
	if(!m_bTrimFade) return;

	CSingleLock sl(&m_cs,TRUE);

	m_bTrimFade=FALSE;

	ClearRendered();
}


// Control for effects
CEffect* CSound::Effect(int iEffect)
{
	ASSERT(iEffect>=0 && iEffect<NUMEFFECTS);

	return m_pEffect[iEffect];
}

void CSound::SetEffect(int iEffect,CRuntimeClass* pEffect,CEffect* pSource)
{
	ASSERT(iEffect>=0 && iEffect<NUMEFFECTS);

	CSingleLock sl(&m_cs,TRUE);

	if(m_pEffect[iEffect]) delete m_pEffect[iEffect];

	if(pEffect==NULL)
		m_pEffect[iEffect]=NULL;
	else
		m_pEffect[iEffect]=(CEffect*)pEffect->CreateObject();

	if(pSource!=NULL)
		m_pEffect[iEffect]->SetFrom(pSource);

	sl.Unlock();
	EffectChanged();
}

void CSound::EffectChanged()
{
	for(int iIndex=0;iIndex<m_viSampleTempos.Size();iIndex++)
	{
		ApplyFX(iIndex);
	}
}

void CSound::Serialize(CArchive& ar,CSong& s,BOOL bSetFile)
{
	CSingleLock sl(&m_cs,TRUE);

	int iVersion=3;

	if(ar.IsStoring())
	{
		ar << iVersion;

		ar << m_strFilename; 
		ar << m_strFolder;

		ar << m_bReverse;
		ar << m_bFit << m_iFitLength << m_iFitTrimStart << m_iFitTrimLength;
		ar << m_bTrimFade << m_iTrimStart << m_iTrimLength << m_iFadeIn << m_iFadeOut;
		ar << m_bTune << m_iTune;

		ar << m_iLevel << m_iAccent << m_iPan;

		ar << m_bMute;

		for(int iEffect=0;iEffect<NUMEFFECTS;iEffect++)
		{
			if(m_pEffect[iEffect]) 
			{
				ar << TRUE;
				ar << m_pEffect[iEffect];
			}
			else ar << FALSE;
		}
	}
	else
	{
		ar >> iVersion;
		if(iVersion < 1 || iVersion > 3)
			throw "File format not supported. Make sure this is a valid file and you are "
			  "using the latest version of leafDrums.";
		
		Clear();

		CString strFilename,strFolder;
		ar >> strFilename; 
		ar >> strFolder;

		if(bSetFile)
		{
			// Force actual set
			m_strFilename="";
			SetFile(strFilename,strFolder);
		}
		else
		{
			m_strFilename=strFilename;
			m_strFolder=strFolder;
		}

		BOOL bReverse=FALSE;
		if(iVersion>=3)
		{
			ar >> bReverse;
		}
		ar >> m_bFit >> m_iFitLength >> m_iFitTrimStart >> m_iFitTrimLength;
		ar >> m_bTrimFade >> m_iTrimStart >> m_iTrimLength >> m_iFadeIn >> m_iFadeOut;
		ar >> m_bTune >> m_iTune;

		ar >> m_iLevel >> m_iAccent >> m_iPan;

		if(iVersion>=2)
		{
			ar >> m_bMute;
		}
		else
			m_bMute=FALSE;

		for(int iEffect=0;iEffect<NUMEFFECTS;iEffect++)
		{
			BOOL fEffect;
			ar >> fEffect;
			if(fEffect) 
			{
				m_pEffect[iEffect]=new CEffect;
				ar >> m_pEffect[iEffect];
			}
			else m_pEffect[iEffect]=NULL;
		}

		if(bSetFile)
		{
			if(bReverse) SetReverse(TRUE);
			ClearRendered();
		}
		else
			m_bReverse=bReverse;
	}
}

void CSound::SetReverse(BOOL bReverse)
{
	if(m_bReverse==bReverse) return;

	CSingleLock sl(&m_cs,TRUE);

	m_bReverse=bReverse;

	if(m_smpOrig.Size()>0)
	{
		for(int iPos=0,iReversePos=m_smpOrig.Size()-1;iPos<m_smpOrig.Size()/2;iPos++,iReversePos--)
		{
			short 
				sLeft1=m_smpOrig.Data()[iPos*2],
				sRight1=m_smpOrig.Data()[iPos*2+1];

			m_smpOrig.Data()[iPos*2]=m_smpOrig.Data()[iReversePos*2];
			m_smpOrig.Data()[iPos*2+1]=m_smpOrig.Data()[iReversePos*2+1];

			m_smpOrig.Data()[iReversePos*2]=sLeft1;
			m_smpOrig.Data()[iReversePos*2+1]=sRight1;
		}

		ClearRendered();
	}
}
