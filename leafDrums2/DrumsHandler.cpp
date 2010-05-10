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
#include "LeafDrums.h"

#include "Identifiers.h"
#include "DrumsHandler.h"

#include "GraphicAlpha.h"
#include "GraphicButton.h"
#include "GraphicHScroll.h"

#include "PatternHandler.h"
#include "SoundsHandler.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define BUTTONX 8
#define BUTTONY 31
#define BUTTONWIDTH 93
#define BUTTONHEIGHT 15

CDrumsHandler::~CDrumsHandler()
{
	delete m_pgBackground;

	delete m_pgButton;
	delete m_pgButtonOver;
	delete m_pgButtonPressed;

	delete m_pgButtonsAltBit;
	delete m_pgButtonsBit;
	delete m_pgButtonsPressedAltBit;
	delete m_pgButtonsPressedBit;

	delete m_pgfFont;
}

// Create all the controls for this section
void CDrumsHandler::Create(CLeafDrumsView* pView,int iX,int iY)
{
	CSectionHandler::Create(pView,iX,iY);

	// Create and load background graphics
	CGraphic background(CLIENTWIDTH,OTHERHEIGHT);
	background.LoadResource("SongScreenBackground");
	m_pgBackground=new CGraphic(GetWidth(),GetHeight());
	background.Copy(m_pgBackground,0,-SONGHANDLER_HEIGHT);

	// Load song buttons and create graphics with those
	CGraphicAlpha gaButtons(BMPDETAILS_SONGSCREEN_BUTTONSWIDTH,BMPDETAILS_SONGSCREEN_BUTTONSHEIGHT);
	gaButtons.LoadResource("SongScreenButtons"); // at 8,7
	CGraphicAlpha gaButtonsAlt(BMPDETAILS_SONGSCREEN_BUTTONSWIDTH,BMPDETAILS_SONGSCREEN_BUTTONSHEIGHT);
	gaButtonsAlt.LoadResource("SongScreenButtonsAlt");
	CGraphicAlpha gaButtonsOver(BMPDETAILS_SONGSCREEN_BUTTONSWIDTH,BMPDETAILS_SONGSCREEN_BUTTONSHEIGHT2);
	gaButtonsOver.LoadResource("SongScreenButtonsOver");
	CGraphicAlpha gaButtonsPressed(BMPDETAILS_SONGSCREEN_BUTTONSWIDTH,BMPDETAILS_SONGSCREEN_BUTTONSHEIGHT2);
	gaButtonsPressed.LoadResource("SongScreenButtonsPressed");
	CGraphicAlpha gaButtonsPressedAlt(BMPDETAILS_SONGSCREEN_BUTTONSWIDTH,BMPDETAILS_SONGSCREEN_BUTTONSHEIGHT2);
	gaButtonsPressedAlt.LoadResource("SongScreenButtonsPressedAlt");

	CGraphic 
		gButtons(GetWidth(),GetHeight()),
		gButtonsAlt(GetWidth(),GetHeight()),
		gButtonsOver(GetWidth(),GetHeight()),
		gButtonsPressed(GetWidth(),GetHeight()),
		gButtonsPressedAlt(GetWidth(),GetHeight());

	m_pgBackground->Copy(&gButtons);
	gaButtons.Copy(&gButtons,8,7-SONGHANDLER_HEIGHT);
	m_pgBackground->Copy(&gButtonsAlt);
	gaButtonsAlt.Copy(&gButtonsAlt,8,7-SONGHANDLER_HEIGHT);
	m_pgBackground->Copy(&gButtonsOver);
	gaButtonsOver.Copy(&gButtonsOver,8,7-SONGHANDLER_HEIGHT);
	m_pgBackground->Copy(&gButtonsPressed);
	gaButtonsPressed.Copy(&gButtonsPressed,8,7-SONGHANDLER_HEIGHT);
	m_pgBackground->Copy(&gButtonsPressedAlt);
	gaButtonsPressedAlt.Copy(&gButtonsPressedAlt,8,7-SONGHANDLER_HEIGHT);

	// Copy the control information from the buttons layer
	gButtons.Copy(m_pgBackground,9,323,9,323,250,18);

	// Create drum button
	m_pgButton=new CGraphic(BUTTONWIDTH,BUTTONHEIGHT);
	gButtons.Copy(m_pgButton,0,0,
		BUTTONX,BUTTONY,BUTTONWIDTH,BUTTONHEIGHT);
	m_pgButtonOver=new CGraphic(BUTTONWIDTH,BUTTONHEIGHT);
	gButtonsOver.Copy(m_pgButtonOver,0,0,
		BUTTONX,BUTTONY,BUTTONWIDTH,BUTTONHEIGHT);
	m_pgButtonPressed=new CGraphic(BUTTONWIDTH,BUTTONHEIGHT);
	gButtonsPressed.Copy(m_pgButtonPressed,0,0,
		BUTTONX,BUTTONY,BUTTONWIDTH,BUTTONHEIGHT);

	// Store bit of images used for checkboxes etc
	m_pgButtonsAltBit=new CGraphic(GetWidth(),CDrumsHandler_BUTTONSPACING);
	gButtonsAlt.Copy(m_pgButtonsAltBit,0,0,0,34,GetWidth(),CDrumsHandler_BUTTONSPACING);
	m_pgButtonsPressedAltBit=new CGraphic(GetWidth(),CDrumsHandler_BUTTONSPACING);
	gButtonsPressedAlt.Copy(m_pgButtonsPressedAltBit,0,0,0,34,GetWidth(),CDrumsHandler_BUTTONSPACING);
	m_pgButtonsBit=new CGraphic(GetWidth(),CDrumsHandler_BUTTONSPACING);
	gButtons.Copy(m_pgButtonsBit,0,0,0,34,GetWidth(),CDrumsHandler_BUTTONSPACING);
	m_pgButtonsPressedBit=new CGraphic(GetWidth(),CDrumsHandler_BUTTONSPACING);
	gButtonsPressed.Copy(m_pgButtonsPressedBit,0,0,0,34,GetWidth(),CDrumsHandler_BUTTONSPACING);

	// Create initial drum buttons and scrollbars
	for(int iLine=0;iLine<16;iLine++)
	{
		CreateCheck(IDC_DRUMS_MUTE0+iLine,104,34+iLine*17,12,12,
			gButtonsAlt,gButtons,gButtonsPressedAlt,gButtonsPressed);

		CreateHScroll(IDC_DRUMS_LEVELSCROLL0+iLine,120,31+iLine*17,96,
			0,256,64,*m_pgBackground);
		CreateHScroll(IDC_DRUMS_PANSCROLL0+iLine,220,31+iLine*17,96,
			0,256,64,*m_pgBackground);
	}

	m_pgfFont=new CGraphicFont("Verdana",11,FALSE);
}

