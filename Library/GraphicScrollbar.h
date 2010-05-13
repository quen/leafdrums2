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
#if !defined(AFX_GRAPHICSCROLLBAR_H__8C5C0A02_6B55_11D2_B7F2_9D51758AD573__INCLUDED_)
#define AFX_GRAPHICSCROLLBAR_H__8C5C0A02_6B55_11D2_B7F2_9D51758AD573__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#define TIMERDELAY	100	// Delay between timer pulses (which move 1)
#define TIMERTICK	4	// # of ticks to wait before "autorepeat" starts

#include "Graphic.h"
#include "GraphicAlpha.h"

class CGraphicListbox;

class CGraphicScrollbar : public CWnd
{
// Construction
public:
	CGraphicScrollbar(BOOL fAutoDelete);

// Attributes
public:

// Operations
public:
	void SetRange(int iMin,int iMax,int iPage);

	int GetMin() { return m_iMin; }
	int GetMax() { return m_iMax; }
	int GetPageSize() { return m_iPageSize; }

	void SetPos(int iPos);
	int GetPos() { return m_iPos; }

	void SetStep(int iAmount);
	int GetStep() { return m_iStep; }

	void SetSnap(int iMultiple=1);

	void SetListbox(CGraphicListbox* pglList);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGraphicScrollbar)
	protected:
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGraphicScrollbar();

	// Generated message map functions
protected:
	//{{AFX_MSG(CGraphicScrollbar)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnCancelMode();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	virtual void MoveThumb(BOOL bApplyMovement)=0;

	// Background graphic used for transparency
	CGraphic* m_pgBackground;

	// Scrollbar information
	int m_iMin,m_iMax,m_iPageSize,m_iPos,m_iStep,m_iSnap;
	
	CGraphicListbox* m_pglList;

	// State information
	BOOL m_fCaptured;
	BOOL m_fOverTop,m_fOverBottom,m_fDownTop,m_fDownBottom;
	int m_iTick;
	BOOL m_fThumbDrag;
	int m_iStartPos;
	BOOL m_fPageTop,m_fPageBottom;
	
	// Standard scrollbar graphics
	CGraphicAlpha *m_pgaUpUp, *m_pgaUpDown, *m_pgaUpOver;
	CGraphicAlpha *m_pgaDownUp, *m_pgaDownDown, *m_pgaDownOver;
	CGraphicAlpha *m_pgaThumbTop, *m_pgaThumbBottom, 
		*m_pgaThumbMid, *m_pgaNormalMid;

	BOOL m_fAutoDelete;
};

#include "GraphicListBox.h"

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRAPHICSCROLLBAR_H__8C5C0A02_6B55_11D2_B7F2_9D51758AD573__INCLUDED_)
