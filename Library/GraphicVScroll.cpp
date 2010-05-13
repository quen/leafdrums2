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
#include "GraphicVScroll.h"
#include "PaletteDC.h"

#include "GraphicOnscreen.h"
#include "ResourceData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGraphicVScroll

CGraphicVScroll::CGraphicVScroll(BOOL fAutoDelete) : CGraphicScrollbar(fAutoDelete)
{
}

CGraphicVScroll::~CGraphicVScroll()
{
}


BEGIN_MESSAGE_MAP(CGraphicVScroll, CGraphicScrollbar)
	//{{AFX_MSG_MAP(CGraphicVScroll)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CGraphicVScroll message handlers

BOOL CGraphicVScroll::Create(CWnd* pwndParent,UINT nID,int iX, int iY,int iH,
	CGraphic* pgBackground, int iXBG,int iYBG, const char* strType,BOOL fShow) 
{
	m_iOriginalHeight=iH;

	// Load the resource defining scrollbar type
	CResourceData res(strType,(char*)RT_RCDATA);

	int* piData=(int*)res.Data();
	ASSERT(piData[0]==0);	// 0 is a "type" value just in case 
	m_iWidth=piData[1];
	m_iButtonY=piData[2];
	m_iPartY=piData[3];

	// Obtain the background graphic
	m_pgBackground=new CGraphic(m_iWidth,iH);
	pgBackground->Copy(m_pgBackground,0,0,iXBG,iYBG,m_iWidth,iH);

	// Make up the creation rectangle
	CRect rect(iX,iY,iX+m_iWidth,iY+iH);

	// Allocate memory for the bitmaps
	m_pgaUpUp=new CGraphicAlpha(m_iWidth,m_iButtonY);
	m_pgaUpDown=new CGraphicAlpha(m_iWidth,m_iButtonY);
	m_pgaUpOver=new CGraphicAlpha(m_iWidth,m_iButtonY);
	m_pgaDownUp=new CGraphicAlpha(m_iWidth,m_iButtonY);
	m_pgaDownDown=new CGraphicAlpha(m_iWidth,m_iButtonY);
	m_pgaDownOver=new CGraphicAlpha(m_iWidth,m_iButtonY);
	m_pgaThumbTop=new CGraphicAlpha(m_iWidth,m_iPartY);
	m_pgaThumbBottom=new CGraphicAlpha(m_iWidth,m_iPartY);
	m_pgaThumbMid=new CGraphicAlpha(m_iWidth,m_iPartY);
	m_pgaNormalMid=new CGraphicAlpha(m_iWidth,m_iPartY);

	// Load graphics from resources given in the type file
	char* pbData=(char*)res.Data();
	pbData=&pbData[16];	// Skip numbers
	m_pgaUpUp->LoadResource(pbData);
	pbData=&pbData[20];
	m_pgaUpDown->LoadResource(pbData);
	pbData=&pbData[20];
	m_pgaUpOver->LoadResource(pbData);
	pbData=&pbData[20];
	if(*pbData==0)
	{
		*m_pgaDownUp=*m_pgaUpUp;
		m_pgaDownUp->FlipVertical();
	}
	else
		m_pgaDownUp->LoadResource(pbData);
	pbData=&pbData[20];
	if(*pbData==0)
	{
		*m_pgaDownDown=*m_pgaUpDown;
		m_pgaDownDown->FlipVertical();
	}
	else
		m_pgaDownDown->LoadResource(pbData);
	pbData=&pbData[20];
	if(*pbData==0)
	{
		*m_pgaDownOver=*m_pgaUpOver;
		m_pgaDownOver->FlipVertical();
	}
	else
		m_pgaDownOver->LoadResource(pbData);
	pbData=&pbData[20];
	m_pgaThumbTop->LoadResource(pbData);
	pbData=&pbData[20];
	if(*pbData==0)
	{
		*m_pgaThumbBottom=*m_pgaThumbTop;
		m_pgaThumbBottom->FlipVertical();
	}
	else
		m_pgaThumbBottom->LoadResource(pbData);
	pbData=&pbData[20];
	m_pgaThumbMid->LoadResource(pbData);
	pbData=&pbData[20];
	m_pgaNormalMid->LoadResource(pbData);
		
	// Create the window
	return CWnd::Create(NULL,"CGraphicVScroll",
		WS_CHILD| (fShow ? WS_VISIBLE : 0),
		rect,
		pwndParent,nID);
}

