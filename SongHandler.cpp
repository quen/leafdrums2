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
#include "SongHandler.h"

#include "PatternHandler.h"

#include "GraphicAlpha.h"
#include "GraphicButton.h"
#include "GraphicHScroll.h"

#include "SetTempoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define CODAY 23
#define CODAHEIGHT 6

#define CODAFLAGOFFSET 1
#define CODAFLAGWIDTH 6

#define CODAARROWOFFSET 48
#define CODAARROWWIDTH 8

#define CODAFLAG_MASK 1024
#define CODAREPEAT_MASK 2048

COLORREF CODAARROWCOLOUR[3]= { RGB(21,26,103),RGB(40,103,21),RGB(103,34,21) };


CSongHandler::CSongHandler()
{
	m_iMarkerL=0;
	m_iMarkerR=1;
	m_pgHelp=NULL;
	m_iGridHighlight=m_iBarHighlight=m_iCodaMouseover=m_iTempoMouseover=-1;
	m_iAreaHighlightFromBar=m_iAreaHighlightFromTrack=
		m_iAreaHighlightToBar=m_iAreaHighlightToTrack=-1;

	m_iMode=SONGMODE_NORMAL;
}

CSongHandler::~CSongHandler()
{
	delete m_pgMarkerHelp;
	delete m_pgPatternHelp;
	delete m_pgEnvelopeHelp;
	delete m_pgInsertHelp;
	delete m_pgDeleteHelp;
	delete m_pgSelectHelp;
	delete m_pgPasteHelp;
	delete m_pgFlagHelp;
	delete m_pgArrowOnHelp;
	delete m_pgArrowOffHelp;
	delete m_pgTempoOnHelp;
	delete m_pgTempoSetOffHelp;
	delete m_pgBackground;
	delete m_pgaGridLine;
	delete m_pgaMarkerLine;
	delete m_pgaL;
	delete m_pgaR;
	delete m_pgaBlank;
	delete m_pgaPattern;
	delete m_pgaPatternMisfit;
	delete m_pgaRepeatColour;
	delete m_pgaRepeatGrey;
	delete m_pFont;
	delete m_pTinyFont;
	delete m_pgaTempo;
}

// Create all the Songs for this section
void CSongHandler::Create(CLeafDrumsView* pView,int iX,int iY)
{
	CSectionHandler::Create(pView,iX,iY);

	// Create font
	m_pFont=new CGraphicFont("Verdana",11,FALSE);
	m_pTinyFont=new CGraphicFont("Verdana",8,TRUE);

	// Create and load background graphics
	CGraphic background(CLIENTWIDTH,OTHERHEIGHT);
	background.LoadResource("SongScreenBackground");
	m_pgBackground=new CGraphic(GetWidth(),GetHeight());
	background.Copy(m_pgBackground,0,0);

	// Load incidental graphics
	m_pgaGridLine=new CGraphicAlpha(1,65);
	m_pgaGridLine->LoadResource("SongGridLine");
	m_pgaMarkerLine=new CGraphicAlpha(3,66);
	m_pgaMarkerLine->LoadResource("SongMarkerLine");
	m_pgaL=new CGraphicAlpha(5,6);
	m_pgaL->LoadResource("SongL");
	m_pgaR=new CGraphicAlpha(6,6);
	m_pgaR->LoadResource("SongR");
	m_pgaBlank=new CGraphicAlpha(49,15);
	m_pgaBlank->LoadResource("SongBlank");
	m_pgaPattern=new CGraphicAlpha(49,15);
	m_pgaPattern->LoadResource("SongPattern");
	m_pgaPatternMisfit=new CGraphicAlpha(49,15);
	m_pgaPatternMisfit->LoadResource("SongPatternMisfit");

	m_pgaRepeatGrey=new CGraphicAlpha(48,6);
	m_pgaRepeatGrey->LoadResource("RepeatGrey");
	m_pgaRepeatColour=new CGraphicAlpha(48,6);
	m_pgaRepeatColour->LoadResource("RepeatColour");

	m_pgaTempo=new CGraphicAlpha(3,6);
	m_pgaTempo->LoadResource("TempoSign");

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
	gaButtons.Copy(&gButtons,8,7);
	m_pgBackground->Copy(&gButtonsAlt);
	gaButtonsAlt.Copy(&gButtonsAlt,8,7);
	m_pgBackground->Copy(&gButtonsOver);
	gaButtonsOver.Copy(&gButtonsOver,8,7);
	m_pgBackground->Copy(&gButtonsPressed);
	gaButtonsPressed.Copy(&gButtonsPressed,8,7);
	m_pgBackground->Copy(&gButtonsPressedAlt);
	gaButtonsPressedAlt.Copy(&gButtonsPressedAlt,8,7);

	// Create the help info
	m_pgMarkerHelp=new CGraphic(SONGHELPWIDTH,SONGHELPHEIGHT);
	gButtons.Copy(m_pgMarkerHelp,-SONGHELPX,-SONGHELPY);
	m_pgPatternHelp=new CGraphic(SONGHELPWIDTH,SONGHELPHEIGHT);
	gButtonsAlt.Copy(m_pgPatternHelp,-SONGHELPX,-SONGHELPY);
	m_pgEnvelopeHelp=new CGraphic(SONGHELPWIDTH,SONGHELPHEIGHT);
	gButtonsOver.Copy(m_pgEnvelopeHelp,-SONGHELPX,-SONGHELPY);

	m_pgInsertHelp=new CGraphic(SONGHELPWIDTH,SONGHELPHEIGHT);
	background.Copy(m_pgInsertHelp,-SONGHELPX,-SONGHELPY);
	CGraphicAlpha gaInsert(350,19);
	gaInsert.LoadResource("Control8");
	gaInsert.Copy(m_pgInsertHelp,0,-1);

	m_pgDeleteHelp=new CGraphic(SONGHELPWIDTH,SONGHELPHEIGHT);
	background.Copy(m_pgDeleteHelp,-SONGHELPX,-SONGHELPY);
	CGraphicAlpha gaDelete(350,19);
	gaDelete.LoadResource("Control7");
	gaDelete.Copy(m_pgDeleteHelp,0,-1);

	m_pgSelectHelp=new CGraphic(SONGHELPWIDTH,SONGHELPHEIGHT);
	background.Copy(m_pgSelectHelp,-SONGHELPX,-SONGHELPY);
	CGraphicAlpha gaSelect(187,19);
	gaSelect.LoadResource("Control9");
	gaSelect.Copy(m_pgSelectHelp,0,-1);

	m_pgPasteHelp=new CGraphic(SONGHELPWIDTH,SONGHELPHEIGHT);
	background.Copy(m_pgPasteHelp,-SONGHELPX,-SONGHELPY);
	CGraphicAlpha gaPaste(480,19);
	gaPaste.LoadResource("Control10");
	gaPaste.Copy(m_pgPasteHelp,0,-1);

	m_pgFlagHelp=new CGraphic(SONGHELPWIDTH,SONGHELPHEIGHT);
	background.Copy(m_pgFlagHelp,-SONGHELPX,-SONGHELPY);
	CGraphicAlpha gaFlag(92,19);
	gaFlag.LoadResource("Control4");
	gaFlag.Copy(m_pgFlagHelp,0,-1);

	m_pgArrowOnHelp=new CGraphic(SONGHELPWIDTH,SONGHELPHEIGHT);
	background.Copy(m_pgArrowOnHelp,-SONGHELPX,-SONGHELPY);
	CGraphicAlpha gaArrowOn(407,19);
	gaArrowOn.LoadResource("Control6");
	gaArrowOn.Copy(m_pgArrowOnHelp,0,-1);

	m_pgArrowOffHelp=new CGraphic(SONGHELPWIDTH,SONGHELPHEIGHT);
	background.Copy(m_pgArrowOffHelp,-SONGHELPX,-SONGHELPY);
	CGraphicAlpha gaArrowOff(105,19);
	gaArrowOff.LoadResource("Control5");
	gaArrowOff.Copy(m_pgArrowOffHelp,0,-1);

	m_pgTempoOnHelp=new CGraphic(SONGHELPWIDTH,SONGHELPHEIGHT);
	m_pgTempoSetOffHelp=new CGraphic(SONGHELPWIDTH,SONGHELPHEIGHT);
	background.Copy(m_pgTempoOnHelp,-SONGHELPX,-SONGHELPY);
	background.Copy(m_pgTempoSetOffHelp,-SONGHELPX,-SONGHELPY);
	CGraphicAlpha gaTempo(310,19);
	gaTempo.LoadResource("Control11");
	gaTempo.Copy(m_pgTempoSetOffHelp,0,-1);
	gaTempo.Copy(m_pgTempoOnHelp,0,-1,0,0,200,19);

	// Create buttons
	CreateButton(IDC_SONG_INSERT,646,7,61,12,
		gButtons,gButtonsOver,gButtonsPressed);
	CreateButton(IDC_SONG_DELETE,711,7,61,12,
		gButtons,gButtonsOver,gButtonsPressed);
	CreateButton(IDC_SONG_SELECT,544,7,48,12,
		gButtons,gButtonsOver,gButtonsPressed);
	CreateButton(IDC_SONG_PASTE,596,7,42,12,
		gButtons,gButtonsOver,gButtonsPressed);

	// Create checkboxes
	CreateCheck(IDC_SONG_LEVELENVELOPE,8,109,104,12,
		gButtonsAlt,gButtons,gButtonsPressedAlt,gButtonsPressed);
	CreateCheck(IDC_SONG_LOOP,8,125,42,12,
		gButtonsAlt,gButtons,gButtonsPressedAlt,gButtonsPressed);

	// Create scrollbars
	for(int iTrack=0;iTrack<4;iTrack++)
	{
		CreateHScroll(IDC_SONG_LEVELSCROLL0+iTrack,8,31+iTrack*17,112,
			0,256,64,*m_pgBackground);
	}
	CreateHScroll(IDC_SONG_HSCROLL,SONGX,SONGY+SONGHEIGHT+10,SONGWIDTH,
		0,0,1,*m_pgBackground);
	((CGraphicHScroll*)GetItem(IDC_SONG_HSCROLL))->SetStep(49);

	CreateVScroll(IDC_SONG_VSCROLL,SONGX+SONGWIDTH+2,SONGY,SONGHEIGHT,
		0,0,1,*m_pgBackground);

	UpdateLevelValues();
	SetHelp(NULL);
}

