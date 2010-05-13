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
#ifndef GRAPHICLISTBOX_H
#define GRAPHICLISTBOX_H

#include "Graphic.h"
#include "GraphicFont.h"
#include "GraphicScrollbar.h"

#define GRAPHICLISTBOX_MAXITEMS	500

/////////////////////////////////////////////////////////////////////////////
// CGraphicListbox window

class CGraphicListbox : public CWnd
{
// Construction
public:
	CGraphicListbox(BOOL fAutoDelete=FALSE);

// Attributes
public:

// Operations
public:
	void AddItem(const char* strText);
	void SetItem(int iPos,const char* strText);
	BOOL DelItem(const char* strText);
	void Clear();

	void SetScrollbar(CGraphicScrollbar* pgsScrollbar);

	void SetPos(int iPos);
	int GetPos();

	int SetSel(const CString strSel);
	BOOL GetSel(CString &strSel);
	void SetSel(int iPos, BOOL fNotify=FALSE);
	int GetSel();

	int Count() { return m_iCount; }
	

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGraphicListbox)
	public:
	virtual BOOL Create(CWnd* pParentWnd,UINT nID,int iX,int iY,int iW,int iH,const char* strFont,int iPointSize,int iLineHeight,CGraphic* pgBackground, int iXBG,int iYBG,COLORREF colour,CGraphic* pgBackgroundo, int iXBGo,int iYBGo,COLORREF colouro,CGraphic* pgBackgroundd, int iXBGd,int iYBGd,COLORREF colourd,CGraphicScrollbar* pgsScrollbar=NULL,BOOL fSorted=TRUE);
	protected:
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGraphicListbox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CGraphicListbox)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()


	// Background graphics for box
	CGraphic *m_pgBackground, *m_pgBackgroundOver, *m_pgBackgroundDown;

	// Colours used for text
	COLORREF m_clrNormal,m_clrOver,m_clrDown;

	// Strings
	int m_iCount;
	CString m_strText[GRAPHICLISTBOX_MAXITEMS];
	BOOL m_fSorted;

	// Font, size info, etc.
	CGraphicFont* m_pfont;
	int m_iLineHeight,m_iLineOffset;
	int m_iSizeLines,m_iViewLines;

	// State
	int m_iPos;
	BOOL m_fCaptured,m_fOver,m_fDown;
	int m_iHighlight,m_iSelected;

	CGraphicScrollbar* m_pgsScrollbar;

	BOOL m_fAutoDelete;
};

/////////////////////////////////////////////////////////////////////////////
#endif // GRAPHICLISTBOX_H