// Draw this section onto the specified graphic
void CDrumsHandler::Draw(CGraphicOnscreen& goTarget)
{
	PaintBackground(goTarget,m_pgBackground);

	// Draw buttons
	for(int iLine=0;iLine<GetNumDisplayLines();iLine++)
	{
		int iSound=GetSoundNumber(iLine);
		if(iSound>=GetSong()->GetNumSounds()) continue;

		if(m_iPressedSound==iSound && m_iOverSound==iSound)
			m_pgButtonPressed->Copy(&goTarget,BUTTONX,BUTTONY+iLine*CDrumsHandler_BUTTONSPACING);
		else if(m_iOverSound==iSound)
			m_pgButtonOver->Copy(&goTarget,BUTTONX,BUTTONY+iLine*CDrumsHandler_BUTTONSPACING);
		else
			m_pgButton->Copy(&goTarget,BUTTONX,BUTTONY+iLine*CDrumsHandler_BUTTONSPACING);

		CString strName=GetSong()->GetSound(iSound).GetName();
		if(strName=="") strName="(Click to set...)";

		goTarget.Clip(BUTTONX,BUTTONY+iLine*CDrumsHandler_BUTTONSPACING,BUTTONWIDTH,BUTTONHEIGHT);
		m_pgfFont->Draw(&goTarget,BUTTONX+2,BUTTONY+iLine*CDrumsHandler_BUTTONSPACING,
			strName,RGB(0,0,0));
		goTarget.UnClip();
	}
}

void CDrumsHandler::OnButton(int iID)
{
	if(iID>=IDC_DRUMS_MUTE0 && iID<IDC_DRUMS_MUTE0+GetNumDisplayLines())
	{
		GetSong()->GetSound(GetSoundNumber(iID-IDC_DRUMS_MUTE0)).
			SetMute(GetCheck(iID));
		MarkChange();
	}
}