void CGraphicVScroll::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if(m_iMax==m_iMin) return;

	// Check thumb position
	int iSpace=m_pgBackground->Height()-2*m_iButtonY;
	int iThumbSize,iThumbStart;
	if(m_iMin!=m_iMax)
	{
		iThumbSize=(iSpace*m_iPageSize)/(m_iPageSize+m_iMax-m_iMin);
		if(iThumbSize<2*m_iPartY) iThumbSize=2*m_iPartY;
		iThumbStart=((m_iPos-m_iMin)*(iSpace-iThumbSize))/(m_iMax-m_iMin);
	}
	else
	{
		iThumbSize=iSpace;
		iThumbStart=0;
	}

	if(m_fOverTop)
	{
		// Set button pressed and fake mousemove to repaint it
		m_fDownTop=TRUE;
		m_fOverTop=FALSE;
		OnMouseMove(nFlags,point);

		MoveThumb(TRUE);
		m_iTick=0;
		SetTimer(1,TIMERDELAY,NULL);
	}
	else if(m_fOverBottom)
	{
		// Set button pressed and fake mousemove to repaint it
		m_fDownBottom=TRUE;
		m_fOverBottom=FALSE;
		OnMouseMove(nFlags,point);

		MoveThumb(TRUE);
		m_iTick=0;
		SetTimer(1,TIMERDELAY,NULL);
	}
	// If within thumb...
	else if( point.y>=m_iButtonY+iThumbStart && 
		point.y<m_iButtonY+iThumbStart+iThumbSize)
	{
		// Capture mouse
		SetCapture();
		m_fCaptured=TRUE;

		// Store start positions
		m_iStartY=point.y;
		m_iStartPos=m_iPos;

		// Set thumb-drag on
		m_fThumbDrag=TRUE;
	}
	else if(point.y<m_iButtonY+iThumbStart)
	{
		// Capture mouse
		SetCapture();
		m_fCaptured=TRUE;

		// Set flag
		m_fPageTop=TRUE;

		// Move thumb and set timer for autorepeat
		MoveThumb(TRUE);
		m_iTick=0;
		SetTimer(1,TIMERDELAY,NULL);
	}
	else if(point.y>=m_iButtonY+iThumbStart+iThumbSize)
	{
		// Capture mouse
		SetCapture();
		m_fCaptured=TRUE;

		// Set flag
		m_fPageBottom=TRUE;

		// Move thumb and set timer for autorepeat
		MoveThumb(TRUE);
		m_iTick=0;
		SetTimer(1,TIMERDELAY,NULL);
	}
	
	CWnd::OnLButtonDown(nFlags, point);
}

