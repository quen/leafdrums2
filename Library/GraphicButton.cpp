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
#include "GraphicButton.h"

#include "PaletteDC.h"
#include "GraphicOnscreen.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGraphicButton

CGraphicButton::CGraphicButton(BOOL fAutoDelete)
{
	m_pgbPrev=m_pgbNext=NULL;
	m_pgBackground=m_pgBackgroundOver=m_pgBackgroundDown=NULL;
	m_pfont=NULL;

	m_fCaptured=m_fOver=m_fDown=FALSE;

	m_fAutoDelete=fAutoDelete;
}

CGraphicButton::~CGraphicButton()
{
	if(m_pfont) delete m_pfont;
	if(m_pgBackground) delete m_pgBackground;
	if(m_pgBackgroundDown) delete m_pgBackgroundDown;
	if(m_pgBackgroundOver) delete m_pgBackgroundOver;
}


BEGIN_MESSAGE_MAP(CGraphicButton, CWnd)
	//{{AFX_MSG_MAP(CGraphicButton)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
	ON_WM_CANCELMODE()
	ON_WM_SETCURSOR()
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CGraphicButton message handlers

BOOL CGraphicButton::Create(CWnd* pParentWnd, UINT nID, 
	const char* strText,int iX,int iY,int iW,int iH,
	const char* strFont,int iPointSize,
	CGraphic* pgBackground, int iXBG,int iYBG,COLORREF colour,
	CGraphic* pgBackgroundo, int iXBGo,int iYBGo,COLORREF colouro,
	CGraphic* pgBackgroundd, int iXBGd,int iYBGd,COLORREF colourd,
	CGraphicButton* pgbLastInGroup)
{
	m_strText=strText;

	m_pgbPrev=pgbLastInGroup;
	if(m_pgbPrev!=NULL)
		m_pgbPrev->SetNextButton(this);

	// Create font
	m_pfont=new CGraphicFont(strFont,iPointSize);

	// Obtain the background graphics
	m_pgBackground=new CGraphic(iW,iH);
	pgBackground->Copy(m_pgBackground,0,0,iXBG,iYBG,iW,iH);
	m_pgBackgroundOver=new CGraphic(iW,iH);
	pgBackgroundo->Copy(m_pgBackgroundOver,0,0,iXBGo,iYBGo,iW,iH);
	m_pgBackgroundDown=new CGraphic(iW,iH);
	pgBackgroundd->Copy(m_pgBackgroundDown,0,0,iXBGd,iYBGd,iW,iH);
	
	// Set the colours etc.
	m_clrNormal=colour;
	m_clrOver=colouro;
	m_clrDown=colourd;

	// Work out the positioning
	m_iTextPos=(iH-m_pfont->Height())/2;
	
	// Make up the creation rectangle
	CRect rect(iX,iY,iX+iW,iY+iH);

	return CWnd::Create(NULL, "CGraphicButton", 
		WS_CHILD|WS_VISIBLE, rect, pParentWnd, nID);
}

void CGraphicButton::SetFont(const char* strFont,int iPointSize,BOOL bAntiAliased)
{
	if(m_pfont) delete m_pfont;
	m_pfont=new CGraphicFont(strFont,iPointSize,bAntiAliased);

	// Work out text positioning
	CRect r;
	GetClientRect(&r);
	m_iTextPos=(r.Height()-m_pfont->Height())/2;
}

void CGraphicButton::SetGraphics(CGraphic* pgNormal,CGraphic* pgOver,CGraphic* pgPressed)
{
	// Obtain the background graphics
	pgNormal->Copy(m_pgBackground);
	pgOver->Copy(m_pgBackgroundOver);
	pgPressed->Copy(m_pgBackgroundDown);

	// Repaint some time
	GraphicRepaint();
}

void CGraphicButton::SetTextColours(COLORREF clrNormal,COLORREF clrOver,COLORREF clrPressed)
{
	// Update colours
	m_clrNormal=clrNormal;
	m_clrOver=clrOver;
	m_clrDown=clrPressed;

	// Repaint some time
	GraphicRepaint();
}

void CGraphicButton::OffsetText(int iBaselineOffset)
{
	m_iTextPos+=iBaselineOffset;
}


