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
#include "leafDrums.h"

#include "Identifiers.h"
#include "AudioHandler.h"

#include "GraphicAlpha.h"
#include "GraphicButton.h"
#include "GraphicHScroll.h"
 
#include "DSoundPlayDevice.h"

#include "FXcode\Distortion.h"
#include "FXcode\Quieten.h"
#include "FXcode\Surround.h"
#include "FXcode\Gate.h"
#include "FXcode\Compressor.h"
#include "FXcode\Expander.h"
#include "FXcode\Delay.h"
#include "FXcode\Chorus.h"
#include "FXcode\Flanger.h"
#include "FXcode\Phaser.h"
#include "FXcode\Tremolo.h"
#include "FXcode\Filter.h"
#include "FXcode\BandFilter.h"
#include "FXcode\EQ.h"

#include "DrumsHandler.h"
#include "SoundsHandler.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CAudioHandler::~CAudioHandler()
{
	delete m_pgBackground;
}

// Create all the controls for this section
void CAudioHandler::Create(CLeafDrumsView* pView,int iX,int iY)
{
	CSectionHandler::Create(pView,iX,iY);

	// Create and load background graphics
	CGraphic background(CLIENTWIDTH,491);
	background.LoadResource("DrumScreenBackground");
	m_pgBackground=new CGraphic(GetWidth(),GetHeight());
	// This line necessary because I didn't bother remaking the background
	// bitmaps when I made the screen 8 pixels taller
	m_pgBackground->FillRect(0,0,GetWidth(),GetHeight(),RGB(255,255,255));
	background.Copy(m_pgBackground,-167,0);

	// Load buttons and create graphics with those
	CGraphicAlpha gaButtons(770,458);
	gaButtons.LoadResource("DrumScreenButtons"); // Offset is 8,59
	CGraphicAlpha gaButtonsAlt(770,458);
	gaButtonsAlt.LoadResource("DrumScreenButtonsAlt");
	CGraphicAlpha gaButtonsOver(770,458);
	gaButtonsOver.LoadResource("DrumScreenButtonsOver");
	CGraphicAlpha gaButtonsPressed(770,458);
	gaButtonsPressed.LoadResource("DrumScreenButtonsPressed");
	CGraphicAlpha gaButtonsPressedAlt(770,458);
	gaButtonsPressedAlt.LoadResource("DrumScreenButtonsPressedAlt");

	CGraphic 
		gButtons(GetWidth(),GetHeight()),
		gButtonsAlt(GetWidth(),GetHeight()),
		gButtonsOver(GetWidth(),GetHeight()),
		gButtonsPressed(GetWidth(),GetHeight()),
		gButtonsPressedAlt(GetWidth(),GetHeight());

	m_pgBackground->Copy(&gButtons);
	gaButtons.Copy(&gButtons,8-167,59-39);
	m_pgBackground->Copy(&gButtonsAlt);
	gaButtonsAlt.Copy(&gButtonsAlt,8-167,59-39);
	m_pgBackground->Copy(&gButtonsOver);
	gaButtonsOver.Copy(&gButtonsOver,8-167,59-39);
	m_pgBackground->Copy(&gButtonsPressed);
	gaButtonsPressed.Copy(&gButtonsPressed,8-167,59-39);
	m_pgBackground->Copy(&gButtonsPressedAlt);
	gaButtonsPressedAlt.Copy(&gButtonsPressedAlt,8-167,59-39);

	// Create buttons
	CreateButton(IDC_AUDIO_TESTORIGINAL,65,20,36,12,
		gButtons,gButtonsOver,gButtonsPressed);
	CreateButton(IDC_AUDIO_TESTOUTPUT,569,446,36,12,
		gButtons,gButtonsOver,gButtonsPressed);
	CreateButton(IDC_AUDIO_PRESETOPEN,525,20,38,12,
		gButtons,gButtonsOver,gButtonsPressed);
	CreateButton(IDC_AUDIO_PRESETSAVE,567,20,38,12,
		gButtons,gButtonsOver,gButtonsPressed);
	CreateButton(IDC_AUDIO_SONG,561,466,50,12,
		gButtons,gButtonsOver,gButtonsPressed);

	// Create checkboxes
    CreateCheck(IDC_AUDIO_ENVELOPE,21,52,112,12,
		gButtonsAlt,gButtons,gButtonsPressedAlt,gButtonsPressed);
    CreateCheck(IDC_AUDIO_TUNING,21,188,57,12,
		gButtonsAlt,gButtons,gButtonsPressedAlt,gButtonsPressed);
    CreateCheck(IDC_AUDIO_STRETCH,319,52,103,12,
		gButtonsAlt,gButtons,gButtonsPressedAlt,gButtonsPressed);
    CreateCheck(IDC_AUDIO_REVERSE,117,20,63,12,
		gButtonsAlt,gButtons,gButtonsPressedAlt,gButtonsPressed);
    CreateCheck(IDC_AUDIO_FX1SNAP,229,279,74,12,
		gButtonsAlt,gButtons,gButtonsPressedAlt,gButtonsPressed);
    CreateCheck(IDC_AUDIO_FX2SNAP,527,279,74,12,
		gButtonsAlt,gButtons,gButtonsPressedAlt,gButtonsPressed);


	// Create scrollbars
	CreateHScroll(IDC_AUDIO_E_SKIP,41,75,197,0,1024,1,*m_pgBackground);
	CreateHScroll(IDC_AUDIO_E_LENGTH,41,102,197,0,1024,1,*m_pgBackground);
	CreateHScroll(IDC_AUDIO_E_ATTACK,41,129,197,0,1024,1,*m_pgBackground);
	CreateHScroll(IDC_AUDIO_E_DECAY,41,156,197,0,1024,1,*m_pgBackground);

	CreateHScroll(IDC_AUDIO_S_TOTAL,339,75,197,0,1024,1,*m_pgBackground);
	CreateHScroll(IDC_AUDIO_S_SKIP,339,102,197,0,1024,1,*m_pgBackground);
	CreateHScroll(IDC_AUDIO_S_LENGTH,339,129,197,0,1024,1,*m_pgBackground);

	CreateHScroll(IDC_AUDIO_ACCENTSCROLL,37,454,112,0,512,64,*m_pgBackground);

	for(int iEffectScroll=0;iEffectScroll<5;iEffectScroll++)
	{
		CreateHScroll(IDC_AUDIO_FX1SCROLL0+iEffectScroll,123,295+17*iEffectScroll,180,
			0,1024,1,*m_pgBackground);
		CreateHScroll(IDC_AUDIO_FX2SCROLL0+iEffectScroll,123+298,295+17*iEffectScroll,180,
			0,1024,1,*m_pgBackground);
	}

	// Create spin controls
	CGraphicTextOption* pgto=CreateTextOption(
		IDC_AUDIO_T_DIRECTION,41,211,69,
		*m_pgBackground);
	pgto->AddItem("Up");
	pgto->AddItem("Down");

	CreateNumberOption(
		IDC_AUDIO_T_OCTAVE,114,211,59,
		0,4,*m_pgBackground);
	CreateNumberOption(
		IDC_AUDIO_T_SEMITONE,177,211,59,
		0,11,*m_pgBackground);
	CreateNumberOption(
		IDC_AUDIO_T_FINETUNE,240,211,59,
		0,23,*m_pgBackground);

	for(int iEffectSpin=0;iEffectSpin<3;iEffectSpin++)
	{
		pgto=CreateTextOption(
			IDC_AUDIO_FX1SPIN0+iEffectSpin,123,295+17*(5+iEffectSpin),180,
			*m_pgBackground);
		pgto->AddItem("(fx param)");
		pgto=CreateTextOption(
			IDC_AUDIO_FX2SPIN0+iEffectSpin,123+298,295+17*(5+iEffectSpin),180,
			*m_pgBackground);
		pgto->AddItem("(fx param)");
	}

	// Initialise effects
	m_iNumEffects=12;
	ASSERT(m_iNumEffects<=MAXEFFECTS);

	int i=0;
	m_prcEffect[i++]=RUNTIME_CLASS(CDelay);
	m_prcEffect[i++]=RUNTIME_CLASS(CChorus);
	m_prcEffect[i++]=RUNTIME_CLASS(CFlanger);
	m_prcEffect[i++]=RUNTIME_CLASS(CPhaser);
	m_prcEffect[i++]=RUNTIME_CLASS(CTremolo);
	m_prcEffect[i++]=RUNTIME_CLASS(CDistortion);
//	m_prcEffect[i++]=RUNTIME_CLASS(CQuieten);
	m_prcEffect[i++]=RUNTIME_CLASS(CEQ);
	m_prcEffect[i++]=RUNTIME_CLASS(CFilter);
	m_prcEffect[i++]=RUNTIME_CLASS(CBandFilter);
//	m_prcEffect[i++]=RUNTIME_CLASS(CSurround);
	m_prcEffect[i++]=RUNTIME_CLASS(CGate);
	m_prcEffect[i++]=RUNTIME_CLASS(CCompressor);
	m_prcEffect[i++]=RUNTIME_CLASS(CExpander);

	ASSERT(i==m_iNumEffects);

	// Create and initialise effects spinners
	pgto=CreateTextOption(
		IDC_AUDIO_FX1TYPE,41,276,141,
		*m_pgBackground);
	FillEffectsSpinner(pgto);
	pgto=CreateTextOption(
		IDC_AUDIO_FX2TYPE,41+298,276,141,
		*m_pgBackground);
	FillEffectsSpinner(pgto);
}