// Draw this section onto the specified graphic
void CSongHandler::Draw(CGraphicOnscreen& goTarget)
{
	// Draw background
	PaintBackground(goTarget,m_pgBackground);

	// Draw help
	if(m_pgHelp)
		m_pgHelp->Copy(&goTarget,SONGHELPX,SONGHELPY);

	// Get song
	CSong* pSong=GetSong();

	// Get current scroll positions
	int iCurrentVPos=
		((CGraphicVScroll*)GetItem(IDC_SONG_VSCROLL))->
		GetPos(),
		iCurrentHPos=
		((CGraphicHScroll*)GetItem(IDC_SONG_HSCROLL))->
		GetPos();

	// Set clip for song area
	goTarget.Clip(SONGX-1,SONGY-OVERSONGPAINT,SONGWIDTH+GetWidthOffset(),SONGHEIGHT+TEMPOSPACEABOVE+TEMPOHEIGHT+OVERSONGPAINT);

	// Get song length
	int iSongLength=pSong->GetBarCount();

	// Make sure song has at least one track
	if(pSong->GetNumTracks()==0) pSong->AddTrack();

	// Work out bar positions to begin and end on
	int 
		iBarStart=iCurrentHPos/49,
		iBarEnd=(iCurrentHPos/49 + (SONGWIDTH+GetWidthOffset())/49+2);
	if(iBarEnd > iSongLength+1) iBarEnd=iSongLength+1;

	// Draw blank spaces
	for(int iBar=iBarStart;iBar<iBarEnd;iBar++)
	{
		// Get x position of start
		int iXPos=SONGX + (iBar*49) - iCurrentHPos;

		// Draw tracks
		for(int iTrack=0;iTrack<4;iTrack++)
		{
			// Get current track
			if(iTrack+iCurrentVPos>=pSong->GetNumTracks()) continue;
			CTrack& track=pSong->GetTrack(iTrack+iCurrentVPos);

			// Draw blank if present
			if(track.GetBar(iBar)==NULL)
				m_pgaBlank->Copy(&goTarget,iXPos,SONGY+17*iTrack);
		}
	}

	// Draw markers
	for(iBar=iBarStart;iBar<iBarEnd+1;iBar++)
	{
		// Get x position of start
		int iXPos=SONGX + (iBar*49) - iCurrentHPos;

		// Draw markers if present
		if(m_iMarkerL==iBar)
		{
			m_pgaMarkerLine->Copy(&goTarget,iXPos-1,SONGY);

			if((m_iCodaMouseover==-1 || m_iCodaMouseover<iBar-1 || m_iCodaMouseover>iBar) &&
				pSong->GetCoda(iBar).IsEmpty() && (iBar==0 || pSong->GetCoda(iBar-1).IsEmpty()))
				m_pgaL->Copy(&goTarget,iXPos-2,SONGY-8);
		}
		if(m_iMarkerR==iBar)
		{
			m_pgaMarkerLine->Copy(&goTarget,iXPos-1,SONGY);

			if((m_iCodaMouseover==-1 || m_iCodaMouseover<iBar-1 || m_iCodaMouseover>iBar) &&
				pSong->GetCoda(iBar).IsEmpty() && (iBar==0 || pSong->GetCoda(iBar-1).IsEmpty()))
				m_pgaR->Copy(&goTarget,iXPos-2,SONGY-8);
		}
	}

	// Draw bar numbers
	for(iBar=iBarStart;iBar<iBarEnd;iBar++)
	{
		if(m_iCodaMouseover!=-1 && m_iCodaMouseover==iBar) continue;
		
		if(!pSong->GetCoda(iBar).IsEmpty()) continue;

		// Get x position of start
		int iXPos=SONGX + (iBar*49) - iCurrentHPos;

		// Write text
		CString sNumber;
		sNumber.Format("%i",iBar+1);
		m_pTinyFont->Draw(&goTarget,
			(iXPos*2+49-m_pTinyFont->Width(sNumber))/2,
			SONGY-10, 
			sNumber,RGB(0,0,0));
	}

	// Draw patterns 
	for(iBar=iBarStart;iBar<iBarEnd;iBar++)
	{
		// Get x position of start
		int iXPos=SONGX + (iBar*49) - iCurrentHPos;

		// Get timesig of bar
		int iTimeSignature=pSong->GetUniqueTimeSignature(iBar);

		// Draw tracks
		for(int iTrack=0;iTrack<4;iTrack++)
		{
			// Get current track
			if(iTrack+iCurrentVPos>=pSong->GetNumTracks()) continue;
			CTrack& track=pSong->GetTrack(iTrack+iCurrentVPos);

			// Draw blank if present
			CPattern* pPattern=track.GetBar(iBar);
			if(pPattern!=NULL)
			{
				// Copy background
				if(pPattern->GetTimeSignature()==iTimeSignature)
					m_pgaPattern->Copy(&goTarget,iXPos,SONGY+17*iTrack);
				else
					m_pgaPatternMisfit->Copy(&goTarget,iXPos,SONGY+17*iTrack);

				// Write text
				m_pFont->Draw(&goTarget,
					(iXPos*2+49-m_pFont->Width(pPattern->GetName()))/2,
					SONGY+17*iTrack+1, 
					pPattern->GetName(),RGB(255,255,255));
			}
		}

	}

	// Draw gridlines 
	for(iBar=iBarStart;iBar<=iBarEnd;iBar++)
	{
		// Get x position of start
		int iXPos=SONGX + (iBar*49) - iCurrentHPos;

		// Draw gridline
		m_pgaGridLine->Copy(&goTarget,iXPos,SONGY);
	}

	// Draw level envelopes
	if(ShowLevelEnvelope())
	{
		for(int iTrack=0;iTrack<4;iTrack++)
		{
			// Get current track
			if(iTrack+iCurrentVPos>=pSong->GetNumTracks()) continue;
			CTrack& track=pSong->GetTrack(iTrack+iCurrentVPos);

			// Get level envelope
			CVariableParameter& vpLevel=track.GetLevelEnvelope();

			// Get level at start position
			int iLevel=vpLevel.GetValueAt(iBarStart,0);
			int iBar=iBarStart,iFraction=0;

			while(true)
			{
				int iNextPoint=vpLevel.GetNextPoint(iBar,iFraction);
				if(iNextPoint==-1)
				{
					// Draw line static at this level to the end
					goTarget.DrawLine(
						SONGX + (iBar*49) + (iFraction*49)/256 - iCurrentHPos,
						SONGY+17*iTrack+14 - (iLevel*14)/256, 
						SONGX + (iBarEnd*49) - iCurrentHPos,
						SONGY+17*iTrack+14 - (iLevel*14)/256, 
						RGB(255,97,30));
					break;
				}

				int iNextBar=vpLevel.GetPointBar(iNextPoint);
				int iNextFraction=vpLevel.GetPointFraction(iNextPoint);
				int iNextLevel=vpLevel.GetPointValue(iNextPoint);

				// Draw next point itself
				goTarget.FillRect(
					SONGX + (iNextBar*49) + (iNextFraction*49)/256 - iCurrentHPos-2,
					SONGY+17*iTrack+14 - (iNextLevel*14)/256-2,
					5,5,RGB(255,255,255));
				goTarget.FillRect(
					SONGX + (iNextBar*49) + (iNextFraction*49)/256 - iCurrentHPos-1,
					SONGY+17*iTrack+14 - (iNextLevel*14)/256-1,
					3,3,RGB(0,0,0));

				// Draw line to next point
				goTarget.DrawLine(
					SONGX + (iBar*49) + (iFraction*49)/256 - iCurrentHPos,
					SONGY+17*iTrack+14 - (iLevel*14)/256, 
					SONGX + (iNextBar*49) + (iNextFraction*49)/256 - iCurrentHPos,
					SONGY+17*iTrack+14 - (iNextLevel*14)/256,
					RGB(255,97,30));

				if(iNextBar>iBarEnd) return;

				iBar=iNextBar; iFraction=iNextFraction; iLevel=iNextLevel;
			}
		}
	}

	// Draw flags and arrows
	for(iBar=iBarStart;iBar<iBarEnd;iBar++)
	{
		// Get x position of start
		int iXPos=SONGX + (iBar*49) - iCurrentHPos;

		CCoda& c=pSong->GetCoda(iBar);

		// Draw background mouseover
		if(m_iCodaMouseover!=-1 && m_iCodaMouseover==iBar) 
		{
			int iItem=(m_iCodaMouseoverItem & (~(CODAFLAG_MASK|CODAREPEAT_MASK)));
			if(m_iCodaMouseoverItem & CODAFLAG_MASK) // Flag
			{
				m_pgaRepeatGrey->Copy(&goTarget,
					iXPos+CODAFLAGOFFSET+CODAFLAGWIDTH*iItem,CODAY,
					CODAFLAGWIDTH*iItem,0,CODAFLAGWIDTH,CODAHEIGHT);
			}
			if(m_iCodaMouseoverItem & CODAREPEAT_MASK) // Arrow
			{
				// Draw arrow mouseover
				m_pgaRepeatGrey->Copy(&goTarget,
					iXPos+CODAARROWOFFSET-CODAARROWWIDTH*(iItem+1),CODAY,
					CODAARROWOFFSET-CODAARROWWIDTH*(iItem+1),0,CODAARROWWIDTH,CODAHEIGHT);

				// If arrow's selected
				if(c.HasRepeat(iItem))
				{
					// Get count
					CString sCount;
					sCount.Format("%i",c.GetRepeat(iItem)+1);

					// Outline box
					goTarget.FillRect(
						iXPos+CODAARROWOFFSET-CODAARROWWIDTH*(iItem+1)-13,
						SONGY-1,
						14,12,
						CODAARROWCOLOUR[iItem]);

					// Clear box
					goTarget.FillRect(
						iXPos+CODAARROWOFFSET-CODAARROWWIDTH*(iItem+1)-12,
						SONGY,
						12,10,
						RGB(0xff,0xff,0xff));

					// Draw count
					m_pTinyFont->Draw(&goTarget,
						iXPos+CODAARROWOFFSET-CODAARROWWIDTH*(iItem+1)-12+6-
						  (m_pTinyFont->Width(sCount)/2),
						SONGY, 
						sCount,CODAARROWCOLOUR[iItem]);
				}
			}
		}

		// Draw each flag
		for(int iMarker=0;iMarker<CCoda_MAXMARKERS;iMarker++)
		{
			if(c.HasFlag(iMarker))
			{
				m_pgaRepeatColour->Copy(&goTarget,iXPos+CODAFLAGOFFSET+CODAFLAGWIDTH*iMarker,CODAY,
					CODAFLAGWIDTH*iMarker,0,CODAFLAGWIDTH,CODAHEIGHT);
			}

			if(c.HasRepeat(iMarker))
			{
				m_pgaRepeatColour->Copy(&goTarget,iXPos+CODAARROWOFFSET-CODAARROWWIDTH*(iMarker+1),CODAY,
					CODAARROWOFFSET-CODAARROWWIDTH*(iMarker+1),0,CODAARROWWIDTH,CODAHEIGHT);
			}
		}
	}

	// Draw tempo markers
	for(iBar=iBarStart;iBar<iBarEnd;iBar++)
	{
		// Get x position of start
		int iXPos=SONGX + (iBar*49) - iCurrentHPos;

		// Draw tempo mouseover
		if(m_iTempoMouseover==iBar)
		{
			int iXPos=SONGX+m_iTempoMouseover*49-iCurrentHPos;
			goTarget.FillRect(iXPos,SONGY+SONGHEIGHT+TEMPOSPACEABOVE,49,TEMPOHEIGHT,RGB(255,255,255),190);
		}

		CTimeChange& tc=pSong->GetTimeChange(iBar);
		if(tc.IsEmpty()) continue;

		m_pgaTempo->Copy(&goTarget,iXPos,SONGY+SONGHEIGHT+TEMPOSPACEABOVE);

		CString sDetails;
		sDetails.Format("%i %i:4",tc.GetTempo(),tc.GetTimeSignature());
		m_pTinyFont->Draw(&goTarget,
			iXPos+5,
			SONGY+SONGHEIGHT+TEMPOSPACEABOVE-2, 
			sDetails,RGB(0,0,0));
	}

	// Draw grid highlight
	if(m_iGridHighlight!=-1)
	{
		int iXPos=SONGX+m_iGridHighlight*49-iCurrentHPos;
		goTarget.FillRect(iXPos-4,SONGY,9,SONGHEIGHT,RGB(255,255,255),210);
	}

	// Draw bar highlight
	if(m_iBarHighlight!=-1)
	{
		int iXPos=SONGX+m_iBarHighlight*49-iCurrentHPos;
		goTarget.FillRect(iXPos+1,SONGY,48,SONGHEIGHT,RGB(255,255,255),210);
	}


	// Draw area highlight
	if(m_iAreaHighlightFromBar!=-1)
	{
		// Order co-ords
		int 
			iLowestBar=min(m_iAreaHighlightFromBar, m_iAreaHighlightToBar),
			iLowestTrack=min(m_iAreaHighlightFromTrack, m_iAreaHighlightToTrack),
			iHighestBar=max(m_iAreaHighlightFromBar, m_iAreaHighlightToBar),
			iHighestTrack=max(m_iAreaHighlightFromTrack, m_iAreaHighlightToTrack);

		goTarget.FillRect(
			SONGX+iLowestBar*49-iCurrentHPos,
			SONGY+(iLowestTrack-iCurrentVPos)*17,
			(iHighestBar-iLowestBar+1)*49,
			(iHighestTrack-iLowestTrack+1)*17-2,
			RGB(255,255,255),210);
	}

	// Draw play position
	if(GetSong()->IsPlaying())
	{
		int iXPos=SONGX+m_iCurrentBar*49 + m_iCurrentSixteenth*3-iCurrentHPos;
		goTarget.FillRect(iXPos,SONGY,1,SONGHEIGHT,RGB(255,97,30));
	}

	goTarget.UnClip();
}