BOOL CGraphicButton::Create(CWnd* pParentWnd, UINT nID, 
	const char* strText,int iX,int iY,int iW,int iH,
	const char* strFont,int iPointSize,
	CGraphic* pgBackground, int iXBG,int iYBG,
	CGraphicAlpha* pgaNorm, int iXBGn,int iYBGn,COLORREF colour,
	CGraphicAlpha* pgaOver, int iXBGo,int iYBGo,COLORREF colouro,
	CGraphicAlpha* pgaDown, int iXBGd,int iYBGd,COLORREF colourd,
	CGraphicButton* pgbLastInGroup)
{
	m_strText=strText;

	m_pgbPrev=pgbLastInGroup;
	if(m_pgbPrev!=NULL)
		m_pgbPrev->SetNextButton(this);

	// Create font
	m_pfont=new CGraphicFont(strFont,iPointSize);

	// Obtain the background graphics
	m_pgBackground=new CGraphic(iW,iH);
	pgBackground->Copy(m_pgBackground,0,0,iXBG,iYBG,iW,iH);
	pgaNorm->Copy(m_pgBackground,0,0,iXBGn,iYBGn,iW,iH);

	m_pgBackgroundOver=new CGraphic(iW,iH);
	pgBackground->Copy(m_pgBackgroundOver,0,0,iXBG,iYBG,iW,iH);
	pgaOver->Copy(m_pgBackgroundOver,0,0,iXBGo,iYBGo,iW,iH);

	m_pgBackgroundDown=new CGraphic(iW,iH);
	pgBackground->Copy(m_pgBackgroundDown,0,0,iXBG,iYBG,iW,iH);
	pgaDown->Copy(m_pgBackgroundDown,0,0,iXBGd,iYBGd,iW,iH);
	
	// Set the colours etc.
	m_clrNormal=colour;
	m_clrOver=colouro;
	m_clrDown=colourd;

	// Work out the positioning
	m_iTextPos=(iH-m_pfont->Height())/2;
	
	// Make up the creation rectangle
	CRect rect(iX,iY,iX+iW,iY+iH);

	return CWnd::Create(NULL, "CGraphicButton", 
		WS_CHILD|WS_VISIBLE, rect, pParentWnd, nID);
}

void CGraphicButton::OnLButtonDown(UINT nFlags, CPoint point) 
{
	m_fDown=TRUE;

	// If this means we activate button from radio set...
	if(m_pgbPrev || m_pgbNext)
	{
		// Release capture, we don't need that any more
		if(m_fCaptured)
		{
			ReleaseCapture();
			m_fCaptured=FALSE;
		}

		m_fOver=FALSE;

		// Set this button down
		GraphicRepaint();

		// Tell other buttons to get lost
		if(m_pgbPrev) m_pgbPrev->OffBackwards();
		if(m_pgbNext) m_pgbNext->OffForwards();

		// Inform parent
		GetParent()->SendMessage(WM_COMMAND,
			BN_CLICKED<<16 | GetDlgCtrlID(),(LPARAM)GetSafeHwnd());
	}
	else	// Button is "on its own" and works on release etc.
	{
		// Should already be captured, but check
		if(!m_fCaptured)
		{
			m_fCaptured=TRUE;
			SetCapture();
		}

		// Repaint down
		GraphicRepaint();
	}
	
	CWnd::OnLButtonDown(nFlags, point);
}

void CGraphicButton::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// Uncapture
	if(m_fCaptured && !m_fOver)
	{
		m_fCaptured=FALSE;
		ReleaseCapture();
	}

	// Radio-set style buttons ignore mouseup
	if(m_pgbPrev || m_pgbNext) return;

	// If not actually down, ignore
	if(!m_fDown) return;

	// Set button not-down
	m_fDown=FALSE;

	// If not actually pressed, ignore
	if(!m_fOver) return;
	
	// If it's over, we need to repaint properly
	m_fOver=FALSE;
	OnMouseMove(nFlags,point);

	// Inform parent
	GetParent()->SendMessage(WM_COMMAND,
		BN_CLICKED<<16 | GetDlgCtrlID(),(LPARAM)GetSafeHwnd());
}