void CDrumsHandler::OnHScroll(int iID,int iValue)
{
	if(iID>=IDC_DRUMS_LEVELSCROLL0 && iID<IDC_DRUMS_LEVELSCROLL0+GetNumDisplayLines())
	{
		GetSong()->GetSound(GetSoundNumber(iID-IDC_DRUMS_LEVELSCROLL0)).
			SetLevel(iValue);
		MarkChange();
	}
	else if(iID>=IDC_DRUMS_PANSCROLL0 && iID<IDC_DRUMS_PANSCROLL0+GetNumDisplayLines())
	{
		GetSong()->GetSound(GetSoundNumber(iID-IDC_DRUMS_PANSCROLL0)).
			SetPan(iValue);
		MarkChange();
	}
}

void CDrumsHandler::Update()
{
	// Get V pos
	m_iVScroll=
		((CPatternHandler*)
			(GetView()->GetSection(typeid(CPatternHandler))))->
			GetVScrollPos();

	// Set button to name of each sound
	for(int iLine=0;iLine<GetNumDisplayLines();iLine++)
	{
		int iSongSound=GetSoundNumber(iLine);

		// Get scrollbars
		CGraphicHScroll
			* pghsLevel=(CGraphicHScroll*)GetItem(IDC_DRUMS_LEVELSCROLL0+iLine),
			* pghsPan=(CGraphicHScroll*)GetItem(IDC_DRUMS_PANSCROLL0+iLine);
		CGraphicCheckbox
			* pgcMute=(CGraphicCheckbox*)GetItem(IDC_DRUMS_MUTE0+iLine);

		// See if track is valid
		if(GetSong()->GetNumSounds() <= iSongSound)
		{
			// Not valid, hide scrollbars
			ShowControl(IDC_DRUMS_LEVELSCROLL0+iLine,FALSE);
			ShowControl(IDC_DRUMS_PANSCROLL0+iLine,FALSE);
			ShowControl(IDC_DRUMS_MUTE0+iLine,FALSE);
		}
		else
		{
			// Valid, set & show scrollbars
			pghsLevel->SetPos(GetSong()->GetSound(iSongSound).GetLevel());
			ShowControl(IDC_DRUMS_LEVELSCROLL0+iLine,TRUE);
			pghsPan->SetPos(GetSong()->GetSound(iSongSound).GetPan());
			ShowControl(IDC_DRUMS_PANSCROLL0+iLine,TRUE);
			pgcMute->SetChecked(GetSong()->GetSound(iSongSound).GetMute());
			ShowControl(IDC_DRUMS_MUTE0+iLine,TRUE);
		}
	}

	Repaint(BUTTONX,BUTTONY,BUTTONWIDTH,GetNumDisplayLines()*CDrumsHandler_BUTTONSPACING);
}

void CDrumsHandler::SetOver(int iSound)
{
	if(m_iOverSound==iSound) return;
	m_iOverSound=iSound;
	Repaint(BUTTONX,BUTTONY,BUTTONWIDTH,GetNumDisplayLines()*CDrumsHandler_BUTTONSPACING);
}

void CDrumsHandler::SetPressed(int iSound)
{
	if(m_iPressedSound==iSound) return;
	m_iPressedSound=iSound;
	Repaint(BUTTONX,BUTTONY,BUTTONWIDTH,GetNumDisplayLines()*CDrumsHandler_BUTTONSPACING);
}

void CDrumsHandler::OnCancelMode()
{
	if(HasCapture()) 
	{
		Release();
		SetPressed();
	}
}

int CDrumsHandler::GetLine(int iX,int iY)
{
	int iLine=-1;
	if(iX>=BUTTONX && iY>=BUTTONY && iX<BUTTONX+BUTTONWIDTH)
	{
		if( (iY-BUTTONY)%CDrumsHandler_BUTTONSPACING < BUTTONHEIGHT )
			iLine=(iY-BUTTONY)/CDrumsHandler_BUTTONSPACING;
		if(iLine>=GetNumDisplayLines()) iLine=-1;
	}
	return iLine;
}

void CDrumsHandler::OnMouseMove(int iX,int iY)
{
	int iLine=GetLine(iX,iY);
	if(iLine==-1)
		SetOver();
	else
	{
		int iSongSound=GetSoundNumber(iLine);

		if(iSongSound < GetSong()->GetNumSounds())
		{
			if(m_iPressedSound!=-1 && iSongSound!=m_iPressedSound)
			{
				SwapSounds(m_iPressedSound,iSongSound);
				m_bSwapped=TRUE;
			}
			SetOver(iSongSound);
		}
		else
			SetOver();
	}
}