void CAudioHandler::FillEffectsSpinner(CGraphicTextOption* pgto)
{
	// Add blank item
	pgto->AddItem("(No effect)");

	// Go through each effect, creating it and getting name then
	// deleting it
	for(int i=0;i<m_iNumEffects;i++)
	{
		CEffect* pEffect=(CEffect*)m_prcEffect[i]->CreateObject();
		ASSERT(pEffect);
		CString strTemp=pEffect->Name();
		delete pEffect;

		pgto->AddItem(strTemp);
	}
}

// Draw this section onto the specified graphic
void CAudioHandler::Draw(CGraphicOnscreen& goTarget)
{
	PaintBackground(goTarget,m_pgBackground);

	BOOL 
		bHideAll=(m_psSound->GetFile()==""),
		bEnvelope=m_psSound->IsTrimFade(),
		bStretch=m_psSound->IsFit(),
		bTuning=m_psSound->IsTuned();

	if(bHideAll)
	{
		goTarget.FillRect(4,20,470,32,RGB(238,231,221));
		goTarget.FillRect(4,32,601,441,RGB(238,231,221));
		return;
	}

	if(!bEnvelope)
		goTarget.FillRect(47,67,256,89,RGB(245,241,235));
	else
	{
		CString sText;

		goTarget.FillRect(240,75,63,15,RGB(251,249,247));
		sText.Format("%i",(m_psSound->GetTrimStart()*10)/441);
		m_gfFont.Draw(&goTarget,244,76,sText,RGB(0,0,0));

		goTarget.FillRect(240,75+27,63,15,RGB(251,249,247));
		sText.Format("%i",(m_psSound->GetTrimLength()*10)/441);
		m_gfFont.Draw(&goTarget,244,76+27,sText,RGB(0,0,0));

		goTarget.FillRect(240,75+27*2,63,15,RGB(251,249,247));
		sText.Format("%i",(m_psSound->GetFadeIn()*10)/441);
		m_gfFont.Draw(&goTarget,244,76+27*2,sText,RGB(0,0,0));

		goTarget.FillRect(240,75+27*3,63,15,RGB(251,249,247));
		sText.Format("%i",(m_psSound->GetFadeOut()*10)/441);
		m_gfFont.Draw(&goTarget,244,76+27*3,sText,RGB(0,0,0));
	}

	if(!bStretch)
		goTarget.FillRect(345,67,257,60,RGB(245,241,235));
	else
	{
		CString sText;

		goTarget.FillRect(538,75,64,15,RGB(251,249,247));
		sText.Format("%i.%02i",m_psSound->GetFitLength()/4,(m_psSound->GetFitLength()%4)*25);
		m_gfFont.Draw(&goTarget,542,76,sText,RGB(0,0,0));

		goTarget.FillRect(538,75+27,64,15,RGB(251,249,247));
		sText.Format("%i.%02i",m_psSound->GetFitTrimStart()/4,(m_psSound->GetFitTrimStart()%4)*25);
		m_gfFont.Draw(&goTarget,542,76+27,sText,RGB(0,0,0));

		goTarget.FillRect(538,75+27*2,64,15,RGB(251,249,247));
		sText.Format("%i.%02i",m_psSound->GetFitTrimLength()/4,(m_psSound->GetFitTrimLength()%4)*25);
		m_gfFont.Draw(&goTarget,542,76+27*2,sText,RGB(0,0,0));
	}

	if(!bTuning)
		goTarget.FillRect(47,203,224,6,RGB(245,241,235));

	// Draw effect param names
	for(int iEffect=0;iEffect<2;iEffect++)
	{
		for(int iScroll=0;iScroll<5;iScroll++)
			m_gfFont.Draw(&goTarget,45+298*iEffect,296+17*iScroll,
				m_strScrollParamName[iEffect][iScroll],RGB(0,0,0));
		for(int iSpin=0;iSpin<3;iSpin++)
			m_gfFont.Draw(&goTarget,45+298*iEffect,296+17*(iSpin+5),
				m_strSpinParamName[iEffect][iSpin],RGB(0,0,0));
	}
}