void CSongHandler::OnHScroll(int iID,int iValue)
{
	if(iID==IDC_SONG_HSCROLL)
		RepaintSong();
	else if(iID>=IDC_SONG_LEVELSCROLL0 && iID<IDC_SONG_LEVELSCROLL0+4)
	{
		GetSong()->GetTrack(iID-IDC_SONG_LEVELSCROLL0+GetVScrollPos()).
			SetLevel(iValue);
		MarkChange();
	}
}

void CSongHandler::OnVScroll(int iID,int iValue)
{
	UpdateLevelValues();
	RepaintSong();
}

void CSongHandler::OnSelchange(int iID,int iValue)
{
}


void CSongHandler::OnButton(int iID)
{
	// Handle loop and envelope here
	switch(iID)
	{
	case IDC_SONG_LEVELENVELOPE:
		RepaintSong();
		break;
	case IDC_SONG_LOOP:
		GetSong()->SetLoop(GetCheck(IDC_SONG_LOOP));
		break;
	case IDC_SONG_INSERT:
		m_iMode=SONGMODE_INSERT;
		Capture();
		RefreshMouse();
		break;
	case IDC_SONG_DELETE:
		m_iMode=SONGMODE_DELETE;
		Capture();
		RefreshMouse();
		break;
	case IDC_SONG_SELECT:
		m_iMode=SONGMODE_SELECT;
		m_iSelectFromBar=-1;
		Capture();
		RefreshMouse();
		break;
	case IDC_SONG_PASTE:
		if(GetSong()->GetClipboard().IsEmpty()) break;
		m_iMode=SONGMODE_PASTE;
		Capture();
		RefreshMouse();
		break;
	}
}

