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
#ifndef SOUNDSHANDLER_H
#define SOUNDSHANDLER_H

#include "SectionHandler.h"
#include "FileHierarchy.h"

class CSoundsHandler : public CSectionHandler
{
private:
	CGraphic* m_pgBackground;

	// Icons for file list
	CGraphicAlpha *m_pgaPlus,*m_pgaMinus;

	// Current button status information
	int m_iActive;
	BOOL m_bOver,m_bPressed;

	// Number of active buttons
	int m_iNumButtons;

	// Selected sound
	CString m_sSelectedFolder,m_sSelectedFile;

	// Sound object being changed
	CSound* m_pSound;

	// Object managing displayed hierarchy
	CFileHierarchy m_fhHierarchy;

	void UpdateScrollbar();
	int GetVScroll();
	void RepaintButton(int iButton);
	void RepaintButtons();

	// Whether to jump back to song screen after selecting a sound
	BOOL m_bJumpBack;

	int GetFullButtonCount() { return 24+GetHeightOffset()/17; }
	int GetPartialButtonCount() { return 24 + (GetHeightOffset()+16/17); }

public:
	CSoundsHandler() { m_iActive=-1; m_bOver=FALSE; m_bPressed=FALSE; }

	virtual int GetWidth() { return 167; }
	virtual int GetHeight() { return 478+8; }
	virtual int GetSplitY() { return 100; }

	virtual ~CSoundsHandler();

	// Create all the controls for this section
	virtual void Create(CLeafDrumsView* pView,int iX,int iY);

	// Draw this section onto the specified graphic
	virtual void Draw(CGraphicOnscreen& goTarget);

	// Command messages
	virtual void OnVScroll(int iID,int iValue);
	virtual void OnButton(int iID);
	virtual void OnLButtonDown(int iX,int iY);
	virtual void OnLButtonUp(int iX,int iY);
	virtual void OnMouseMove(int iX,int iY);
	virtual void OnMouseOut();

	// From other sections
	void SetSound(CSound* pSound);
	BOOL IsSoundValid();
	void ResetCurrentSound();

	// Clear on new song
	virtual void NewSong();

	// Tell window when it's resized
	virtual void InformResize(int iPreviousWOffset,int iPreviousHOffset);
};

#endif // SOUNDSHANDLER_H
