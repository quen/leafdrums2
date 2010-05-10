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
#ifndef DRUMSHANDLER_H
#define DRUMSHANDLER_H

#include "SectionHandler.h"

#define CDrumsHandler_BUTTONSPACING 17


class CDrumsHandler : public CSectionHandler
{
private:
	CGraphic* m_pgBackground;

	int m_iVScroll;

	CGraphic* m_pgButton;
	CGraphic* m_pgButtonOver;
	CGraphic* m_pgButtonPressed;

	CGraphic* m_pgButtonsAltBit;
	CGraphic* m_pgButtonsBit;
	CGraphic* m_pgButtonsPressedAltBit;
	CGraphic* m_pgButtonsPressedBit;

	CGraphicFont* m_pgfFont;

	int m_iOverSound,m_iPressedSound;
	BOOL m_bSwapped;
	void SetOver(int iSound=-1);
	void SetPressed(int iSound=-1);

	int GetNumDisplayLines() { return 16+(GetHeightOffset()/CDrumsHandler_BUTTONSPACING); }

	int GetLine(int iX,int iY);

	// Call to swap two sound lines (has no effect in song, UI only)
	void SwapSounds(int iSound1,int iSound2);

public:
	CDrumsHandler() { m_iOverSound=m_iPressedSound=-1; }

	virtual int GetWidth() { return 324; }
	virtual int GetHeight() { return 341; }
	virtual int GetSplitY() { return 100; }

	virtual ~CDrumsHandler();

	// Create all the controls for this section
	virtual void Create(CLeafDrumsView* pView,int iX,int iY);

	// Draw this section onto the specified graphic
	virtual void Draw(CGraphicOnscreen& goTarget);

	// Command messages
	virtual void OnButton(int iID);
	virtual void OnHScroll(int iID,int iValue);

	virtual void OnCancelMode();

	virtual void OnLButtonDown(int iX,int iY);
	virtual void OnLButtonUp(int iX,int iY);
	virtual void OnMouseMove(int iX,int iY);
	virtual void OnMouseOut();

	// Called from pattern when buttons need to be updated
	void Update();

	// Section should reset its controls etc.
	virtual void NewSong() { Update(); }

	// Return number of a line after rearrangement
	int GetSoundNumber(int iScreenLine);

	// Don't move our controls!
	virtual BOOL AutomoveControl(CWnd* pw) { return FALSE; }
	virtual void InformResize(int iPreviousWOffset,int iPreviousHOffset);
};

#include "LeafDrumsView.h"

#endif // DRUMSHANDLER_H
