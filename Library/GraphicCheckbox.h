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
#ifndef GRAPHICCHECKBOX_H
#define GRAPHICCHECKBOX_H

#include "GraphicFont.h"
#include "Graphic.h"
#include "GraphicAlpha.h"
#include "GraphicalProducer.h"

/////////////////////////////////////////////////////////////////////////////
// CGraphicCheckbox window

class CGraphicCheckbox : public CGraphicalProducer
{
// Construction
public:
	CGraphicCheckbox(BOOL fAutoDelete=FALSE);

// Attributes
public:

// Operations
public:
	void SetText(const char* strText);

	void SetChecked(BOOL fChecked=TRUE);
	BOOL Checked();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGraphicCheckbox)
	public:
	protected:
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL
public:
	virtual BOOL Create(CWnd* pParentWnd, UINT nID, 
		const char* strText,int iX,int iY,int iW,int iH,
		const char* strFont,int iPointSize,
		CGraphic* pgBackground, int iXBG,int iYBG,COLORREF colour,
		CGraphic* pgBackgrounddn, int iXBGdn,int iYBGdn,COLORREF colourdn,
		CGraphic* pgBackgrounddc, int iXBGdc,int iYBGdc,COLORREF colourdc,
		CGraphic* pgBackgroundc, int iXBGc,int iYBGc,COLORREF colourc);

	virtual BOOL Create(CWnd* pParentWnd, UINT nID, 
		const char* strText,int iX,int iY,int iW,int iH,
		const char* strFont,int iPointSize,
		CGraphic* pgBackground, int iXBG,int iYBG,
		CGraphicAlpha* pgaNorm, int iXBGn,int iYBGn,COLORREF colour,
		CGraphicAlpha* pgaDownNorm, int iXBGon,int iYBGon,COLORREF colouron,
		CGraphicAlpha* pgaDownChecked, int iXBGoc,int iYBGoc,COLORREF colouroc,
		CGraphicAlpha* pgaChecked, int iXBGd,int iYBGd,COLORREF colourd);

// Implementation
public:
	virtual ~CGraphicCheckbox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CGraphicCheckbox)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnCancelMode();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	virtual void PaintTo(CGraphic* pgTarget,int iXStart,int iYStart);

	// Button text
	CString m_strText;

	// Font used for text
	CGraphicFont* m_pfont;

	// Graphics for button
	CGraphic *m_pgBackground, *m_pgBackgroundDownNorm, *m_pgBackgroundDownChecked, *m_pgBackgroundChecked;

	// Colours used for text
	COLORREF m_clrNormal,m_clrDownNorm,m_clrDownChecked,m_clrChecked;

	// Position of text
	int m_iTextPos;

	// State
	BOOL m_fCaptured,m_fDown,m_fChecked,m_fOver;

	BOOL m_fAutoDelete;

};

/////////////////////////////////////////////////////////////////////////////
#endif // GRAPHICCHECKBOX_H