void CAudioHandler::OnHScroll(int iID,int iValue)
{
	switch(iID)
	{
	case IDC_AUDIO_ACCENTSCROLL:
		m_psSound->SetAccent(iValue);
		MarkChange();
		break;
	case IDC_AUDIO_E_SKIP:
		{
			int iNewStart=iValue;
			int iNewLength=m_psSound->GetTrimLength();
			if(iNewLength+iNewStart > m_psSound->GetOriginal().Size())
				iNewLength=m_psSound->GetOriginal().Size()-iNewStart;
			int iNewFadeIn=m_psSound->GetFadeIn();
			if(iNewFadeIn>iNewLength) iNewFadeIn=iNewLength;
			int iNewFadeOut=m_psSound->GetFadeOut();
			if(iNewFadeOut>iNewLength) iNewFadeOut=iNewLength;
			m_psSound->SetTrimFade(iNewStart,iNewLength,iNewFadeIn,iNewFadeOut);
			MarkChange();
			InitTrimFadeControls(IDC_AUDIO_E_SKIP);
		} break;
	case IDC_AUDIO_E_LENGTH:
		{
			int iNewStart=m_psSound->GetTrimStart();
			int iNewLength=iValue;
			int iNewFadeIn=m_psSound->GetFadeIn();
			if(iNewFadeIn>iNewLength) iNewFadeIn=iNewLength;
			int iNewFadeOut=m_psSound->GetFadeOut();
			if(iNewFadeOut>iNewLength) iNewFadeOut=iNewLength;
			m_psSound->SetTrimFade(iNewStart,iNewLength,iNewFadeIn,iNewFadeOut);
			MarkChange();
			InitTrimFadeControls(IDC_AUDIO_E_LENGTH);
		} break;
	case IDC_AUDIO_E_ATTACK :
		{
			int iNewStart=m_psSound->GetTrimStart();
			int iNewLength=m_psSound->GetTrimLength();
			int iNewFadeIn=iValue;
			int iNewFadeOut=m_psSound->GetFadeOut();
			m_psSound->SetTrimFade(iNewStart,iNewLength,iNewFadeIn,iNewFadeOut);
			MarkChange();
			InitTrimFadeControls(IDC_AUDIO_E_ATTACK);
		} break;
	case IDC_AUDIO_E_DECAY :
		{
			int iNewStart=m_psSound->GetTrimStart();
			int iNewLength=m_psSound->GetTrimLength();
			int iNewFadeIn=m_psSound->GetFadeIn();
			int iNewFadeOut=iValue;
			MarkChange();
			m_psSound->SetTrimFade(iNewStart,iNewLength,iNewFadeIn,iNewFadeOut);
			InitTrimFadeControls(IDC_AUDIO_E_DECAY);
		} break;
	case IDC_AUDIO_S_TOTAL :
		{
			int iNewTotal=iValue;
			int iNewStart=m_psSound->GetFitTrimStart();
			if(iNewStart>=iNewTotal) iNewStart=iNewTotal-1;
			int iNewLength=m_psSound->GetFitTrimLength();
			if(iNewStart+iNewLength>iNewTotal)
				iNewLength=iNewTotal-iNewStart;
			m_psSound->SetFit(iNewTotal,iNewStart,iNewLength);
			MarkChange();
			InitFitControls(IDC_AUDIO_S_TOTAL);
		} break;
	case IDC_AUDIO_S_SKIP :
		{
			int iNewTotal=m_psSound->GetFitLength();
			int iNewStart=iValue;
			int iNewLength=m_psSound->GetFitTrimLength();
			if(iNewStart+iNewLength>iNewTotal)
				iNewLength=iNewTotal-iNewStart;
			m_psSound->SetFit(iNewTotal,iNewStart,iNewLength);
			MarkChange();
			InitFitControls(IDC_AUDIO_S_SKIP);
		} break;
	case IDC_AUDIO_S_LENGTH :
		{
			int iNewTotal=m_psSound->GetFitLength();
			int iNewStart=m_psSound->GetFitTrimStart();
			int iNewLength=iValue;
			m_psSound->SetFit(iNewTotal,iNewStart,iNewLength);
			MarkChange();
			InitFitControls(IDC_AUDIO_S_LENGTH);
		} break;
	default:
		if(iID>=IDC_AUDIO_FX1SCROLL0 && iID<IDC_AUDIO_FX1SCROLL0+5)
			NewEffectValue(0,m_iScrollParamNumber[0][iID-IDC_AUDIO_FX1SCROLL0],iValue);
		else if(iID>=IDC_AUDIO_FX2SCROLL0 && iID<IDC_AUDIO_FX2SCROLL0+5)
			NewEffectValue(1,m_iScrollParamNumber[1][iID-IDC_AUDIO_FX2SCROLL0],iValue);
		break;
	}
}