void CSongHandler::OnLButtonDown(int iX,int iY)
{
	// Get song
	CSong* pSong=GetSong(); 

	// Don't do flags if we're in a particular mode
	if(m_iMode==SONGMODE_NORMAL)
	{
		// See if we're on a repeat flag
		int iFlag=GetRepeatFlag(iX,iY);
		if(iFlag!=-1)
		{
			int iBar=GetBar(iX);
			CCoda& c=pSong->GetCoda(iBar);
			c.SetFlag(iFlag,!c.HasFlag(iFlag));
			RepaintSong();
			return;
		}

		// See if we're on a repeat arrow
		int iArrow=GetRepeatArrow(iX,iY);
		if(iArrow!=-1)
		{
			int iBar=GetBar(iX);
			CCoda& c=pSong->GetCoda(iBar);
			c.SetRepeat(iArrow,c.HasRepeat(iArrow) ? 0 : 1);
			RepaintSong();
			RefreshMouse();
			return;
		}
	}

	// Check in song area
	if(!InSongArea(iX,iY)) return;

	// Handle level envelope
	if(ShowLevelEnvelope())
	{
		int iPoint,iBar,iFraction,iLevel;
		if(OnEnvelope(iX,iY,iPoint,iBar,iFraction,iLevel))
		{
			CVariableParameter& vpLevel=
				GetSong()->GetTrack(GetTrack(iY)).GetLevelEnvelope();

			if(iPoint==-1)
			{
				// Add point
				vpLevel.AddPoint(iBar,iFraction,iLevel);
				iPoint=vpLevel.GetPreviousPoint(iBar,iFraction);
				RepaintSong();
				MarkChange();
				OnMouseMove(iX,iY);
			}

			// Begin moving point
			m_iMode=SONGMODE_DRAGPOINT;
			m_iDragStartX=iX;
			m_iDragStartY=iY;
			m_iDragStartPos=vpLevel.GetPointBar(iPoint)*256+vpLevel.GetPointFraction(iPoint);
			m_iDragStartLevel=vpLevel.GetPointValue(iPoint);
			m_pvpDragEnvelope=&vpLevel;
			m_iDragPoint=iPoint;
			Capture();
			return;
		}

	}

	// Handle insert mode
	if(m_iMode==SONGMODE_INSERT)
	{
		// Convert into song position
		int iBar=GridBar(iX);

		SetGridHighlight();

		// See if position is valid
		if(iBar<pSong->GetBarCount())
			pSong->InsertBar(iBar);

		// Cancel mode, unless they held control
		if(!((GetKeyState(VK_CONTROL)&0x8000)==0x8000))
		{
			m_iMode=SONGMODE_NORMAL;
			Release();
		}
		OnMouseMove(iX,iY);
		RepaintSong();
		return;
	}

	// Handle delete mode
	if(m_iMode==SONGMODE_DELETE)
	{
		// Convert into song position
		int iBar=GetBar(iX);

		SetBarHighlight();

		// See if position is valid
		if(iBar<pSong->GetBarCount())
			pSong->DeleteBar(iBar);

		// Cancel mode, unless they held control
		if(!((GetKeyState(VK_CONTROL)&0x8000)==0x8000))
		{
			m_iMode=SONGMODE_NORMAL;
			Release();
		}

		OnMouseMove(iX,iY);
		RepaintSong();
		return;
	}

	// Handle select mode (click to begin select)
	if(m_iMode==SONGMODE_SELECT)
	{
		// Convert into song position
		int iBar=GetBar(iX),iTrack=GetTrack(iY);

		// See if position is valid
		if(iBar<pSong->GetBarCount() && iTrack<pSong->GetNumTracks())
		{
			m_iSelectFromBar=iBar;
			m_iSelectFromTrack=iTrack;
		}

		OnMouseMove(iX,iY);
		return;
	}

	// Handle paste mode
	if(m_iMode==SONGMODE_PASTE)
	{
		// Update mouse pos just in case
		OnMouseMove(iX,iY);

		// Actually do the paste if available
		if(m_iAreaHighlightFromBar!=-1)
		{
			GetSong()->GetClipboard().Paste(*(GetSong()),
				m_iAreaHighlightFromBar,m_iAreaHighlightFromTrack,TRUE);

			// Add another track below if there isn't one yet
			if(pSong->GetNumTracks() ==
				m_iAreaHighlightFromTrack + GetSong()->GetClipboard().GetHeight() )
			{
				pSong->AddTrack();
				UpdateLevelValues();
			}

			// Repaint
			RepaintSong();
			UpdateScrollbars();
		}
		SetAreaHighlight();

		// Cancel mode, unless they held control
		if(!((GetKeyState(VK_CONTROL)&0x8000)==0x8000))
		{
			m_iMode=SONGMODE_NORMAL;
			Release();
		}
		OnMouseMove(iX,iY);
		RepaintSong();
		return;
	}

	if(m_iMode==SONGMODE_NORMAL)
	{
		// See if cursor's on grid
		if(OnGrid(iX))
		{
			if(GridBar(iX)<=pSong->GetBarCount() && GridBar(iX)!=m_iMarkerR)
			{
				m_iMarkerL=GridBar(iX);
				if(m_iMarkerR<m_iMarkerL)
				{
					int iTemp=m_iMarkerR;
					m_iMarkerR=m_iMarkerL;
					m_iMarkerL=iTemp;
				}
				GetSong()->SetPlayRange(m_iMarkerL,m_iMarkerR);
				RepaintSong();
				MarkChange();
			}
			return;
		}

		// Convert into song position
		int iBar=GetBar(iX),iTrack=GetTrack(iY);

		// Check position is valid
		if(iBar>pSong->GetBarCount() || iTrack>=pSong->GetNumTracks())
			return;

		// See if Ctrl is held down
		if((GetKeyState(VK_CONTROL)&0x8000)==0x8000)
		{
			// Go through existing patterns to see what number names there are
			int iHighestNumber=1;
			for(int i=0;i<pSong->GetNumPatterns();i++)
			{
				// Get name
				CString sName=pSong->GetPattern(i).GetName();

				// See if it's all a number
				for(int i=0;i<sName.GetLength();i++)
					if(!isdigit(sName[i])) break;
				if(i!=sName.GetLength()) continue;

				// Get number
				int iNumber=atoi(sName);
				if(iNumber>iHighestNumber) iHighestNumber=iNumber;
			}

			MarkChange();

			// Create a new pattern
			CPattern& p=pSong->AddPattern();

			// Name it
			CString sName;
			sName.Format("%i",iHighestNumber+1);
			p.SetName(sName);

			// Set it at this location
			pSong->GetTrack(iTrack).SetBar(*pSong,iBar,&p);

			// Add another track below if there isn't one yet
			if(pSong->GetNumTracks()==iTrack+1)
			{
				pSong->AddTrack();
				UpdateLevelValues();
			}

			// Show pattern in pattern handler
			((CPatternHandler*)
				(GetView()->GetSection(typeid(CPatternHandler))))->
				SetPatternIndex(p.GetIndex(*pSong));

			// Repaint
			RepaintSong();
			UpdateScrollbars();

			// Update cursor
			OnMouseMove(iX,iY);

			return;
		}

		// See if there's already a pattern there
		if(pSong->GetTrack(iTrack).GetBar(iBar)!=NULL)
		{
			pSong->GetTrack(iTrack).SetBar(*pSong,iBar,NULL);
			MarkChange();
		}
		else
		{
			int iPatternIndex=
				((CPatternHandler*)
					(GetView()->GetSection(typeid(CPatternHandler))))->
					GetPatternIndex();

			pSong->GetTrack(iTrack).SetBar(*pSong,iBar,&pSong->GetPattern(iPatternIndex));
			MarkChange();

			// Add another track below if there isn't one yet
			if(pSong->GetNumTracks()==iTrack+1)
			{
				pSong->AddTrack();
				UpdateLevelValues();
			}
		}
		RepaintSong();
		UpdateScrollbars();

		// Update cursor
		OnMouseMove(iX,iY);
	}
}

