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
#include "stdafx.h"
#include "GraphicCheckbox.h"

#include "PaletteDC.h"
#include "GraphicOnscreen.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGraphicCheckbox

CGraphicCheckbox::CGraphicCheckbox(BOOL fAutoDelete)
{
	m_pgBackground=m_pgBackgroundDownNorm=m_pgBackgroundDownChecked=m_pgBackgroundChecked=NULL;
	m_pfont=NULL;

	m_fCaptured=m_fOver=m_fDown=m_fChecked=FALSE;

	m_fAutoDelete=fAutoDelete;
}

CGraphicCheckbox::~CGraphicCheckbox()
{
	if(m_pfont) delete m_pfont;
	if(m_pgBackground) delete m_pgBackground;
	if(m_pgBackgroundDownNorm) delete m_pgBackgroundDownNorm;
	if(m_pgBackgroundDownChecked) delete m_pgBackgroundDownChecked;
	if(m_pgBackgroundChecked) delete m_pgBackgroundChecked;
}


BEGIN_MESSAGE_MAP(CGraphicCheckbox, CWnd)
	//{{AFX_MSG_MAP(CGraphicCheckbox)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
	ON_WM_CANCELMODE()
	ON_WM_SETCURSOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CGraphicCheckbox message handlers

BOOL CGraphicCheckbox::Create(CWnd* pParentWnd, UINT nID, 
	const char* strText,int iX,int iY,int iW,int iH,
	const char* strFont,int iPointSize,
	CGraphic* pgBackground, int iXBG,int iYBG,COLORREF colour,
	CGraphic* pgBackgrounddn, int iXBGdn,int iYBGdn,COLORREF colourdn,
	CGraphic* pgBackgrounddc, int iXBGdc,int iYBGdc,COLORREF colourdc,
	CGraphic* pgBackgroundc, int iXBGc,int iYBGc,COLORREF colourc)
{
	m_strText=strText;

	// Create font
	m_pfont=new CGraphicFont(strFont,iPointSize);

	// Obtain the background graphics
	m_pgBackground=new CGraphic(iW,iH);
	pgBackground->Copy(m_pgBackground,0,0,iXBG,iYBG,iW,iH);
	m_pgBackgroundChecked=new CGraphic(iW,iH);
	pgBackgroundc->Copy(m_pgBackgroundChecked,0,0,iXBGc,iYBGc,iW,iH);
	m_pgBackgroundDownNorm=new CGraphic(iW,iH);
	pgBackgrounddn->Copy(m_pgBackgroundDownNorm,0,0,iXBGdn,iYBGdn,iW,iH);
	m_pgBackgroundDownChecked=new CGraphic(iW,iH);
	pgBackgrounddc->Copy(m_pgBackgroundDownChecked,0,0,iXBGdc,iYBGdc,iW,iH);
	
	// Set the colours etc.
	m_clrNormal=colour;
	m_clrChecked=colourc;
	m_clrDownNorm=colourdn;
	m_clrDownChecked=colourdc;

	// Work out the positioning
	m_iTextPos=(iH-m_pfont->Height())/2;
	
	// Make up the creation rectangle
	CRect rect(iX,iY,iX+iW,iY+iH);

	return CWnd::Create(NULL, "CGraphicCheckbox", 
		WS_CHILD|WS_VISIBLE, rect, pParentWnd, nID);
}

BOOL CGraphicCheckbox::Create(CWnd* pParentWnd, UINT nID, 
	const char* strText,int iX,int iY,int iW,int iH,
	const char* strFont,int iPointSize,
	CGraphic* pgBackground, int iXBG,int iYBG,
	CGraphicAlpha* pgaNorm, int iXBGn,int iYBGn,COLORREF colour,
	CGraphicAlpha* pgaDownNorm, int iXBGdn,int iYBGdn,COLORREF colourdn,
	CGraphicAlpha* pgaDownChecked, int iXBGdc,int iYBGdc,COLORREF colourdc,
	CGraphicAlpha* pgaChecked, int iXBGc,int iYBGc,COLORREF colourc)
{
	m_strText=strText;

	// Create font
	m_pfont=new CGraphicFont(strFont,iPointSize);

	// Obtain the background graphics
	m_pgBackground=new CGraphic(iW,iH);
	pgBackground->Copy(m_pgBackground,0,0,iXBG,iYBG,iW,iH);
	pgaNorm->Copy(m_pgBackground,0,0,iXBGn,iYBGn,iW,iH);

	m_pgBackgroundChecked=new CGraphic(iW,iH);
	pgBackground->Copy(m_pgBackgroundChecked,0,0,iXBG,iYBG,iW,iH);
	pgaChecked->Copy(m_pgBackgroundChecked,0,0,iXBGc,iYBGc,iW,iH);

	m_pgBackgroundDownNorm=new CGraphic(iW,iH);
	pgBackground->Copy(m_pgBackgroundDownNorm,0,0,iXBG,iYBG,iW,iH);
	pgaDownNorm->Copy(m_pgBackgroundDownNorm,0,0,iXBGdn,iYBGdn,iW,iH);

	m_pgBackgroundDownChecked=new CGraphic(iW,iH);
	pgBackground->Copy(m_pgBackgroundDownChecked,0,0,iXBG,iYBG,iW,iH);
	pgaDownChecked->Copy(m_pgBackgroundDownChecked,0,0,iXBGdc,iYBGdc,iW,iH);
	
	// Set the colours etc.
	m_clrNormal=colour;
	m_clrChecked=colourc;
	m_clrDownNorm=colourdn;
	m_clrDownChecked=colourdc;

	// Work out the positioning
	m_iTextPos=(iH-m_pfont->Height())/2;
	
	// Make up the creation rectangle
	CRect rect(iX,iY,iX+iW,iY+iH);

	return CWnd::Create(NULL, "CGraphicCheckbox", 
		WS_CHILD|WS_VISIBLE, rect, pParentWnd, nID);
}