void CAudioHandler::OnSelchange(int iID,int iValue)
{
	switch(iID)
	{
	case IDC_AUDIO_T_DIRECTION :
	case IDC_AUDIO_T_OCTAVE :
	case IDC_AUDIO_T_SEMITONE :
	case IDC_AUDIO_T_FINETUNE :
		{
			int iTuning=
				(GetNumberOption(IDC_AUDIO_T_FINETUNE)->GetSel() +
				24*GetNumberOption(IDC_AUDIO_T_SEMITONE)->GetSel() +
				24*12*GetNumberOption(IDC_AUDIO_T_OCTAVE)->GetSel()) *
				((GetTextOption(IDC_AUDIO_T_DIRECTION)->GetSel() == 0) ? 1 : -1);
			MarkChange();
			m_psSound->SetTuned(iTuning);
		} break;
	case IDC_AUDIO_FX1TYPE:
	case IDC_AUDIO_FX2TYPE:
		{
			CRuntimeClass* pEffect= (iValue==0) ? NULL : (m_prcEffect[iValue-1]);
			m_psSound->SetEffect(iID-IDC_AUDIO_FX1TYPE,pEffect);
			MarkChange();
			InitEffectControls(iID-IDC_AUDIO_FX1TYPE);
		} break;
	default:
		if(iID>=IDC_AUDIO_FX1SPIN0 && iID<IDC_AUDIO_FX1SPIN0+3)
			NewEffectValue(0,m_iSpinParamNumber[0][iID-IDC_AUDIO_FX1SPIN0],iValue);
		else if(iID>=IDC_AUDIO_FX2SPIN0 && iID<IDC_AUDIO_FX2SPIN0+3)
			NewEffectValue(1,m_iSpinParamNumber[1][iID-IDC_AUDIO_FX2SPIN0],iValue);
		break;
	}
}

void CAudioHandler::NewEffectValue(int iEffect,int iParam,int iValue)
{
	// Get effect
	CEffect* pEffect=m_psSound->Effect(iEffect);

	// Set param
	pEffect->SetParam(iParam,iValue);

	// Recalculate sound
	m_psSound->EffectChanged();
	MarkChange();
}


void CAudioHandler::OnButton(int iID)
{
	switch(iID)
	{
	case IDC_AUDIO_SONG:
		GetView()->ShowMainScreen();
		// Also update drum names
		((CDrumsHandler*)
			(GetView()->GetSection(typeid(CDrumsHandler))))->
			Update();
		break;
	case IDC_AUDIO_TESTORIGINAL:
		Play(m_psSound->GetOriginal());
		break;
	case IDC_AUDIO_TESTOUTPUT:
		Play(m_psSound->GetFinal(GetView()->GetDocument()->GetSong()->GetStartTempo()));
		break;

	case IDC_AUDIO_REVERSE:
		m_psSound->SetReverse(!m_psSound->GetReverse());
		MarkChange();
		break;

	case IDC_AUDIO_ENVELOPE:
		if(GetCheck(IDC_AUDIO_ENVELOPE))
		{
			// Clear stretch if it's on
			if(GetCheck(IDC_AUDIO_STRETCH))
			{
				SetCheck(IDC_AUDIO_STRETCH,FALSE);
				OnButton(IDC_AUDIO_STRETCH);
			}

			m_psSound->SetTrimFade(
				0,m_psSound->GetOriginal().Size(),0,0);
			MarkChange();
			InitTrimFadeControls();
			UpdateItemVisibility();
			Repaint(47,67,256,106);
		}
		else
		{
			m_psSound->ClearTrimFade();
			MarkChange();
			UpdateItemVisibility();
			Repaint(47,67,256,106);
		}
		break;
	case IDC_AUDIO_STRETCH:
		if(GetCheck(IDC_AUDIO_STRETCH))
		{
			// Clear tuning and envelope if they're on
			if(GetCheck(IDC_AUDIO_ENVELOPE))
			{
				SetCheck(IDC_AUDIO_ENVELOPE,FALSE);
				OnButton(IDC_AUDIO_ENVELOPE);
			}
			if(GetCheck(IDC_AUDIO_TUNING))
			{
				SetCheck(IDC_AUDIO_TUNING,FALSE);
				OnButton(IDC_AUDIO_TUNING);
			}

			m_psSound->SetFit(16,0,16);
			MarkChange();
			InitFitControls();
			UpdateItemVisibility();
			Repaint(345,67,257,77);
		}
		else
		{
			m_psSound->ClearFit();
			MarkChange();
			UpdateItemVisibility();
			Repaint(345,67,257,77);
		}
		break;
	case IDC_AUDIO_TUNING:
		if(GetCheck(IDC_AUDIO_TUNING))
		{
			// Clear stretch if it's on
			if(GetCheck(IDC_AUDIO_STRETCH))
			{
				SetCheck(IDC_AUDIO_STRETCH,FALSE);
				OnButton(IDC_AUDIO_STRETCH);
			}

			m_psSound->SetTuned(0);
			MarkChange();
			InitTunedControls();
			UpdateItemVisibility();
			Repaint(47,203,224,6);
		}
		else
		{
			m_psSound->ClearTuned();
			MarkChange();
			UpdateItemVisibility();
			Repaint(47,203,224,6);
		}
		break;

	case IDC_AUDIO_FX1SNAP:
		InitEffectControls(0);
		break;		
	case IDC_AUDIO_FX2SNAP:
		InitEffectControls(1);
		break;		

	case IDC_AUDIO_PRESETOPEN:
		OpenPreset();
		break;

	case IDC_AUDIO_PRESETSAVE:
		SavePreset();
		break;
	}
}

