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
#include "GraphicScrollbar.h"
#include "PaletteDC.h"

#include "GraphicOnscreen.h"
#include "ResourceData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGraphicScrollbar

CGraphicScrollbar::CGraphicScrollbar(BOOL fAutoDelete)
{
	// Clear variables
	m_iMin=0; m_iMax=10; m_iPos=4; m_iPageSize=3; m_iStep=1; m_iSnap=1;
	m_pglList=NULL;

	m_fCaptured=m_fOverTop=m_fOverBottom=m_fDownTop=m_fDownBottom=FALSE;
	m_fThumbDrag=m_fPageTop=m_fPageBottom=FALSE;

	// Null background graphic
	m_pgBackground=NULL;
	m_pgaUpUp=NULL;
	m_pgaUpDown=NULL;
	m_pgaUpOver=NULL;
	m_pgaDownUp=NULL;
	m_pgaDownDown=NULL;
	m_pgaDownOver=NULL;
	m_pgaThumbTop=NULL;
	m_pgaThumbBottom=NULL;
	m_pgaThumbMid=NULL;
	m_pgaNormalMid=NULL;

	m_fAutoDelete=fAutoDelete;
}

CGraphicScrollbar::~CGraphicScrollbar()
{
	// Delete background
	if(m_pgBackground)
		delete m_pgBackground;
	if(m_pgaUpUp)
		delete m_pgaUpUp;
	if(m_pgaUpDown)
		delete m_pgaUpDown;
	if(m_pgaUpOver)
		delete m_pgaUpOver;
	if(m_pgaDownUp)
		delete m_pgaDownUp;
	if(m_pgaDownDown)
		delete m_pgaDownDown;
	if(m_pgaDownOver)
		delete m_pgaDownOver;
	if(m_pgaThumbTop)
		delete m_pgaThumbTop;
	if(m_pgaThumbBottom)
		delete m_pgaThumbBottom;
	if(m_pgaThumbMid)
		delete m_pgaThumbMid;
	if(m_pgaNormalMid)
		delete m_pgaNormalMid;
}


BEGIN_MESSAGE_MAP(CGraphicScrollbar, CWnd)
	//{{AFX_MSG_MAP(CGraphicScrollbar)
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_SETCURSOR()
	ON_WM_CANCELMODE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CGraphicScrollbar message handlers

BOOL CGraphicScrollbar::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
}

void CGraphicScrollbar::OnTimer(UINT nIDEvent) 
{
	m_iTick++;
	if(m_iTick>TIMERTICK) MoveThumb(TRUE);
	
	CWnd::OnTimer(nIDEvent);
}

void CGraphicScrollbar::PostNcDestroy() 
{
	CWnd::PostNcDestroy();

	if(m_fAutoDelete) delete this;
}

void CGraphicScrollbar::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if(m_fDownTop)
	{
		if(m_fOverTop)
		{
			// Repaint to "over" status
			m_fDownTop=FALSE;
			m_fOverTop=FALSE;
			OnMouseMove(nFlags,point);
		}

		KillTimer(1);
	}
	if(m_fDownBottom)
	{
		if(m_fOverBottom)
		{
			// Repaint to "over" status
			m_fDownBottom=FALSE;
			m_fOverBottom=FALSE;
			OnMouseMove(nFlags,point);
		}

		KillTimer(1);
	}
	if(m_fThumbDrag)
	{
		m_fThumbDrag=FALSE;
	}
	if(m_fPageTop || m_fPageBottom)
	{
		m_fPageTop=m_fPageBottom=FALSE;
		KillTimer(1);
	}

	// Release capture
	if(m_fCaptured && !m_fOverTop && !m_fOverBottom)
	{
		m_fCaptured=FALSE;
		ReleaseCapture();
	}
	
	CWnd::OnLButtonUp(nFlags, point);
}

void CGraphicScrollbar::SetRange(int iMin,int iMax,int iPage)
{ 
	m_iMin=iMin; m_iMax=iMax; m_iPageSize=iPage; 
	if(m_iMax<m_iMin) m_iMax=m_iMin;
	if(m_iPos<m_iMin) m_iPos=m_iMin;
	else if(m_iPos>m_iMax) m_iPos=m_iMax;

	// If we're in a state where it could be repainted, repaint
	if(m_pgBackground)
		InvalidateRect(NULL,FALSE);
}

void CGraphicScrollbar::SetPos(int iPos)
{
	ASSERT(m_iPos>=m_iMin);
	ASSERT(m_iPos<=m_iMax);
	m_iPos=iPos;

	// If we're in a state where it could be repainted, repaint
	if(m_pgBackground)
		InvalidateRect(NULL,FALSE);
}

void CGraphicScrollbar::SetStep(int iStep)
{
	ASSERT(iStep>0);
	m_iStep=iStep;
}

void CGraphicScrollbar::SetSnap(int iMultiple)
{
	ASSERT(iMultiple>=1);
	m_iSnap=iMultiple;
}

void CGraphicScrollbar::SetListbox(CGraphicListbox* pglList)
{
	m_pglList=pglList;
}


// Convert double-clicks to a normal button-down
void CGraphicScrollbar::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	SendMessage(WM_LBUTTONDOWN,nFlags,MAKELONG(point.x,point.y));
}

BOOL CGraphicScrollbar::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	::SetCursor(::LoadCursor(NULL,IDC_ARROW));
	return TRUE;
}

void CGraphicScrollbar::OnCancelMode()
{
	CPoint pt;
	OnLButtonUp(0,pt);
	if(m_fCaptured)
	{
		m_fCaptured=FALSE;
		ReleaseCapture();

		m_fCaptured=m_fOverTop=m_fOverBottom=m_fDownTop=m_fDownBottom=FALSE;
		m_fThumbDrag=m_fPageTop=m_fPageBottom=FALSE;
	}
}