void CGraphicCheckbox::OnLButtonDown(UINT nFlags, CPoint point) 
{
	m_fDown=TRUE;
	m_fOver=TRUE;

	ASSERT(!m_fCaptured);
	m_fCaptured=TRUE;
	SetCapture();

	// Repaint down
	GraphicRepaint();
	
	CWnd::OnLButtonDown(nFlags, point);
}

void CGraphicCheckbox::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// Uncapture
	if(m_fCaptured)
	{
		m_fCaptured=FALSE;
		ReleaseCapture();
	}

	// Set button not-down
	m_fDown=FALSE;

	if(m_fOver)
	{
		// Button was actually pressed, then
		m_fChecked=!m_fChecked;
		GraphicRepaint();

		// Inform parent
		GetParent()->SendMessage(WM_COMMAND,
			BN_CLICKED<<16 | GetDlgCtrlID(),(LPARAM)GetSafeHwnd());
	}

	CWnd::OnLButtonUp(nFlags, point);
}

void CGraphicCheckbox::OnMouseMove(UINT nFlags, CPoint point) 
{
	CRect rect;
	GetClientRect(&rect);
	BOOL fIn=rect.PtInRect(point);

	if(m_fDown)
	{
		if(m_fOver && !fIn)
		{
			m_fOver=FALSE;

			// Repaint normal
			GraphicRepaint();
		}
		else if(!m_fOver && fIn)
		{
			m_fOver=TRUE;

			// Repaint down
			GraphicRepaint();
		}
	}
	
	CWnd::OnMouseMove(nFlags, point);
}

void CGraphicCheckbox::PaintTo(CGraphic* pgTarget,int iXStart,int iYStart)
{
	CGraphic* pg;
	COLORREF clr;

	if(m_fDown && m_fOver)
	{
		if(m_fChecked)
		{
			pg=m_pgBackgroundDownChecked;
			clr=m_clrDownChecked;
		}
		else
		{
			pg=m_pgBackgroundDownNorm;
			clr=m_clrDownNorm;
		}
	}
	else
	{
		if(m_fChecked)
		{
			pg=m_pgBackgroundChecked;
			clr=m_clrChecked;
		}
		else
		{
			pg=m_pgBackground;
			clr=m_clrNormal;
		}
	}

	pg->Copy(pgTarget,iXStart,iYStart);
	m_pfont->Draw(pg,2+iXStart,m_iTextPos+iYStart,m_strText,clr);
}

void CGraphicCheckbox::OnPaint() 
{
	CPaletteDC dc(this); // device context for painting
	if(HaveOutputSink()) return; // Just mark valid if we're painting on output-sink's request

	// Create graphic to calculate display in, init with background
	CGraphicOnscreen gr(m_pgBackground->Width(),m_pgBackground->Height());

	// Paint and copy to screen
	PaintTo(&gr,0,0);
	gr.Draw(&dc,0,0);
}

void CGraphicCheckbox::SetText(const char* strText)
{
	m_strText=strText;
	GraphicRepaint();
}

void CGraphicCheckbox::SetChecked(BOOL fChecked)
{
	// If mouse is captured, release it in order not to 
	// mess things up
	if(m_fDown) m_fDown=FALSE;
	if(m_fCaptured)
	{
		ReleaseCapture();
		m_fCaptured=FALSE;
	}

	// Set state and redraw
	m_fChecked=fChecked;
	GraphicRepaint();
}

BOOL CGraphicCheckbox::Checked()
{
	return m_fChecked;
}

void CGraphicCheckbox::PostNcDestroy() 
{
	CWnd::PostNcDestroy();

	if(m_fAutoDelete) delete this;
}

void CGraphicCheckbox::OnCancelMode() 
{
	if(m_fCaptured)
	{
		m_fCaptured=FALSE;
		ReleaseCapture();
	}

	if(m_fOver || m_fDown)
	{
		m_fOver=m_fDown=FALSE;
		GraphicRepaint();
	}
}

BOOL CGraphicCheckbox::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	::SetCursor(::LoadCursor(NULL,IDC_ARROW));
	return TRUE;
}