UINT SinglePlayThread(LPVOID lpvParam)
{
	try
	{
		// Create sound device
		CDSoundPlayDevice sd;

		// Get block details
		CBlock* pBlock=(CBlock*)lpvParam;
		int iLength=pBlock->Size();

		// Play block in 32KB (8096 sample) increments
		for(int iPos=0;iPos<iLength;iPos+=8096)
		{
			// Copy to smaller block
			int iSize=iLength-iPos;
			if(iSize>8096) iSize=8096;
			CBlock* pSmallBlock=CBlock::Create(TRUE,iSize);
			CopyMemory(pSmallBlock->Data(),&(pBlock->Data()[iPos*2]),iSize*4);

			// Add buffer
			sd.PlayBuffer(pSmallBlock);

			// Wait until < 250 ms
			while(TRUE)
			{
				DWORD dwNow=GetTickCount();
				DWORD dwEnd=sd.GetPlayEndTime();

				if(dwEnd<dwNow ||
					dwEnd-dwNow <= 1000) break;

				Sleep(dwEnd-dwNow-1000);
			}
		}

		// Wait before killing sound driver
		sd.StopAfterBuffers();

#ifdef _DEBUG
		CString sTemp;
		sTemp.Format("Ended temporary play thread: 0x%x\n",GetCurrentThreadId());
		TRACE(sTemp);
#endif
	}
	catch(CString strError)
	{
		MessageBox(AfxGetMainWnd()->GetSafeHwnd(),
			"Failed to play sound.\r\n\r\n"
			"This is probably because you have a sound card that "
			"can only play one sound at a time (it depends on the "
			"drivers), and you are already playing the drum "
			"rhythm or another sound test.\r\n\r\n"
			"To avoid seeing this message in future, stop the drum rhythm before "
			"you test sounds, and wait for one sound test to finish before you "
			"attempt another.",
			"Sound play failed",
			MB_OK|MB_ICONINFORMATION);
	}

	return 0;
}

void CAudioHandler::Play(CSample& smp)
{
	// Create a block
	CBlock* pBlock=CBlock::Create(TRUE,smp.Size());

	// Copy sample data into block
	CopyMemory(pBlock->Data(),smp.Data(),smp.Size()*4);

	CWinThread* pPlayThread=AfxBeginThread(&SinglePlayThread,pBlock);
#ifdef _DEBUG
	CString sTemp;
	sTemp.Format("Started temporary play thread: 0x%x\n",pPlayThread->m_nThreadID);
	TRACE(sTemp);
#endif
}

void CAudioHandler::SetSound(CSound* psSound)
{
	m_psSound=psSound;

	GetHScroll(IDC_AUDIO_ACCENTSCROLL)->SetPos(m_psSound->GetAccent());

	SetCheck(IDC_AUDIO_ENVELOPE,m_psSound->IsTrimFade());
	SetCheck(IDC_AUDIO_TUNING,m_psSound->IsTuned());
	SetCheck(IDC_AUDIO_STRETCH,m_psSound->IsFit());
	SetCheck(IDC_AUDIO_REVERSE,m_psSound->GetReverse());

	Repaint();
	UpdateItemVisibility();
	if(m_psSound->IsTrimFade())
		InitTrimFadeControls();
	if(m_psSound->IsTuned())
		InitTunedControls();
	if(m_psSound->IsFit())
		InitFitControls();
	InitEffectControls();
}