void CSongHandler::OnRButtonDown(int iX,int iY)
{
	// Get song
	CSong* pSong=GetSong(); 

	// Handle paste mode
	if(m_iMode==SONGMODE_PASTE)
	{
		// Update mouse pos just in case
		OnMouseMove(iX,iY);

		// Actually do the paste if available
		if(m_iAreaHighlightFromBar!=-1)
		{
			GetSong()->GetClipboard().Paste(*(GetSong()),
				m_iAreaHighlightFromBar,m_iAreaHighlightFromTrack,FALSE);

			// Add another track below if there isn't one yet
			if(pSong->GetNumTracks() ==
				m_iAreaHighlightFromTrack + GetSong()->GetClipboard().GetHeight() )
			{
				pSong->AddTrack();
				UpdateLevelValues();
			}

			// Repaint
			RepaintSong();
			UpdateScrollbars();
		}
		SetAreaHighlight();

		// Cancel mode, unless they held control
		if(!((GetKeyState(VK_CONTROL)&0x8000)==0x8000))
		{
			m_iMode=SONGMODE_NORMAL;
			Release();
		}
		OnMouseMove(iX,iY);
		RepaintSong();
		return;
	}

	if(m_iMode==SONGMODE_NORMAL)
	{
		// See if we're on a repeat arrow
		int iArrow=GetRepeatArrow(iX,iY);
		if(iArrow!=-1)
		{
			int iBar=GetBar(iX);
			CCoda& c=pSong->GetCoda(iBar);
			if(!c.HasRepeat(iArrow)) return;

			// See if Ctrl is held down
			int iDirection;
			if((GetKeyState(VK_CONTROL)&0x8000)==0x8000)
				iDirection=-1;
			else
				iDirection=1;

			// Adjust if permitted
			int iNew=c.GetRepeat(iArrow)+iDirection;
			if(iNew<=0 || iNew>98) return;
			c.SetRepeat(iArrow,iNew);

			// Repaint and stuff
			RepaintSong();
			return;
		}

		int iTempoBar=GetTempoBar(iX,iY);
		if(iTempoBar>0) // Don't allow at position 0 either
		{
			CTimeChange& tc=GetSong()->GetTimeChange(iTempoBar);
			if(!tc.IsEmpty())
			{
				tc.SetEmpty();
				OnMouseMove(iX,iY);
				RepaintSong();
				MarkChange();
				return;
			}
		}

		
		// Check in song area
		if(!InSongArea(iX,iY)) return;

		// Handle level envelope
		if(ShowLevelEnvelope())
		{
			int iPoint,iBar,iFraction,iLevel;
			if(OnEnvelope(iX,iY,iPoint,iBar,iFraction,iLevel))
			{
				CVariableParameter& vpLevel=
					GetSong()->GetTrack(GetTrack(iY)).GetLevelEnvelope();

				if(iPoint!=-1)
				{
					// Delete point
					vpLevel.RemovePoint(
						vpLevel.GetPointBar(iPoint),vpLevel.GetPointFraction(iPoint));
					RepaintSong();
					MarkChange();
					OnMouseMove(iX,iY);
				}

				return;
			}

		}

		// See if cursor's on grid
		if(OnGrid(iX))
		{
			if(GridBar(iX)<=pSong->GetBarCount() && GridBar(iX)!=m_iMarkerL)
			{
				m_iMarkerR=GridBar(iX);
				if(m_iMarkerR<m_iMarkerL)
				{
					int iTemp=m_iMarkerR;
					m_iMarkerR=m_iMarkerL;
					m_iMarkerL=iTemp;
				}
				GetSong()->SetPlayRange(m_iMarkerL,m_iMarkerR);
				RepaintSong();
				MarkChange();
			}
			return;
		}
		else
		{
			// Convert into song position
			int iBar=GetBar(iX),iTrack=GetTrack(iY);

			// Check position is valid
			if(iBar>pSong->GetBarCount() || iTrack>=pSong->GetNumTracks())
				return;

			CPattern* pPattern=pSong->GetTrack(iTrack).GetBar(iBar);
			if(pPattern!=NULL)
			{
				((CPatternHandler*)
					(GetView()->GetSection(typeid(CPatternHandler))))->
					SetPatternIndex(pPattern->GetIndex(*pSong));
			}
			return;
		}
	}
}

void CSongHandler::OnLButtonUp(int iX,int iY)
{
	if(m_iMode==SONGMODE_DRAGPOINT)
	{
		m_iMode=SONGMODE_NORMAL;
		Release();
	}
	else if(m_iMode==SONGMODE_SELECT)
	{
		OnMouseMove(iX,iY);

		if(m_iAreaHighlightFromBar!=-1)
		{
			GetSong()->GetClipboard().Set(*(GetSong()),
				m_iAreaHighlightFromBar,m_iAreaHighlightFromTrack,
				m_iAreaHighlightToBar,m_iAreaHighlightToTrack);
		}
		SetAreaHighlight();

		m_iMode=SONGMODE_NORMAL;
		Release();
	}
	else if(m_iTempoMouseover!=-1)
	{
		CSetTempoDlg dlg(this,GetSong(),m_iTempoMouseover,AfxGetMainWnd());
		dlg.DoModal();
	}
}

void CSongHandler::OnRButtonUp(int iX,int iY)
{
}

BOOL CSongHandler::InSongArea(int iX,int iY)
{
	return !(iX<SONGX || iY<SONGY || iX>=SONGX+SONGWIDTH+GetWidthOffset() || iY>=SONGY+SONGHEIGHT);
}

