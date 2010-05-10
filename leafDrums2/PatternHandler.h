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
#ifndef PATTERNHANDLER_H
#define PATTERNHANDLER_H

#include "SectionHandler.h"

#define PATTERNX 4
#define PATTERNY 31
#define PATTERNWIDTH 427
#define PATTERNHEIGHT 270

#define PATTERNHELPX PATTERNX
#define PATTERNHELPY 323
#define PATTERNHELPWIDTH 370
#define PATTERNHELPHEIGHT 18

class CPatternHandler : public CSectionHandler
{
private:
	CGraphic* m_pgBackground;

	CGraphicAlpha *m_pgaHit,*m_pgaAccent,*m_pgaMouseOver;

	CGraphic *m_pgHitHelp, *m_pgTripletHelp;

	CGraphic * m_pgHelp;
	
	int GetHScroll();
	BOOL IsFineGrid();

	BOOL InPatternArea(int iX,int iY);
	BOOL InValidPattern(int iX,int iY);

	int GetModifiedLineNumber(int iScreenLine);

	CLine& Line(int iY);
	CBeat& Beat(CLine& line,int iX);
	CBeat* TripletStart(
		CLine& l,int iX,int iLevel,
		CBeat* &pbDisplay1,CBeat* &pbDisplay2,CBeat* &pbDisplay3);
	void SetHelp(CGraphic* pgHelp);
	void UpdateScrollbars();
	void UpdateDrumsSection();
	void UpdatePatternValue();

	int m_iPattern;
	CBeat* m_pbMouseOver;
	CBeat* m_pbTriplet1, *m_pbTriplet2,*m_pbTriplet3;

	BOOL m_bTripletMode;

	CPattern& GetPattern() { return GetSong()->GetPattern(m_iPattern); }

	void UpdateTimeSignature(CPattern& p,int iTimeSignature);

	int GetFullDisplayLines() { return 16+(GetHeightOffset()/17); }
	int GetPartialDisplayLines() { return 16+((GetHeightOffset()+16)/17); }

public:
	void RepaintPattern() { Repaint(PATTERNX,PATTERNY,PATTERNWIDTH+GetWidthOffset(),PATTERNHEIGHT+GetHeightOffset()); }

	virtual int GetWidth() { return 456; }
	virtual int GetHeight() { return 341; }
	virtual int GetSplitY() { return 100; }
	virtual int GetSplitX() { return 143; }

	CPatternHandler();
	virtual ~CPatternHandler();

	// Create all the controls for this section
	virtual void Create(CLeafDrumsView* pView,int iX,int iY);

	// Draw this section onto the specified graphic
	virtual void Draw(CGraphicOnscreen& goTarget);

	// Command messages
	virtual void OnHScroll(int iID,int iValue);
	virtual void OnVScroll(int iID,int iValue);
	virtual void OnButton(int iID);
	virtual void OnMouseMove(int iX,int iY);
	virtual void OnMouseOut();
	virtual void OnLButtonDown(int iX,int iY);
	virtual void OnRButtonDown(int iX,int iY);
	virtual void OnSelchange(int iID,int iValue);
	virtual void OnCancelMode();

	int GetVScrollPos();
	void SetPatternIndex(int iIndex);
	int GetPatternIndex() { return m_iPattern; }

	// Clear on new song
	virtual void NewSong();

	virtual void InformResize(int iPreviousWOffset,int iPreviousHOffset);
};

#endif // PATTERNHANDLER_H