void CAudioHandler::UpdateItemVisibility()
{
	BOOL 
		bHideAll=(m_psSound->GetFile()==""),
		bEnvelope=m_psSound->IsTrimFade(),
		bStretch=m_psSound->IsFit(),
		bTuning=m_psSound->IsTuned();

	ShowControl(IDC_AUDIO_TESTORIGINAL,!(bHideAll));
	ShowControl(IDC_AUDIO_TESTOUTPUT,!(bHideAll));
	ShowControl(IDC_AUDIO_PRESETOPEN,TRUE);
	ShowControl(IDC_AUDIO_PRESETSAVE,TRUE);

	ShowControl(IDC_AUDIO_REVERSE,!(bHideAll));
	ShowControl(IDC_AUDIO_ENVELOPE,!(bHideAll));
	ShowControl(IDC_AUDIO_STRETCH,!(bHideAll));
	ShowControl(IDC_AUDIO_TUNING,!(bHideAll));

	ShowControl(IDC_AUDIO_E_SKIP,!((bHideAll|!bEnvelope)));
	ShowControl(IDC_AUDIO_E_LENGTH,!((bHideAll|!bEnvelope)));
	ShowControl(IDC_AUDIO_E_ATTACK,!((bHideAll|!bEnvelope)));
	ShowControl(IDC_AUDIO_E_DECAY,!((bHideAll|!bEnvelope)));

	ShowControl(IDC_AUDIO_S_TOTAL,!((bHideAll|!bStretch)));
	ShowControl(IDC_AUDIO_S_SKIP,!((bHideAll|!bStretch)));
	ShowControl(IDC_AUDIO_S_LENGTH,!((bHideAll|!bStretch)));

	ShowControl(IDC_AUDIO_T_DIRECTION,!((bHideAll|!bTuning)));
	ShowControl(IDC_AUDIO_T_OCTAVE,!((bHideAll|!bTuning)));
	ShowControl(IDC_AUDIO_T_SEMITONE,!((bHideAll|!bTuning)));
	ShowControl(IDC_AUDIO_T_FINETUNE,!((bHideAll|!bTuning)));

	ShowControl(IDC_AUDIO_ACCENTSCROLL,!(bHideAll));

	ShowControl(IDC_AUDIO_FX1TYPE,!(bHideAll));
	ShowControl(IDC_AUDIO_FX2TYPE,!(bHideAll));
	ShowControl(IDC_AUDIO_FX1SNAP,!(bHideAll));
	ShowControl(IDC_AUDIO_FX2SNAP,!(bHideAll));
}

void CAudioHandler::InitTrimFadeControls(int iExcept)
{
	int iLength=m_psSound->GetOriginal().Size();

	if(iExcept!=IDC_AUDIO_E_SKIP)
	{
		GetHScroll(IDC_AUDIO_E_SKIP)->SetRange(0,iLength,441);
		GetHScroll(IDC_AUDIO_E_SKIP)->SetStep(44);
		GetHScroll(IDC_AUDIO_E_SKIP)->SetPos(m_psSound->GetTrimStart());
	}

	if(iExcept!=IDC_AUDIO_E_LENGTH)
	{
		GetHScroll(IDC_AUDIO_E_LENGTH)->SetRange(0,iLength-m_psSound->GetTrimStart(),441);
		GetHScroll(IDC_AUDIO_E_LENGTH)->SetStep(44);
		GetHScroll(IDC_AUDIO_E_LENGTH)->SetPos(m_psSound->GetTrimLength());
	}

	if(iExcept!=IDC_AUDIO_E_ATTACK)
	{
		GetHScroll(IDC_AUDIO_E_ATTACK)->SetRange(0,m_psSound->GetTrimLength(),441);
		GetHScroll(IDC_AUDIO_E_ATTACK)->SetStep(44);
		GetHScroll(IDC_AUDIO_E_ATTACK)->SetPos(m_psSound->GetFadeIn());
	}

	if(iExcept!=IDC_AUDIO_E_DECAY)
	{
		GetHScroll(IDC_AUDIO_E_DECAY)->SetRange(0,m_psSound->GetTrimLength(),441);
		GetHScroll(IDC_AUDIO_E_DECAY)->SetStep(44);
		GetHScroll(IDC_AUDIO_E_DECAY)->SetPos(m_psSound->GetFadeOut());
	}

	Repaint(240,75,63,15+27*3);
}

void CAudioHandler::InitTunedControls()
{
	int iTuning=m_psSound->GetTuning();

	GetTextOption(IDC_AUDIO_T_DIRECTION)->SetSel(iTuning>=0 ? 0 : 1);
	if(iTuning<0) iTuning=-iTuning;

	GetNumberOption(IDC_AUDIO_T_OCTAVE)->SetSel(iTuning/(24*12));
	iTuning%=(24*12);

	GetNumberOption(IDC_AUDIO_T_SEMITONE)->SetSel(iTuning/24);
	iTuning%=24;

	GetNumberOption(IDC_AUDIO_T_FINETUNE)->SetSel(iTuning);
}

void CAudioHandler::InitFitControls(int iExcept)
{
	if(iExcept!=IDC_AUDIO_S_TOTAL)
	{
		GetHScroll(IDC_AUDIO_S_TOTAL)->SetRange(1,64,8);
		GetHScroll(IDC_AUDIO_S_TOTAL)->SetPos(m_psSound->GetFitLength());
	}

	if(iExcept!=IDC_AUDIO_S_SKIP)
	{
		GetHScroll(IDC_AUDIO_S_SKIP)->SetRange(0,m_psSound->GetFitLength()-1,8);
		GetHScroll(IDC_AUDIO_S_SKIP)->SetPos(m_psSound->GetFitTrimStart());
	}

	if(iExcept!=IDC_AUDIO_S_LENGTH)
	{
		GetHScroll(IDC_AUDIO_S_LENGTH)->SetRange(1,
			m_psSound->GetFitLength()-m_psSound->GetFitTrimStart(),8);
		GetHScroll(IDC_AUDIO_S_LENGTH)->SetPos(m_psSound->GetFitTrimLength());
	}

	Repaint(538,75,64,69);
}

