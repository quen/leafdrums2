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
#ifndef GRAPHICBUTTON_H
#define GRAPHICBUTTON_H

#include "GraphicFont.h"
#include "Graphic.h"
#include "GraphicAlpha.h"
#include "GraphicalProducer.h"

/////////////////////////////////////////////////////////////////////////////
// CGraphicButton window

class CGraphicButton : public CGraphicalProducer
{
// Construction
public:
	CGraphicButton(BOOL fAutoDelete=FALSE);

// Attributes
public:

// Operations
public:
	void SetText(const char* strText);

	void SetPressed(BOOL fPressed=TRUE);
	BOOL Pressed();

	void SetFont(const char* strFont,int iPointSize,BOOL bAntiAliased);
	void OffsetText(int iBaselineOffset);

	void SetGraphics(CGraphic* pgNormal,CGraphic* pgOver,CGraphic* pgPressed);
	void SetTextColours(COLORREF clrNormal,COLORREF clrOver,COLORREF clrPressed);

	void SetRadio(CGraphicButton* pgbLastInGroup=NULL);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGraphicButton)
	public:
	virtual BOOL Create(CWnd* pParentWnd, UINT nID, const char* strText,int iX,int iY,int iW,int iH,const char* strFont,int iPointSize,CGraphic* pgBackground, int iXBG,int iYBG,COLORREF colour,CGraphic* pgBackgroundo, int iXBGo,int iYBGo,COLORREF colouro,CGraphic* pgBackgroundd, int iXBGd,int iYBGd,COLORREF colourd,CGraphicButton* pgbLastInGroup=NULL);
	protected:
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL
public:
	virtual BOOL Create(CWnd* pParentWnd, UINT nID, 
		const char* strText,int iX,int iY,int iW,int iH,
		const char* strFont,int iPointSize,
		CGraphic* pgBackground, int iXBG,int iYBG,
		CGraphicAlpha* pgaNorm, int iXBGn,int iYBGn,COLORREF colour,
		CGraphicAlpha* pgaOver, int iXBGo,int iYBGo,COLORREF colouro,
		CGraphicAlpha* pgaDown, int iXBGd,int iYBGd,COLORREF colourd,
		CGraphicButton* pgbLastInGroup=NULL);

// Implementation
public:
	virtual ~CGraphicButton();

	// Generated message map functions
protected:
	//{{AFX_MSG(CGraphicButton)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnCancelMode();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnShowWindow( BOOL bShow, UINT nStatus );
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	virtual void PaintTo(CGraphic* pgTarget,int iXStart,int iYStart);

	// Grouped buttons (work like radio buttons)
	void SetNextButton(CGraphicButton* pgbNext) { m_pgbNext= pgbNext; }
	void OffForwards();
	void OffBackwards();

	CGraphicButton* m_pgbPrev, * m_pgbNext;

	// Button text
	CString m_strText;

	// Font used for text
	CGraphicFont* m_pfont;

	// Graphics for button
	CGraphic *m_pgBackground, *m_pgBackgroundOver, *m_pgBackgroundDown;

	// Colours used for text
	COLORREF m_clrNormal,m_clrOver,m_clrDown;

	// Position of text
	int m_iTextPos;

	// State
	BOOL m_fCaptured,m_fOver,m_fDown;

	BOOL m_fAutoDelete;

};

/////////////////////////////////////////////////////////////////////////////
#endif // GRAPHICBUTTON_H