BOOL CSongHandler::OnGrid(int iX)
{
	// Get current scroll positions
	int iCurrentHPos=
			((CGraphicHScroll*)GetItem(IDC_SONG_HSCROLL))->GetPos();

	// Get position relative to a bar
	int iBarRelative=(iX-SONGX+iCurrentHPos)%49;

	// At edges, consider on grid
	return (iBarRelative <=2 || iBarRelative>=47);
}

int CSongHandler::GridBar(int iX)
{
	// Get current scroll positions
	int iCurrentHPos=
			((CGraphicHScroll*)GetItem(IDC_SONG_HSCROLL))->GetPos();

	// Get position relative to a bar
	return (iX-SONGX+iCurrentHPos+24)/49;
}

BOOL CSongHandler::OnEnvelope(int iX,int iY,int& iOutputPoint,int& iBar,int& iFraction,int& iOutputLevel)
{
	// Get current scroll positions
	int iCurrentHPos=
			((CGraphicHScroll*)GetItem(IDC_SONG_HSCROLL))->GetPos();
	int iCurrentVPos=
			((CGraphicVScroll*)GetItem(IDC_SONG_VSCROLL))->GetPos();

	// Check relative-to-track level
	int iYLevel=14-(iY-SONGY)%17;
	if(iYLevel<0) return FALSE;

	// Get track
	int iTrack=(iY-SONGY)/17 + iCurrentVPos;
	if(GetSong()->GetNumTracks()<=iTrack) return FALSE;
	CTrack& track=GetSong()->GetTrack(iTrack);

	// Get time
	iBar=(iX-SONGX+iCurrentHPos)/49;
	if(iBar>GetSong()->GetBarCount()) return FALSE;
	iFraction=(((iX-SONGX+iCurrentHPos)%49)*256)/49;

	// Get level at position
	CVariableParameter& vpLevel=track.GetLevelEnvelope();
	iOutputLevel=vpLevel.GetValueAt(iBar,iFraction);

	// Convert to Y value and compare
	int iActualLevel=(iOutputLevel*14)/256;
	int iDifference=iYLevel-iActualLevel;
	if(iDifference<0) iDifference=-iDifference;
	if(iDifference>2) return FALSE;

	// See if there's a point near
	int iPointBefore=vpLevel.GetPreviousPoint(iBar,iFraction),
		iPointAfter=vpLevel.GetNextPoint(iBar,iFraction);

	// Get difference from here and point
	int iBeforeDifference=(iPointBefore==-1 ? 10000 : (
			vpLevel.GetPointBar(iPointBefore)*256+vpLevel.GetPointFraction(iPointBefore) -
			(iBar*256+iFraction))),
		iAfterDifference=(iPointAfter==-1 ? 10000 : (
			vpLevel.GetPointBar(iPointAfter)*256+vpLevel.GetPointFraction(iPointAfter) -
			(iBar*256+iFraction)));
	if(iBeforeDifference<0) iBeforeDifference=-iBeforeDifference;
	if(iAfterDifference<0) iAfterDifference=-iAfterDifference;

	// Get closest point
	int iClosestPoint=(iBeforeDifference>iAfterDifference) 
		? iPointAfter : iPointBefore;
	int iClosestDifference=(iBeforeDifference>iAfterDifference) 
		? iAfterDifference: iBeforeDifference;

	// If it's close, set that point
	if(iClosestDifference<=15)
		iOutputPoint=iClosestPoint;
	else
	{
		iOutputPoint=-1;
	}

	return TRUE;
}

int CSongHandler::GetBar(int iX)
{
	// Get current scroll positions
	int iCurrentHPos=
			((CGraphicHScroll*)GetItem(IDC_SONG_HSCROLL))->GetPos();

	// Convert into song position
	return (iX-SONGX+iCurrentHPos)/49;
}


int CSongHandler::GetTrack(int iY)
{
	// Get current scroll positions
	int iCurrentVPos=
			((CGraphicVScroll*)GetItem(IDC_SONG_VSCROLL))->GetPos();

	// Convert into song position
	return (iY-SONGY)/17 + iCurrentVPos;
}

int CSongHandler::GetRepeatFlag(int iX,int iY)
{
	// Check Y
	if(!(iY>=CODAY && iY<CODAY+CODAHEIGHT)) return -1;

	// Get song-relative position
	int iPos=iX-SONGX+GetHScroll(IDC_SONG_HSCROLL)->GetPos();

	// Convert to bar & check it's in song
	if(iPos<0 || (iPos/49)>=GetSong()->GetBarCount())
		return -1;

	// Check it's within flag area
	iPos%=49;
	if(iPos<CODAFLAGOFFSET || iPos>=CODAFLAGOFFSET+CODAFLAGWIDTH*CCoda_MAXMARKERS)
		return -1;

	return (iPos - CODAFLAGOFFSET)/CODAFLAGWIDTH;
}

int CSongHandler::GetRepeatArrow(int iX,int iY)
{
	// Check Y
	if(!(iY>=CODAY && iY<CODAY+CODAHEIGHT)) return -1;

	// Get song-relative position
	int iPos=iX-SONGX+GetHScroll(IDC_SONG_HSCROLL)->GetPos();

	// Convert to bar & check it's in song
	if(iPos<0 || (iPos/49)>=GetSong()->GetBarCount())
		return -1;

	// Check it's within flag area
	iPos%=49;
	if(iPos>CODAARROWOFFSET || iPos<=CODAARROWOFFSET-CODAARROWWIDTH*CCoda_MAXMARKERS)
		return -1;

	return (CODAARROWOFFSET-iPos)/CODAARROWWIDTH;
}


