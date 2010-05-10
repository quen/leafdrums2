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
#ifndef SONGHANDLER_H
#define SONGHANDLER_H

#include "SectionHandler.h"
#include "GraphicFont.h"
#include "Song.h"

#define SONGX 128
#define SONGY 31
#define SONGWIDTH 627
#define SONGHEIGHT 66

#define TEMPOSPACEABOVE 2
#define TEMPOHEIGHT 6

#define SONGHELPX 128
#define SONGHELPY 126
#define SONGHELPWIDTH 407
#define SONGHELPHEIGHT 18

#define OVERSONGPAINT 8

#define SONGMODE_NORMAL 0
#define SONGMODE_DRAGPOINT 1
#define SONGMODE_INSERT 2
#define SONGMODE_DELETE 3
#define SONGMODE_SELECT 4
#define SONGMODE_PASTE 5



class CSongHandler : public CSectionHandler
{
private:
	int m_iMode;

	CGraphic* m_pgBackground;

	CGraphicAlpha 
		*m_pgaGridLine,*m_pgaMarkerLine,
		*m_pgaL,*m_pgaR,
		*m_pgaBlank,*m_pgaPattern,*m_pgaPatternMisfit,
		*m_pgaTempo;

	CGraphic
		*m_pgMarkerHelp,*m_pgPatternHelp,*m_pgEnvelopeHelp,
		*m_pgInsertHelp,*m_pgDeleteHelp,
		*m_pgSelectHelp,*m_pgPasteHelp,
		*m_pgFlagHelp,*m_pgArrowOnHelp,*m_pgArrowOffHelp,
		*m_pgTempoOnHelp,*m_pgTempoSetOffHelp;

	CGraphicFont* m_pFont;
	CGraphicFont* m_pTinyFont;

	CGraphicAlpha
		*m_pgaRepeatGrey, *m_pgaRepeatColour;

	CGraphic* m_pgHelp;

	int m_iMarkerL,m_iMarkerR;

	int GetBar(int iX);
	int GridBar(int iX);
	int GetTrack(int iY);
	BOOL InSongArea(int iX,int iY);
	BOOL OnGrid(int iX);
	BOOL OnEnvelope(int iX,int iY,int& iOutputPoint,int& iBar,int& iFraction,int& iOutputLevel);
	void SetHelp(CGraphic* pgHelp);
	void UpdateScrollbars();
	void UpdateLevelValues();
	int GetVScrollPos();
	int GetRepeatFlag(int iX,int iY);
	int GetRepeatArrow(int iX,int iY);

	// Set a grid line to highlight
	void SetGridHighlight(int iBar=-1);
	int m_iGridHighlight;

	// Set a bar to highlight
	void SetBarHighlight(int iBar=-1);
	int m_iBarHighlight;

	// Set a whole area to highlight
	void SetAreaHighlight(int iFromBar=-1,int iFromTrack=-1,int iToBar=-1,int iToTrack=-1);
	int m_iAreaHighlightFromBar,m_iAreaHighlightFromTrack,
		m_iAreaHighlightToBar,m_iAreaHighlightToTrack;

	// Set coda mouseover stuff
	void SetCodaMouseover(int iBar=-1,int iItem=0);
	int m_iCodaMouseover;
	int m_iCodaMouseoverItem;

	BOOL ShowLevelEnvelope();

	int m_iCurrentBar,m_iCurrentSixteenth;

	int m_iDragPoint,m_iDragStartX,m_iDragStartY,m_iDragStartPos,m_iDragStartLevel;
	CVariableParameter* m_pvpDragEnvelope;

	int m_iSelectFromBar,m_iSelectFromTrack;

	int m_iTempoMouseover;
	void SetTempoMouseover(int iBar=-1);

	int GetTempoBar(int iX,int iY);

public:
	virtual int GetWidth() { return CLIENTWIDTH; }
	virtual int GetHeight() { return SONGHANDLER_HEIGHT; }
	virtual int GetSplitX() { return 200; }

	CSongHandler();
	virtual ~CSongHandler();

	// Create all the controls for this section
	virtual void Create(CLeafDrumsView* pView,int iX,int iY);

	// Draw this section onto the specified graphic
	virtual void Draw(CGraphicOnscreen& goTarget);

	// Command messages
	virtual void OnHScroll(int iID,int iValue);
	virtual void OnVScroll(int iID,int iValue);
	virtual void OnSelchange(int iID,int iValue);
	virtual void OnButton(int iID);
	virtual void OnLButtonDown(int iX,int iY);
	virtual void OnRButtonDown(int iX,int iY);
	virtual void OnLButtonUp(int iX,int iY);
	virtual void OnRButtonUp(int iX,int iY);
	virtual void OnMouseMove(int iX,int iY);
	virtual void OnMouseOut();
	virtual void OnSystemKeyChange(int iX,int iY);
	virtual void OnPlayPositionChanged();
	virtual void OnCancelMode();

	void RepaintSong() { Repaint(SONGX-2,SONGY-OVERSONGPAINT,SONGWIDTH+4+GetWidthOffset(),SONGHEIGHT+TEMPOSPACEABOVE+TEMPOHEIGHT+OVERSONGPAINT); }

	int GetLMarker() { return m_iMarkerL; }
	int GetRMarker() { return m_iMarkerR; }

	// Clear on new song
	virtual void NewSong();

	// Tell window when it's resized
	virtual void InformResize(int iPreviousWOffset,int iPreviousHOffset);
};

#include "LeafDrumsView.h"

#endif // SONGHANDLER_H