void CDrumsHandler::OnLButtonDown(int iX,int iY)
{
	int iLine=GetLine(iX,iY);
	if(iLine!=-1)
	{
		int iSongSound=GetSoundNumber(iLine);
		if(iSongSound < GetSong()->GetNumSounds())
		{
			SetPressed(iSongSound);
			Capture();
			m_bSwapped=FALSE;
		}
	}
}

void CDrumsHandler::OnLButtonUp(int iX,int iY)
{
	Release();

	if(m_iPressedSound!=-1 && m_iOverSound==m_iPressedSound && !m_bSwapped)
	{
		((CSoundsHandler*)
			(GetView()->GetSection(typeid(CSoundsHandler))))->
			SetSound(&GetSong()->GetSound(m_iPressedSound));

		// Show other screen
		GetView()->ShowSoundScreen();
	}

	m_iPressedSound=-1;
}

void CDrumsHandler::OnMouseOut()
{
	SetOver();
}

int CDrumsHandler::GetSoundNumber(int iLine)
{
	int iOriginalSound=iLine+m_iVScroll;
	return GetSong()->GetSoundNumber(iOriginalSound);
}

void CDrumsHandler::SwapSounds(int iSound1,int iSound2)
{
	GetSong()->SwapSounds(iSound1,iSound2);
	MarkChange();
	Update();
	((CPatternHandler*)
		(GetView()->GetSection(typeid(CPatternHandler))))->
		RepaintPattern();
}


void CDrumsHandler::InformResize(int iPreviousWOffset,int iPreviousHOffset)
{
	int iNewExtraLines=GetHeightOffset()/CDrumsHandler_BUTTONSPACING,
		iCurrentExtraLines=iPreviousHOffset/CDrumsHandler_BUTTONSPACING;
	if(iCurrentExtraLines==iNewExtraLines) return;

	// Add extra buttons
	for(;iCurrentExtraLines<iNewExtraLines;iCurrentExtraLines++)
	{
		int iLine=16+iCurrentExtraLines;

		m_pgButtonsAltBit->Offset(0,-(34+iLine*17));
		m_pgButtonsPressedAltBit->Offset(0,-(34+iLine*17));
		m_pgButtonsBit->Offset(0,-(34+iLine*17));
		m_pgButtonsPressedBit->Offset(0,-(34+iLine*17));

		CreateCheck(IDC_DRUMS_MUTE0+iLine,104,34+iLine*17,12,12,
			*m_pgButtonsAltBit,*m_pgButtonsBit,*m_pgButtonsPressedAltBit,*m_pgButtonsPressedBit);
		if(IsHidden()) GetItem(IDC_DRUMS_MUTE0+iLine)->ShowWindow(SW_HIDE);

		m_pgButtonsAltBit->UnOffset();
		m_pgButtonsPressedAltBit->UnOffset();
		m_pgButtonsBit->UnOffset();
		m_pgButtonsPressedBit->UnOffset();

		m_pgBackground->Offset(0,-(iLine-16)*17);
		CreateHScroll(IDC_DRUMS_LEVELSCROLL0+iLine,120,31+iLine*17,96,
			0,256,64,*m_pgBackground);
		if(IsHidden()) GetItem(IDC_DRUMS_LEVELSCROLL0+iLine)->ShowWindow(SW_HIDE);
		CreateHScroll(IDC_DRUMS_PANSCROLL0+iLine,220,31+iLine*17,96,
			0,256,64,*m_pgBackground);
		if(IsHidden()) GetItem(IDC_DRUMS_PANSCROLL0+iLine)->ShowWindow(SW_HIDE);
		m_pgBackground->UnOffset();
	}
	// Remove unnecessary buttons
	for(;iCurrentExtraLines>iNewExtraLines;iCurrentExtraLines--)
	{
		int iLine=16+iCurrentExtraLines-1;

		DeleteControl(GetItem(IDC_DRUMS_MUTE0+iLine));
		DeleteControl(GetItem(IDC_DRUMS_LEVELSCROLL0+iLine));
		DeleteControl(GetItem(IDC_DRUMS_PANSCROLL0+iLine));
	}

	// Update
	Update();
}