void CSongHandler::OnMouseMove(int iX,int iY)
{
	// See if we're dragging the point
	if(m_iMode==SONGMODE_DRAGPOINT)
	{
		// Work out desired position from here
		int iDesiredPos=
			((iX-m_iDragStartX)*256)/49 + m_iDragStartPos;

		// Clip this against adjoining points and end of song
		if(iDesiredPos/256 > GetSong()->GetBarCount())
			iDesiredPos=GetSong()->GetBarCount()*256+255;
		if(iDesiredPos<1) iDesiredPos=1;
		if(m_iDragPoint+1<m_pvpDragEnvelope->GetNumPoints())
		{
			int iNextPos=
				m_pvpDragEnvelope->GetPointBar(m_iDragPoint+1)*256+
				m_pvpDragEnvelope->GetPointFraction(m_iDragPoint+1);
			if(iDesiredPos >= iNextPos) iDesiredPos=iNextPos-1;
		}
		if(m_iDragPoint>0)
		{
			int iPrevPos=
				m_pvpDragEnvelope->GetPointBar(m_iDragPoint-1)*256+
				m_pvpDragEnvelope->GetPointFraction(m_iDragPoint-1);
			if(iDesiredPos <= iPrevPos) iDesiredPos=iPrevPos+1;
		}

		// Work out desired level
		int iDesiredLevel=
			((m_iDragStartY-iY)*256)/14 + m_iDragStartLevel;
		if(iDesiredLevel>256) iDesiredLevel=256;
		else if(iDesiredLevel<0) iDesiredLevel=0;

		// Set point to this location
		m_pvpDragEnvelope->RemovePoint(
			m_pvpDragEnvelope->GetPointBar(m_iDragPoint),
			m_pvpDragEnvelope->GetPointFraction(m_iDragPoint));
		m_pvpDragEnvelope->AddPoint(
			iDesiredPos/256,iDesiredPos%256,iDesiredLevel);

		// Repaint song
		RepaintSong();
		MarkChange();
		return;
	}

	// Get song
	CSong* pSong=GetSong(); 

	// See if we're in insert mode
	if(m_iMode==SONGMODE_INSERT)
	{
		if(!InSongArea(iX,iY))
		{
			::SetCursor(::LoadCursor(AfxGetResourceHandle(),MAKEINTRESOURCE(IDC_POINTERNO)));
			SetGridHighlight();
			SetHelp(NULL);
			return;
		}

		// Convert into song position
		int iBar=GridBar(iX);

		// See if position is valid
		if(iBar>=pSong->GetBarCount())
		{
			::SetCursor(::LoadCursor(AfxGetResourceHandle(),MAKEINTRESOURCE(IDC_POINTERNO)));		
			SetGridHighlight();
			SetHelp(NULL);
			return;
		}

		// Ok, they can insert
		SetHelp(m_pgInsertHelp);
		SetGridHighlight(iBar);
		::SetCursor(::LoadCursor(AfxGetResourceHandle(),MAKEINTRESOURCE(IDC_POINTERINSERT)));		

		return;
	}

	if(m_iMode==SONGMODE_DELETE)
	{
		if(!InSongArea(iX,iY))
		{
			::SetCursor(::LoadCursor(AfxGetResourceHandle(),MAKEINTRESOURCE(IDC_POINTERNO)));
			SetBarHighlight();
			SetHelp(NULL);
			return;
		}

		// Convert into song position
		int iBar=GetBar(iX);

		// See if position is valid
		if(iBar>=pSong->GetBarCount())
		{
			::SetCursor(::LoadCursor(AfxGetResourceHandle(),MAKEINTRESOURCE(IDC_POINTERNO)));		
			SetBarHighlight();
			SetHelp(NULL);
			return;
		}

		// Ok, they can delete
		SetHelp(m_pgDeleteHelp);
		SetBarHighlight(iBar);
		::SetCursor(::LoadCursor(AfxGetResourceHandle(),MAKEINTRESOURCE(IDC_POINTERDEL)));		

		return;
	}

	if(m_iMode==SONGMODE_SELECT)
	{
		if(!InSongArea(iX,iY))
		{
			::SetCursor(::LoadCursor(AfxGetResourceHandle(),MAKEINTRESOURCE(IDC_POINTERNO)));
			SetAreaHighlight();
			SetHelp(NULL);
			return;
		}

		// Convert into song position
		int iBar=GetBar(iX),iTrack=GetTrack(iY);

		// See if position is valid
		if(iBar>=pSong->GetBarCount() || iTrack>=pSong->GetNumTracks())
		{
			::SetCursor(::LoadCursor(AfxGetResourceHandle(),MAKEINTRESOURCE(IDC_POINTERNO)));		
			SetAreaHighlight();
			SetHelp(NULL);
			return;
		}

		// Ok, they can select, or are selecting
		if(m_iSelectFromBar!=-1)
		{
			SetAreaHighlight(m_iSelectFromBar,m_iSelectFromTrack,iBar,iTrack);
		}
		else
		{
			SetAreaHighlight(iBar,iTrack,iBar,iTrack);
		}

		SetHelp(m_pgSelectHelp);
		::SetCursor(::LoadCursor(NULL,IDC_ARROW));

		return;
	}

	if(m_iMode==SONGMODE_PASTE)
	{
		if(!InSongArea(iX,iY))
		{
			::SetCursor(::LoadCursor(AfxGetResourceHandle(),MAKEINTRESOURCE(IDC_POINTERNO)));
			SetAreaHighlight();
			SetHelp(NULL);
			return;
		}

		// Convert into song position
		int iBar=GetBar(iX),iTrack=GetTrack(iY);

		CPatternBlock& pb=pSong->GetClipboard();

		// See if position is valid
		if(iBar>pSong->GetBarCount() || iTrack+pb.GetHeight()-1 >= pSong->GetNumTracks())
		{
			::SetCursor(::LoadCursor(AfxGetResourceHandle(),MAKEINTRESOURCE(IDC_POINTERNO)));		
			SetAreaHighlight();
			SetHelp(NULL);
			return;
		}

		// Ok, they can paste
		SetAreaHighlight(iBar,iTrack,iBar+pb.GetWidth()-1,iTrack+pb.GetHeight()-1);
		SetHelp(m_pgPasteHelp);
		::SetCursor(::LoadCursor(AfxGetResourceHandle(),MAKEINTRESOURCE(IDC_POINTERADD)));		

		return;
	}

	if(GetRepeatFlag(iX,iY)!=-1)
	{
		::SetCursor(::LoadCursor(NULL,IDC_ARROW));
		SetHelp(m_pgFlagHelp);
		SetCodaMouseover(GetBar(iX),GetRepeatFlag(iX,iY) | CODAFLAG_MASK);
		return;
	}
	if(GetRepeatArrow(iX,iY)!=-1)
	{
		::SetCursor(::LoadCursor(NULL,IDC_ARROW));
		BOOL bArrow=GetSong()->GetCoda(GetBar(iX)).HasRepeat(GetRepeatArrow(iX,iY));
		SetHelp(bArrow ? m_pgArrowOnHelp : m_pgArrowOffHelp);
		SetCodaMouseover(GetBar(iX),GetRepeatArrow(iX,iY) | CODAREPEAT_MASK);
		return;
	}
	SetCodaMouseover();

	if(GetTempoBar(iX,iY)!=-1)
	{
		int iTempoBar=GetTempoBar(iX,iY);
		::SetCursor(::LoadCursor(NULL,IDC_ARROW));
		if(GetSong()->GetTimeChange(iTempoBar).IsEmpty() || iTempoBar==0) // You can't clear first bar
			SetHelp(m_pgTempoOnHelp);
		else
			SetHelp(m_pgTempoSetOffHelp);
		SetTempoMouseover(iTempoBar);
		return;
	}
	SetTempoMouseover();

	// See if cursor is outside song area
	if(!InSongArea(iX,iY))
	{
		::SetCursor(::LoadCursor(NULL,IDC_ARROW));
		SetHelp(NULL);
		return;
	}

	// See if cursor's on level envelope
	if(ShowLevelEnvelope())
	{
		int iPoint,iBar,iFraction,iLevel;
		if(OnEnvelope(iX,iY,iPoint,iBar,iFraction,iLevel))
		{
			if(iPoint==-1)
				::SetCursor(::LoadCursor(AfxGetResourceHandle(),
					MAKEINTRESOURCE(IDC_POINTERADDPOINT)));
			else
				::SetCursor(::LoadCursor(AfxGetResourceHandle(),
					MAKEINTRESOURCE(IDC_POINTERMOVE)));

			SetHelp(m_pgEnvelopeHelp);
			return;
		}

	}

	// See if cursor's on grid
	if(OnGrid(iX))
	{
		if(GridBar(iX)<=pSong->GetBarCount())
		{
			::SetCursor(::LoadCursor(AfxGetResourceHandle(),MAKEINTRESOURCE(IDC_POINTERGRID)));
			SetHelp(m_pgMarkerHelp);
		}
		else
		{
			::SetCursor(::LoadCursor(AfxGetResourceHandle(),MAKEINTRESOURCE(IDC_POINTERNO)));		
		}
		return;
	}

	// Convert into song position
	int iBar=GetBar(iX),iTrack=GetTrack(iY);

	// See if position is valid
	if(iBar>pSong->GetBarCount() || iTrack>=pSong->GetNumTracks())
	{
		::SetCursor(::LoadCursor(AfxGetResourceHandle(),MAKEINTRESOURCE(IDC_POINTERNO)));		
		SetHelp(NULL);
		return;
	}

	SetHelp(m_pgPatternHelp);

	// See if Ctrl is held down
	if((GetKeyState(VK_CONTROL)&0x8000)==0x8000)
	{
		::SetCursor(::LoadCursor(AfxGetResourceHandle(),MAKEINTRESOURCE(IDC_POINTERADD)));		
		return;
	}

	// See if there is a pattern at that location and set cursor
	// accordingly
	if(pSong->GetTrack(iTrack).GetBar(iBar)==NULL)
	{
		::SetCursor(::LoadCursor(AfxGetResourceHandle(),MAKEINTRESOURCE(IDC_POINTERADD)));		
	}
	else
	{
		::SetCursor(::LoadCursor(AfxGetResourceHandle(),MAKEINTRESOURCE(IDC_POINTERDEL)));		
	}
}