void CGraphicVScroll::OnMouseMove(UINT nFlags, CPoint point) 
{
	if(m_iMax==m_iMin) return;

	if(m_fThumbDrag)
	{
		// Avoid divide-by-zeros later
		if(m_iMin==m_iMax) return;

		// Work out difference in point position
		int iYDist=point.y-m_iStartY;

		// Convert that into a new position
		int iSpace=m_pgBackground->Height()-2*m_iButtonY;
		int iThumbSize=(iSpace*m_iPageSize)/(m_iPageSize+m_iMax-m_iMin);
		if(iThumbSize<2*m_iPartY) iThumbSize=2*m_iPartY;
		int iPosDist=(m_iMax-m_iMin)*iYDist/(iSpace-iThumbSize);

		m_iPos=m_iStartPos+iPosDist;

		// Make sure snap-to is applied
		if((m_iPos % m_iSnap)!=0)
			m_iPos=((m_iPos+(m_iSnap/2))/m_iSnap)*m_iSnap;

		if(m_iPos<m_iMin) m_iPos=m_iMin; 
		else if(m_iPos>m_iMax) m_iPos=m_iMax;
		
		// Paint new thumb position
		MoveThumb(FALSE);

		// Do not allow buttons to become activated etc. in thumbdrag
		return;
	}

	if(!m_fOverTop)
	{
		// See if mouse is in top button
		if(point.y<m_iButtonY && point.y>=0 && point.x>=0 && point.x<m_iWidth)
		{
			// Set button over
			m_fOverTop=TRUE;

			// Capture mouse if not already
			if(!m_fCaptured)
			{
				SetCapture();
				m_fCaptured=TRUE;
			}

			// Draw top button mouse-over or down
			CGraphicOnscreen button(m_iWidth,m_iButtonY);
			m_pgBackground->Copy(&button,0,0,0,0,m_iWidth,m_iButtonY);
			if(m_fDownTop)
				m_pgaUpDown->Copy(&button);
			else
				m_pgaUpOver->Copy(&button);
			CPaletteClientDC dc(this);
			button.Draw(&dc,0,0);
		}
	}
	else	// m_fOverTop
	{
		// See if mouse has moved out of button
		if(point.y<0 || point.y>m_iButtonY || point.x<0 || point.x>m_iWidth)
		{
			// Set button not-over
			m_fOverTop=FALSE;

			// Repaint button not-over
			CGraphicOnscreen button(m_iWidth,m_iButtonY);
			m_pgBackground->Copy(&button,0,0,0,0,m_iWidth,m_iButtonY);
			m_pgaUpUp->Copy(&button);
			CPaletteClientDC dc(this);
			button.Draw(&dc,0,0);

			// If not pressed, release capture and fix flags
			if(!m_fDownTop)
			{
				ReleaseCapture();
				m_fCaptured=FALSE;
			}
		}
	}

	CRect rect;
	GetClientRect(&rect);
	
	if(!m_fOverBottom)
	{
		// See if mouse is in Bottom button
		if(point.y<rect.bottom && point.y>=rect.bottom-m_iButtonY && 
			point.x>=0 && point.x<m_iWidth)
		{
			// Set button over
			m_fOverBottom=TRUE;

			// Capture mouse if not already
			if(!m_fCaptured)
			{
				SetCapture();
				m_fCaptured=TRUE;
			}

			// Draw Bottom button mouse-over or down
			CGraphicOnscreen button(m_iWidth,m_iButtonY);
			m_pgBackground->Copy(&button,0,0,0,rect.bottom-m_iButtonY,
				m_iWidth,m_iButtonY);
			if(m_fDownBottom)
				m_pgaDownDown->Copy(&button);
			else
				m_pgaDownOver->Copy(&button);
			CPaletteClientDC dc(this);
			button.Draw(&dc,0,rect.bottom-m_iButtonY);
		}
	}
	else	// m_fOverBottom
	{
		// See if mouse has moved out of button
		if(point.y>=rect.bottom || point.y<rect.bottom-m_iButtonY ||
			point.x<0 || point.x>=m_iWidth)
		{
			// Set button not-over
			m_fOverBottom=FALSE;

			// Repaint button not-over
			CGraphicOnscreen button(m_iWidth,m_iButtonY);
			m_pgBackground->Copy(&button,0,0,0,rect.bottom-m_iButtonY,
				m_iWidth,m_iButtonY);
			m_pgaDownUp->Copy(&button);
			CPaletteClientDC dc(this);
			button.Draw(&dc,0,rect.bottom-m_iButtonY);

			// If not pressed, release capture and fix flags
			if(!m_fDownBottom)
			{
				ReleaseCapture();
				m_fCaptured=FALSE;
			}
		}
	}
	
	CWnd::OnMouseMove(nFlags, point);
}