void CAudioHandler::InitEffectControls(int iEffect)
{
	// Handle both effects
	if(iEffect==-1) 
	{
		InitEffectControls(0);
		InitEffectControls(1);
		return;
	}

	// Hide all controls to begin with and clear all names
	for(int iScroll=0;iScroll<5;iScroll++)
	{
		ShowControl(IDC_AUDIO_FX1SCROLL0+iScroll+100*iEffect,FALSE);
		m_strScrollParamName[iEffect][iScroll]="";
	}
	for(int iSpin=0;iSpin<3;iSpin++)
	{
		ShowControl(IDC_AUDIO_FX1SPIN0+iSpin+100*iEffect,FALSE);
		m_strSpinParamName[iEffect][iSpin]="";
	}

	// Mark param-name area for repainting
	Repaint(41+298*iEffect,295,82,134);

	// Ok, single effect. Get effect
	CEffect* pEffect=m_psSound->Effect(iEffect);

	// If it's null...
	if(pEffect==NULL)
	{
		// Set spin
		GetTextOption(IDC_AUDIO_FX1TYPE+iEffect)->
			SetSel(0);

		// Finished
		return;
	}

	// Find effect and set spin control
	for(int iEffectNum=0;iEffectNum<m_iNumEffects;iEffectNum++)
	{
		if(pEffect->GetRuntimeClass()==m_prcEffect[iEffectNum])
		{
			GetTextOption(IDC_AUDIO_FX1TYPE+iEffect)->
				SetSel(iEffectNum+1);
			break;
		}
	}

	int iParam=-1;
	for(iScroll=0;iScroll<5;iScroll++)
	{
		// Check parameter is ok for scrollbar
		do
		{
			iParam++;
			if(iParam>=pEffect->Parameters()) break;
		}
		while(pEffect->ValueNames(iParam));

		if(iParam>=pEffect->Parameters()) break;

		// Get scrollbar
		CGraphicHScroll* pghs=GetHScroll(
			IDC_AUDIO_FX1SCROLL0+iScroll+100*iEffect);

		// Work out scrollbar range and set it
		int iMin,iMax;
		pEffect->ParamLimits(iParam,iMin,iMax);
		pghs->SetRange(iMin,iMax,(iMax-iMin)/10);

		// Set snap-to if checkbox is on
		if(GetCheck(IDC_AUDIO_FX1SNAP+iEffect)
			&& (pEffect->ParamName(iParam).Find("elay")!=-1))
		{
			pghs->SetSnap(((60*44100)/4) / GetSong()->GetStartTempo());
		}
		else
		{
				pghs->SetSnap();
		}

		// Work out scrollbar pos and set it
		pghs->SetPos(pEffect->GetParam(iParam));

		// Show scrollbar
		ShowControl(IDC_AUDIO_FX1SCROLL0+iScroll+100*iEffect,TRUE);

		// Set parameter text
		m_strScrollParamName[iEffect][iScroll]=
			pEffect->ParamName(iParam);
		m_iScrollParamNumber[iEffect][iScroll]=
			iParam;
	}

	iParam=-1;
	for(iSpin=0;iSpin<3;iSpin++)
	{
		// Check parameter is ok for spin control
		do
		{
			iParam++;
			if(iParam>=pEffect->Parameters()) break;
		}
		while(!pEffect->ValueNames(iParam));

		if(iParam>=pEffect->Parameters()) break;

		// Find out how many parameters
		int iMin,iMax;
		pEffect->ParamLimits(iParam,iMin,iMax);

		// Get spin
		CGraphicTextOption* pgto=GetTextOption(
			IDC_AUDIO_FX1SPIN0+iSpin+100*iEffect);

		// Clear it, add all the parameter names
		pgto->Clear();
		for(int iValue=iMin;iValue<=iMax;iValue++)
			pgto->AddItem(pEffect->ValueName(iParam,iValue));

		// Set the position
		pgto->SetSel(pEffect->GetParam(iParam)-iMin);

		// Show spin
		ShowControl(IDC_AUDIO_FX1SPIN0+iSpin+100*iEffect,TRUE);

		// Set parameter text
		m_strSpinParamName[iEffect][iSpin]=
			pEffect->ParamName(iParam);
		m_iSpinParamNumber[iEffect][iSpin]=
			iParam;
	}
}

BOOL g_bOpenSound,g_bOpenEnvelope,g_bOpenTuning,g_bOpenStretch,g_bOpenEffects,g_bOpenAccentPan;

UINT APIENTRY CustomOpenHookProc(HWND hdlg,UINT uiMsg,WPARAM wParam,LPARAM lParam)
{
	switch(uiMsg)
	{
	case WM_INITDIALOG:
		CheckDlgButton(hdlg,IDC_SOUND,BST_CHECKED);
		CheckDlgButton(hdlg,IDC_ENVELOPE,BST_CHECKED);
		CheckDlgButton(hdlg,IDC_TUNING,BST_CHECKED);
		CheckDlgButton(hdlg,IDC_STRETCH,BST_CHECKED);
		CheckDlgButton(hdlg,IDC_EFFECTS,BST_CHECKED);
		CheckDlgButton(hdlg,IDC_ACCENTPAN,BST_CHECKED);

		g_bOpenSound=g_bOpenEnvelope=g_bOpenTuning=g_bOpenStretch=g_bOpenEffects=g_bOpenAccentPan=TRUE;
		break;

	case WM_COMMAND:
		if(HIWORD(wParam)==BN_CLICKED)
		{
			if(LOWORD(wParam)==IDC_SOUND)
			{
				if((::IsDlgButtonChecked(hdlg,IDC_SOUND) & BST_CHECKED)==BST_CHECKED)
					EnableWindow(GetDlgItem(hdlg,IDC_ENVELOPE),TRUE);
				else
				{
					EnableWindow(GetDlgItem(hdlg,IDC_ENVELOPE),FALSE);
					CheckDlgButton(hdlg,IDC_ENVELOPE,BST_UNCHECKED);
				}
			}

			g_bOpenSound=
				(::IsDlgButtonChecked(hdlg,IDC_SOUND) & BST_CHECKED)==BST_CHECKED;
			g_bOpenEnvelope=
				(::IsDlgButtonChecked(hdlg,IDC_ENVELOPE) & BST_CHECKED)==BST_CHECKED;
			g_bOpenTuning=
				(::IsDlgButtonChecked(hdlg,IDC_TUNING) & BST_CHECKED)==BST_CHECKED;
			g_bOpenStretch=
				(::IsDlgButtonChecked(hdlg,IDC_STRETCH) & BST_CHECKED)==BST_CHECKED;
			g_bOpenEffects=
				(::IsDlgButtonChecked(hdlg,IDC_EFFECTS) & BST_CHECKED)==BST_CHECKED;
			g_bOpenAccentPan=
				(::IsDlgButtonChecked(hdlg,IDC_ACCENTPAN) & BST_CHECKED)==BST_CHECKED;
		}
		break;
	}
	return 0;
}