void CSongHandler::SetHelp(CGraphic* pgHelp)
{
	if(m_pgHelp==pgHelp) return;
	m_pgHelp=pgHelp;
	Repaint(SONGHELPX,SONGHELPY,SONGHELPWIDTH,SONGHELPHEIGHT);
}

void CSongHandler::OnMouseOut() 
{
	if(m_iMode!=SONGMODE_NORMAL) return;

	// Clear cursor
	::SetCursor(::LoadCursor(NULL,IDC_ARROW));

	// Clear help
	SetHelp(NULL);

	// Clear coda mouseover
	SetCodaMouseover();
}

void CSongHandler::OnSystemKeyChange(int iX,int iY)
{
	OnMouseMove(iX,iY);
}

void CSongHandler::UpdateScrollbars()
{
	// Horizontal scrollbar
	CGraphicHScroll* pghs=(CGraphicHScroll*)GetItem(IDC_SONG_HSCROLL);
	pghs->SetWidth(SONGWIDTH+GetWidthOffset(),200);
	pghs->SetRange(0,(GetSong()->GetBarCount()+1)*49-(SONGWIDTH+GetWidthOffset()),SONGWIDTH+GetWidthOffset());

	// Vertical
	CGraphicVScroll* pgvs=(CGraphicVScroll*)GetItem(IDC_SONG_VSCROLL);
	pgvs->SetRange(0,(GetSong()->GetNumTracks()-4),4);
}

void CSongHandler::UpdateLevelValues()
{
	CGraphicVScroll* pgvs=(CGraphicVScroll*)GetItem(IDC_SONG_VSCROLL);

	// For each track...
	for(int iTrack=0;iTrack<4;iTrack++)
	{
		// Get scrollbars
		CGraphicHScroll
			* pghsLevel=(CGraphicHScroll*)GetItem(IDC_SONG_LEVELSCROLL0+iTrack);

		// See if track is valid
		int iSongTrack=iTrack+pgvs->GetPos();
		if(GetSong()->GetNumTracks() <= iSongTrack)
		{
			// Not valid, hide scrollbar
			ShowControl(IDC_SONG_LEVELSCROLL0+iTrack,FALSE);
		}
		else
		{
			// Valid, set & show scrollbar
			pghsLevel->SetPos(GetSong()->GetTrack(iSongTrack).GetLevel());
			ShowControl(IDC_SONG_LEVELSCROLL0+iTrack,TRUE);
		}
	}
}

int CSongHandler::GetVScrollPos()
{
	CGraphicVScroll* pgvs=(CGraphicVScroll*)GetItem(IDC_SONG_VSCROLL);
	return pgvs->GetPos();
}

void CSongHandler::OnPlayPositionChanged()
{
	CGraphicHScroll* pghs=GetHScroll(IDC_SONG_HSCROLL);
	int
		iCurrentHPos=pghs->GetPos();

	// Repaint existing play position
	BOOL bPreviouslyVisible=FALSE;
	int iXPos=SONGX+m_iCurrentBar*49 + m_iCurrentSixteenth*3 - iCurrentHPos;
	if(iXPos>=SONGX && iXPos<SONGX+SONGWIDTH+GetWidthOffset())
	{
		bPreviouslyVisible=TRUE;
		Repaint(iXPos,SONGY,1,SONGHEIGHT);
	}

	m_iCurrentBar=GetSong()->GetCurrentBar();
	m_iCurrentSixteenth=GetSong()->GetCurrentSixteenth();

	iXPos=SONGX+m_iCurrentBar*49 + m_iCurrentSixteenth*3 - iCurrentHPos;
	if(iXPos>=SONGX && iXPos<SONGX+SONGWIDTH+GetWidthOffset())
		Repaint(iXPos,SONGY,1,SONGHEIGHT);
	else 
	{
		// Follow display by scrolling, if it was previously visible
		if(bPreviouslyVisible)
		{
			// Set position so bar is near the beginning of the scren
			int iTargetPos=m_iCurrentBar*49 + m_iCurrentSixteenth*3 - 49;
			if(iTargetPos<pghs->GetMin()) iTargetPos=pghs->GetMin();
			if(iTargetPos>pghs->GetMax()) iTargetPos=pghs->GetMax();
			pghs->SetPos(iTargetPos);

			OnHScroll(IDC_SONG_HSCROLL,iTargetPos);
		}
	}
}

// Clear on new song
void CSongHandler::NewSong()
{
	m_iMarkerL=GetSong()->GetStartBar();
	m_iMarkerR=GetSong()->GetStopBar();
	SetCheck(IDC_SONG_LOOP,GetSong()->IsLoop());
	UpdateScrollbars();
	GetHScroll(IDC_SONG_HSCROLL)->SetPos(0);
	GetVScroll(IDC_SONG_VSCROLL)->SetPos(0);
	UpdateLevelValues();
	RepaintSong();
}

BOOL CSongHandler::ShowLevelEnvelope()
{
	return GetCheck(IDC_SONG_LEVELENVELOPE);
}

void CSongHandler::OnCancelMode()
{
	if(m_iMode==SONGMODE_DRAGPOINT)
	{
		m_iMode=SONGMODE_NORMAL;
		Release();
	}
	if(m_iMode==SONGMODE_INSERT)
	{
		m_iMode=SONGMODE_NORMAL;
		Release();
		SetGridHighlight();
	}
	if(m_iMode==SONGMODE_DELETE)
	{
		m_iMode=SONGMODE_NORMAL;
		Release();
		SetBarHighlight();
	}
	if(m_iMode==SONGMODE_SELECT)
	{
		m_iMode=SONGMODE_NORMAL;
		Release();
		SetAreaHighlight();
	}
	if(m_iMode==SONGMODE_PASTE)
	{
		m_iMode=SONGMODE_NORMAL;
		Release();
		SetAreaHighlight();
	}
}

void CSongHandler::SetGridHighlight(int iBar /*=-1*/)
{
	if(m_iGridHighlight!=iBar)
	{
		m_iGridHighlight=iBar;
		RepaintSong();
	}
}

void CSongHandler::SetBarHighlight(int iBar /*=-1*/)
{
	if(m_iBarHighlight!=iBar)
	{
		m_iBarHighlight=iBar;
		RepaintSong();
	}
}

void CSongHandler::SetAreaHighlight(int iFromBar,int iFromTrack,int iToBar,int iToTrack)
{
	if(iFromBar==m_iAreaHighlightFromBar && iFromTrack==m_iAreaHighlightFromTrack &&
		iToBar==m_iAreaHighlightToBar && iToTrack==m_iAreaHighlightToTrack) return;

	m_iAreaHighlightFromBar=iFromBar;
	m_iAreaHighlightFromTrack=iFromTrack;
	m_iAreaHighlightToBar=iToBar;
	m_iAreaHighlightToTrack=iToTrack;
	RepaintSong();
}

void CSongHandler::SetCodaMouseover(int iBar /*=-1*/, int iItem /*=0*/)
{
	if(m_iCodaMouseover!=iBar || m_iCodaMouseoverItem!=iItem)
	{
		m_iCodaMouseover=iBar;
		m_iCodaMouseoverItem=iItem;
		RepaintSong();
	}
}

void CSongHandler::SetTempoMouseover(int iBar /*=-1*/)
{
	if(m_iTempoMouseover!=iBar)
	{
		m_iTempoMouseover=iBar;
		RepaintSong();
	}
}

int CSongHandler::GetTempoBar(int iX,int iY)
{
	if(iY < SONGY+SONGHEIGHT+TEMPOSPACEABOVE || iY >= SONGY+SONGHEIGHT+TEMPOSPACEABOVE+TEMPOHEIGHT)
		return -1;
	int iBar=GetBar(iX);
	if(iBar>=0 && iBar < GetSong()->GetBarCount()) 
		return iBar;
	else 
		return -1;
}

// Tell window when it's resized
void CSongHandler::InformResize(int iPreviousWOffset,int iPreviousHOffset)
{
	UpdateScrollbars();
}