void CGraphicVScroll::OnPaint() 
{
	CPaletteDC dc(this); // device context for painting

	ASSERT(m_pgBackground);
	CGraphicOnscreen graphic(m_pgBackground->Width(),m_pgBackground->Height());

	// Copy background to temporary graphic
	m_pgBackground->Copy(&graphic);

	// Paint buttons...
	m_pgaUpUp->Copy(&graphic,0,0);
	m_pgaDownUp->Copy(&graphic,0,m_pgBackground->Height()-m_iButtonY);

	// Work out thumb position
	int iSpace=m_pgBackground->Height()-2*m_iButtonY;
	int iThumbSize,iThumbStart;
	if(m_iMin!=m_iMax)
	{
		iThumbSize=(iSpace*m_iPageSize)/(m_iPageSize+m_iMax-m_iMin);
		if(iThumbSize<2*m_iPartY) iThumbSize=2*m_iPartY;
		iThumbStart=((m_iPos-m_iMin)*(iSpace-iThumbSize))/(m_iMax-m_iMin);
	}
	else
	{
		iThumbSize=iSpace;
		iThumbStart=0;
	}


	// Draw background, top part
	int iStop=iThumbStart-m_iPartY;
	for(int iY=0;iY<iStop;iY+=m_iPartY)
		m_pgaNormalMid->Copy(&graphic,0,iY+m_iButtonY);
	m_pgaNormalMid->Copy(&graphic,0,iY+m_iButtonY,0,0,m_iWidth,iStop+m_iPartY-iY);

	// Draw background, bottom part
	iStop=m_pgBackground->Height()-m_iButtonY-m_iPartY;
	for(iY=iThumbStart+iThumbSize+m_iButtonY;
		iY<iStop;iY+=m_iPartY)
		m_pgaNormalMid->Copy(&graphic,0,iY);
	m_pgaNormalMid->Copy(&graphic,0,iY,0,0,m_iWidth,iStop+m_iPartY-iY);

	// Draw thumb top and bottom
	m_pgaThumbTop->Copy(&graphic,0,iThumbStart+m_iButtonY);
	m_pgaThumbBottom->Copy(&graphic,0,iThumbStart+iThumbSize-m_iPartY+m_iButtonY);

	// Draw thumb middle bit
	iStop=iThumbStart+iThumbSize-m_iPartY*2;
	for(iY=iThumbStart+m_iPartY;iY<iStop;iY+=m_iPartY)
		m_pgaThumbMid->Copy(&graphic,0,iY+m_iButtonY);
	m_pgaThumbMid->Copy(&graphic,0,iY+m_iButtonY,0,0,m_iWidth,iStop+m_iPartY-iY);

	graphic.Draw(&dc,0,0);
}

void CGraphicVScroll::MoveThumb(BOOL bApplyMovement)
{
	int iBefore=m_iPos;

	// Handle possibly-pressed buttons
	if(m_fDownTop) m_iPos-=GetStep();
	if(m_fDownBottom) m_iPos+=GetStep();
	if(m_fPageTop) m_iPos-=m_iPageSize;
	if(m_fPageBottom) m_iPos+=m_iPageSize;

	// Make sure snap-to is applied
	if((m_iPos % m_iSnap)!=0)
	{
		m_iPos=((m_iPos+(m_iSnap/2))/m_iSnap)*m_iSnap;
		if(m_iPos>m_iMax) m_iPos=m_iMax;
		else if(m_iPos<m_iMin) m_iPos=m_iMin;

		// Must move!
		if(m_iPos==iBefore)
		{
			if(m_fDownTop || m_fPageTop) 
				m_iPos-=m_iSnap;
			else
				m_iPos+=m_iSnap;
		}
	}

	if(m_iPos<m_iMin) m_iPos=m_iMin;
	else if(m_iPos>m_iMax) m_iPos=m_iMax;

	// Repaint thumb in new position
	CRect rect;
	GetClientRect(&rect);
	rect.top+=m_iButtonY;
	rect.bottom-=m_iButtonY;

	InvalidateRect(&rect,FALSE);

	// Send appropriate message
	if(m_pglList) m_pglList->SetPos(m_iPos);
	else
	{
		GetParent()->SendMessage(WM_VSCROLL,SB_THUMBTRACK | (m_iPos<<16),
			(LPARAM)GetSafeHwnd());
	}
}

void CGraphicVScroll::SetHeight(int iPixels,int iBackgroundSplitY)
{
	if(iPixels==m_pgBackground->Height()) return;

	// Adjust the background graphic
	CGraphic* pgBackground2=new CGraphic(m_iWidth,iPixels);
	m_pgBackground->Copy(pgBackground2,0,0,0,0,m_iWidth,iBackgroundSplitY);
	m_pgBackground->Copy(pgBackground2,0,iPixels-(m_iOriginalHeight-iBackgroundSplitY),
		0,m_pgBackground->Height()-(m_iOriginalHeight-iBackgroundSplitY),m_iWidth,m_iOriginalHeight-iBackgroundSplitY);
	for(int iY=0;iY<iPixels-m_iOriginalHeight;iY++)
	{
		m_pgBackground->Copy(pgBackground2,0,iBackgroundSplitY+iY,
			0,iBackgroundSplitY,m_iWidth,1);
	}
	delete m_pgBackground;
	m_pgBackground=pgBackground2;

	// Resize actual window
	SetWindowPos(NULL,0,0,m_iWidth,iPixels,SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOMOVE|SWP_NOREDRAW|SWP_NOCOPYBITS);
}