void CGraphicButton::OnMouseMove(UINT nFlags, CPoint point) 
{
	CRect rect;
	GetClientRect(&rect);
	BOOL fIn=rect.PtInRect(point);

	if(fIn)
	{
		CPoint pt2=point;
		ClientToScreen(&pt2);
		if(WindowFromPoint(pt2)!=this) fIn=FALSE;
	}

	if(!m_fDown)
	{
		if(m_fOver && !fIn)
		{
			m_fOver=FALSE;
			m_fCaptured=FALSE;
			ReleaseCapture();
			GraphicRepaint();
		}
		else if(!m_fOver && fIn)
		{
			m_fOver=TRUE;
			m_fCaptured=TRUE;
			SetCapture();
			GraphicRepaint();
		}
	}
	else	// The button is down
	{
		// If semipermanently down, don't do anything when over
		if(m_pgbPrev || m_pgbNext) return;

		// Otherwise this is a normal button, so check if in & draw
		if(m_fOver && !fIn)
		{
			m_fOver=FALSE;
			GraphicRepaint();
		}
		else if(!m_fOver && fIn)
		{
			m_fOver=TRUE;
			GraphicRepaint();
		}
	}
	
	CWnd::OnMouseMove(nFlags, point);
}

void CGraphicButton::OffForwards()
{
	if(m_fDown)
	{
		m_fDown=FALSE;
		GraphicRepaint();
	}
	
	if(m_pgbNext) m_pgbNext->OffForwards();
}

void CGraphicButton::OffBackwards()
{
	if(m_fDown)
	{
		m_fDown=FALSE;
		GraphicRepaint();
	}
	
	if(m_pgbPrev) m_pgbPrev->OffBackwards();
}

void CGraphicButton::PaintTo(CGraphic* pgTarget,int iXStart,int iYStart)
{
	CGraphic* pg;
	COLORREF clr;
	if(m_fDown)
	{
		if(m_fOver || (m_pgbPrev || m_pgbNext))
		{
			pg=m_pgBackgroundDown;
			clr=m_clrDown;
		}
		else
		{
			pg=m_pgBackground;
			clr=m_clrNormal;
		}
	}
	else
	{
		if(m_fOver)
		{
			pg=m_pgBackgroundOver;
			clr=m_clrOver;
		}
		else
		{
			pg=m_pgBackground;
			clr=m_clrNormal;
		}
	}
	
	pg->Copy(pgTarget,iXStart,iYStart);
	m_pfont->Draw(pgTarget,2+iXStart,m_iTextPos+iYStart,m_strText,clr);
}

void CGraphicButton::OnPaint() 
{
	CPaletteDC dc(this); // device context for painting
	if(HaveOutputSink()) return; // Just mark valid if we're painting on output-sink's request

	// Create graphic to calculate display in
	CGraphicOnscreen gr(m_pgBackground->Width(),m_pgBackground->Height());

	// Paint and copy to screen
	PaintTo(&gr,0,0);
	gr.Draw(&dc,0,0);
}

void CGraphicButton::SetText(const char* strText)
{
	m_strText=strText;
	GraphicRepaint();
}

void CGraphicButton::SetPressed(BOOL fPressed)
{
	// Only makes sense on a state-holding button
	ASSERT(m_pgbPrev || m_pgbNext);

	// If mouse is captured, release it in order not to 
	// mess things up
	if(m_fOver) m_fOver=FALSE;
	if(m_fCaptured)
	{
		ReleaseCapture();
		m_fCaptured=FALSE;
	}

	// Set state and redraw
	m_fDown=fPressed;
	GraphicRepaint();

	// Turn off other buttons
	if(m_pgbPrev) m_pgbPrev->OffBackwards();
	if(m_pgbNext) m_pgbNext->OffForwards();
}

BOOL CGraphicButton::Pressed()
{
	// Only makes sense on a state-holding button
	ASSERT(m_pgbPrev || m_pgbNext);

	return m_fDown;
}

void CGraphicButton::PostNcDestroy() 
{
	CWnd::PostNcDestroy();

	if(m_fAutoDelete) delete this;
}

void CGraphicButton::OnCancelMode() 
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

void CGraphicButton::SetRadio(CGraphicButton* pgbLastInGroup)
{
	m_pgbPrev=pgbLastInGroup;
	if(m_pgbPrev!=NULL)
		m_pgbPrev->SetNextButton(this);
	else
		m_pgbNext=NULL;
}

BOOL CGraphicButton::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	::SetCursor(::LoadCursor(NULL,IDC_ARROW));
	return TRUE;
}

void CGraphicButton::OnShowWindow( BOOL bShow, UINT nStatus )
{
	if(bShow)
	{
		// If shown, run mouseover if need be
		POINT pt;
		GetCursorPos(&pt);
		ScreenToClient(&pt);
		RECT r;
		GetClientRect(&r);
		if(::PtInRect(&r,pt))
		{
			OnMouseMove(-1, pt);
		}
	}
	else
	{
		// If hidden, cancel stuff
		OnCancelMode();
	}
}