void CAudioHandler::OpenPreset()
{
	OPENFILENAME ofn;
	ZeroMemory(&ofn,sizeof(ofn));
	
	ofn.lStructSize=sizeof(ofn);
	ofn.hwndOwner=AfxGetMainWnd()->GetSafeHwnd();
	ofn.hInstance=AfxGetResourceHandle();
	ofn.lpstrFilter="leafDrums 2 presets (*.leafDrums2Preset)\0*.leafDrums2Preset\0";
	char acFilename[1024];
	lstrcpy(acFilename,"");
	ofn.lpstrFile=acFilename;
	ofn.nMaxFile=sizeof(acFilename);
	ofn.Flags=OFN_HIDEREADONLY|
		OFN_ENABLETEMPLATE|OFN_EXPLORER|OFN_ENABLEHOOK;
	ofn.lpstrDefExt="leafDrums2Preset";
	ofn.lpTemplateName=MAKEINTRESOURCE(IDD_CUSTOMPRESETOPEN);
	ofn.lpfnHook=CustomOpenHookProc;

	if(::GetOpenFileName(&ofn))
	{
		CString strFilename=ofn.lpstrFile;
		if(strFilename.Right(4)==".lea") strFilename+="fDrums2Preset";

		// ok, we have a filename... 
		try
		{
			CFile f(strFilename,CFile::modeRead|CFile::shareDenyWrite);
			CArchive a(&f,CArchive::load);

			// Load a new CSound with the details
			CSound s;
			s.Serialize(a,*GetSong(),FALSE);

			// Copy the desired details into the current sound
			if(g_bOpenSound)
				m_psSound->SetFile(s.GetFile(),s.GetFolder());

			if(m_psSound->GetFile()!="")
			{
				if(g_bOpenEnvelope)
				{
					if(s.IsTrimFade())
					{
						m_psSound->ClearFit();
						m_psSound->SetTrimFade(s.GetTrimStart(),s.GetTrimLength(),
							s.GetFadeIn(),s.GetFadeOut());
					}
					else
						m_psSound->ClearTrimFade();
				}

				if(g_bOpenTuning)
				{
					if(s.IsTuned())
					{
						m_psSound->ClearFit();
						m_psSound->SetTuned(s.GetTuning());
					}
					else
						m_psSound->ClearTuned();
				}

				if(g_bOpenStretch)
				{
					if(s.IsFit())
					{
						m_psSound->ClearTuned();
						m_psSound->ClearTrimFade();					
						m_psSound->SetFit(s.GetFitLength(),
							s.GetFitTrimStart(),s.GetFitTrimLength());
					}
					else
						m_psSound->ClearFit();
				}

				if(g_bOpenAccentPan)
				{
					m_psSound->SetAccent(s.GetAccent());
					m_psSound->SetPan(s.GetPan());
				}

				if(g_bOpenEffects)
				{
					for(int iEffect=0;iEffect<2;iEffect++)
					{
						if(s.Effect(iEffect)!=NULL)
							m_psSound->SetEffect(iEffect,
								s.Effect(iEffect)->GetRuntimeClass(),
								s.Effect(iEffect));
						else
							m_psSound->SetEffect(iEffect,NULL);
					}
				}
			}
		}
		catch(CException* pe)
		{
			pe->Delete();
			GetView()->MessageBox(
				"There was an error opening the preset.\r\n\r\n"
				"Check that the file is correct.",
				"Preset open error",
				MB_OK|MB_ICONEXCLAMATION);
		}
		catch(char* pstrError)
		{
			CString sError;
			sError.Format(
				"There was an error opening the preset.\r\n\r\n%s",pstrError);
			GetView()->MessageBox(
				sError,
				"Preset open error",
				MB_OK|MB_ICONEXCLAMATION);
		}
		catch(CString strErrorInt)
		{
			CString sError;
			sError.Format(
				"There was an error opening the preset.\r\n\r\n%s",strErrorInt);
			GetView()->MessageBox(
				sError,
				"Preset open error",
				MB_OK|MB_ICONEXCLAMATION);
		}

		// Update sounds display
		((CSoundsHandler*)
			(GetView()->GetSection(typeid(CSoundsHandler))))->
			ResetCurrentSound();
	}
}

void CAudioHandler::SavePreset()
{
	CFileDialog fd(FALSE,".leafDrums2Preset",NULL,
		OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT|OFN_NOREADONLYRETURN,
		"leafDrums 2 presets (*.leafDrums2Preset)|*.leafDrums2Preset||",
		GetView());
	if(fd.DoModal()==IDOK)
	{
		CString strFilename=fd.GetPathName();
		if(strFilename.Right(4)==".lea") strFilename+="fDrums2Preset";

		// Save preset 
		try
		{
			CFile f(strFilename,CFile::modeCreate|CFile::modeWrite|CFile::shareExclusive);
			CArchive a(&f,CArchive::store);

			m_psSound->Serialize(a,*GetSong());
		}
		catch(CException* pe)
		{
			pe->Delete();
			GetView()->MessageBox(
				"There was an error saving the preset.\r\n\r\n"
				"Make sure you are saving to a disc drive on which you can write files, "
				"and make sure there is sufficient room to save the preset. (Presets are "
				"rather small, so this should be okay, but...)",
				"Preset save error",
				MB_OK|MB_ICONEXCLAMATION);
		}
	}
}

