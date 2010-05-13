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
#if !defined(AFX_GRAPHICHSCROLL_H__EBA3AF00_6B59_11D2_B7F2_9D51758AD573__INCLUDED_)
#define AFX_GRAPHICHSCROLL_H__EBA3AF00_6B59_11D2_B7F2_9D51758AD573__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "Graphic.h"
#include "GraphicAlpha.h"
#include "GraphicScrollbar.h"

class CGraphicHScroll : public CGraphicScrollbar
{
// Construction
public:
	CGraphicHScroll(BOOL fAutoDelete=FALSE);

// Attributes
public:

// Operations
public:
	void SetWidth(int iPixels,int iBackgroundSplitX);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGraphicHScroll)
	public:
	virtual BOOL Create(CWnd* pwndParent,UINT nID,int iX,int iY,int iW,CGraphic* pgBackground, int iXBG,int iYBG, const char* pstrType,BOOL fShow=TRUE);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGraphicHScroll();

	// Generated message map functions
protected:
	//{{AFX_MSG(CGraphicHScroll)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	int m_iHeight,m_iPartX,m_iButtonX,m_iStartX;
	int m_iOriginalWidth;
	void MoveThumb(BOOL bApplyMovement);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRAPHICHSCROLL_H__EBA3AF00_6B59_11D2_B7F2_9D51758AD573__INCLUDED_)
