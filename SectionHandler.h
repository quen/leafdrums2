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
#ifndef SECTIONHANDLER_H
#define SECTIONHANDLER_H

#include "vec.h"
#include "GraphicOnscreen.h"
#include "GraphicButton.h"
#include "GraphicCheckbox.h"
#include "GraphicHScroll.h"
#include "GraphicVScroll.h"
#include "GraphicTextOption.h"
#include "GraphicNumberOption.h"
#include "CustomEdit.h"
#include "Song.h"

class CLeafDrumsView;

class CSectionHandler
{
private:
	// Controls owned
	vec<CWnd*> m_vpwControls;

	// Controls that are supposed to be hidden
	vec<CWnd*> m_vpwHidden;

	// Co-ordinates of TL of this section
	int m_iX,m_iY;

	// If window has been stretched
	int m_iWidthOffset,m_iHeightOffset;

	// True if hidden
	BOOL m_bHidden;

	// View parent
	CLeafDrumsView* m_pView;

	// Move all controls after/including the specific coordinates
	void MoveControls(int iMoveX,int iMoveY,int iAfterX,int iAfterY);
	// Move all controls when section moves
	void ShiftControls(int iMoveX,int iMoveY);

protected:
	// Add control to owned list
	void AddControl(CWnd* pwControl) { m_vpwControls.Add(pwControl); }
	void DeleteControl(CWnd* pwControl) { m_vpwControls.Remove(pwControl); pwControl->DestroyWindow(); }

	CLeafDrumsView* GetView() { return m_pView; }
	CSong* GetSong();

	BOOL IsDoubleClick();

	// Shortcut methods 

	CGraphicButton* CreateButton(int iID,int iX,int iY,int iWidth,int iHeight,
		CGraphic& gNormal,CGraphic& gOver,CGraphic& gDown);

	CGraphicCheckbox* CreateCheck(int iID,int iX,int iY,int iWidth,int iHeight,
		CGraphic& gNormal,CGraphic& gChecked,CGraphic& gDownNormal,CGraphic& gDownChecked);

	CGraphicHScroll* CreateHScroll(int iID,int iX,int iY,int iWidth,
		int iMin,int iMax,int iPage,CGraphic& gNormal);
	CGraphicVScroll* CreateVScroll(int iID,int iX,int iY,int iHeight,
		int iMin,int iMax,int iPage,CGraphic& gNormal);

	CGraphicNumberOption* CreateNumberOption(int iID,int iX,int iY,int iWidth,
		int iMin,int iMax,CGraphic& gNormal);
	CGraphicTextOption* CreateTextOption(int iID,int iX,int iY,int iWidth,
		CGraphic& gNormal);

	CCustomEdit* CreateEdit(int iID,int iX,int iY,int iWidth,int iHeight);

	CWnd* GetItem(int iID);
	CGraphicHScroll* GetHScroll(int iID) { return (CGraphicHScroll*)GetItem(iID); }
	CGraphicVScroll* GetVScroll(int iID) { return (CGraphicVScroll*)GetItem(iID); }
	CGraphicTextOption* GetTextOption(int iID) { return (CGraphicTextOption*)GetItem(iID); }
	CGraphicNumberOption* GetNumberOption(int iID) { return (CGraphicNumberOption*)GetItem(iID); }

	void ShowControl(int iID,BOOL bShow);

	BOOL GetCheck(int iID);
	void SetCheck(int iID,BOOL bChecked);

	// Call to tell mfc that document has changed
	void MarkChange();

	// Call to re-send a mousemove for current mouse position
	void RefreshMouse();

	// Call to paint background, repeating pixels at SplitX and SplitY if needed
	void PaintBackground(CGraphicOnscreen& goTarget,CGraphic* pgBackground);

public:
	CSectionHandler() { m_bHidden=FALSE; m_iWidthOffset=0; m_iHeightOffset=0; }

	int GetX() { return m_iX; }
	int GetY() { return m_iY; }
	virtual int GetWidth() { return -1; }
	virtual int GetHeight() { return -1; }

	int GetWidthOffset() { return m_iWidthOffset; }
	int GetHeightOffset() { return m_iHeightOffset; }

	virtual ~CSectionHandler() {}

	// Create all the controls for this section
	virtual void Create(CLeafDrumsView* pView,int iX,int iY);

	// Show/hide all the controls for this section
	void Show();
	void Hide();
	BOOL IsHidden() { return m_bHidden; }

	// Returns true if this section includes the specified control
	BOOL HasControl(CWnd* pwControl);

	// Returns true if given co-ordinate is in this area
	BOOL HasPoint(int iX,int iY);

	// Repaints this section or a section of it
	void Repaint(int iX=0,int iY=0,int iWidth=-1,int iHeight=-1);

	// Capture/release the mouse for this section
	void Capture();
	void Release();
	BOOL HasCapture();
	virtual void OnCancelMode() {}

	// Draw this section onto the specified graphic
	virtual void Draw(CGraphicOnscreen& goTarget) {}

	// Pass on command messages
	virtual void OnHScroll(int iID,int iValue) {}
	virtual void OnVScroll(int iID,int iValue) {}
	virtual void OnSelchange(int iID,int iValue) {}
	virtual void OnButton(int iID) {}
	virtual void OnPlayPositionChanged() {}
	virtual void OnPlayStopped() {}
	virtual void OnLButtonDown(int iX,int iY) {}
	virtual void OnRButtonDown(int iX,int iY) {}
	virtual void OnLButtonUp(int iX,int iY) {}
	virtual void OnRButtonUp(int iX,int iY) {}
	virtual void OnMouseMove(int iX,int iY) {}
	virtual void OnMouseOut() {}
	virtual void OnSystemKeyChange(int iX,int iY) {}

	// Section should reset its controls etc.
	virtual void NewSong() {}

	// Called when window is resized with the new width+height offsets
	void MoveSize(int iX,int iY,int iWidth,int iHeight);

	// Override to do things when window is resized
	// (controls have already been automoved by this point).
	virtual void InformResize(int iPreviousWOffset,int iPreviousHOffset) {}

	// Override to false if you don't want controls automoved
	virtual BOOL AutomoveControl(CWnd* pw) { return TRUE; }

	// If this stretches widthways, return the pixel co-ord
	// after which controls gets moved right
	virtual int GetSplitX() { return -1; }
	// Same for if this stretches heightways
	virtual int GetSplitY() { return -1; }
};

#include "LeafDrumsView.h"

#endif //  SECTIONHANDLER_H
