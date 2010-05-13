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
#ifndef GRAPHICOPTION_H
#define GRAPHICOPTION_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "Graphic.h"
#include "GraphicAlpha.h"
#include "GraphicFont.h"

class CGraphicOption : public CWnd
{
// Construction
public:
	CGraphicOption(const BOOL fAutoDelete=FALSE);

// Attributes
public:

// Operations
public:
	virtual int GetMin()=0;
	virtual int GetMax()=0;
	virtual CString GetText(int iPos)=0;

	// Get or set the currently selected/shown item
	int GetSel();
	void SetSel(const int iIndex,const BOOL fNotify=FALSE);
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGraphicOption)
	public:
	virtual BOOL Create(CWnd* pwndParent,UINT nID,int iX,int iY,int iW,int iH,const char* strFont, int iPointSize, COLORREF clrText,CGraphic* pgBackground, int iXBG,int iYBG, const char* pstrType,int iBaselineAdjust=0,BOOL fShow=TRUE,BOOL bAntiAliased=TRUE);
	protected:
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGraphicOption();

	// Generated message map functions
protected:
	//{{AFX_MSG(CGraphicOption)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnCancelMode();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG
	afx_msg void OnTimer(UINT nIDEvent);
	DECLARE_MESSAGE_MAP()

	// Font and sizes
	CGraphicFont* m_pfont;
	COLORREF m_clrText;
	int m_iButtonHeight,m_iPartX,m_iButtonX;
	int m_iBaselineAdjust;

	// Standard scrollbar arrow graphics
	CGraphic* m_pgBackground;
	CGraphicAlpha *m_pgaUpUp, *m_pgaUpDown, *m_pgaUpOver;
	CGraphicAlpha *m_pgaDownUp, *m_pgaDownDown, *m_pgaDownOver;
	CGraphicAlpha *m_pgaNormalMid;

	// State
	int m_iPos;
	BOOL m_fCaptured,m_fOverLeft,m_fDownLeft,m_fOverRight,m_fDownRight;
	BOOL m_fAutoDelete;

	// Timer
	int m_iTick,m_iTimerDirection;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // GRAPHICOPTION_H
