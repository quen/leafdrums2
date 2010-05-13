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
#include "GraphicHScroll.h"
#include "PaletteDC.h"

#include "GraphicOnscreen.h"
#include "ResourceData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGraphicHScroll

CGraphicHScroll::CGraphicHScroll(BOOL fAutoDelete) : CGraphicScrollbar(fAutoDelete)
{
}

CGraphicHScroll::~CGraphicHScroll()
{
}


BEGIN_MESSAGE_MAP(CGraphicHScroll, CGraphicScrollbar)
	//{{AFX_MSG_MAP(CGraphicHScroll)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CGraphicHScroll message handlers

BOOL CGraphicHScroll::Create(CWnd* pwndParent,UINT nID,int iX, int iY,int iW,
	CGraphic* pgBackground, int iXBG,int iYBG, const char* strType,BOOL fShow) 
{
	m_iOriginalWidth=iW;

	// Load the resource defining scrollbar type
	CResourceData res(strType,(char*)RT_RCDATA);

	int* piData=(int*)res.Data();
	ASSERT(piData[0]==0);	// 0 is a "type" value just in case 
	m_iHeight=piData[1];
	m_iButtonX=piData[2];
	m_iPartX=piData[3];

	// Obtain the background graphic
	m_pgBackground=new CGraphic(iW,m_iHeight);
	pgBackground->Copy(m_pgBackground,0,0,iXBG,iYBG,iW,m_iHeight);

	// Make up the creation rectangle
	CRect rect(iX,iY,iX+iW,iY+m_iHeight);

	// Allocate memory for the bitmaps (in vertical mode)
	m_pgaUpUp=new CGraphicAlpha(m_iHeight,m_iButtonX);
	m_pgaUpDown=new CGraphicAlpha(m_iHeight,m_iButtonX);
	m_pgaUpOver=new CGraphicAlpha(m_iHeight,m_iButtonX);
	m_pgaDownUp=new CGraphicAlpha(m_iHeight,m_iButtonX);
	m_pgaDownDown=new CGraphicAlpha(m_iHeight,m_iButtonX);
	m_pgaDownOver=new CGraphicAlpha(m_iHeight,m_iButtonX);
	m_pgaThumbTop=new CGraphicAlpha(m_iHeight,m_iPartX);
	m_pgaThumbBottom=new CGraphicAlpha(m_iHeight,m_iPartX);
	m_pgaThumbMid=new CGraphicAlpha(m_iHeight,m_iPartX);
	m_pgaNormalMid=new CGraphicAlpha(m_iHeight,m_iPartX);

	// Load graphics from resources given in the type file
	char* pbData=(char*)res.Data();
	pbData=&pbData[16];	// Skip numbers
	m_pgaUpUp->LoadResource(pbData);
	m_pgaUpUp->Rotate();
	pbData=&pbData[20];
	m_pgaUpDown->LoadResource(pbData);
	m_pgaUpDown->Rotate();
	pbData=&pbData[20];
	m_pgaUpOver->LoadResource(pbData);
	m_pgaUpOver->Rotate();
	pbData=&pbData[20];
	if(*pbData==0)
	{
		*m_pgaDownUp=*m_pgaUpUp;
		m_pgaDownUp->FlipHorizontal();
	}
	else
	{
		m_pgaDownUp->LoadResource("ScrollUpNormal");
		m_pgaDownUp->Rotate();
	}
	pbData=&pbData[20];
	if(*pbData==0)
	{
		*m_pgaDownDown=*m_pgaUpDown;
		m_pgaDownDown->FlipHorizontal();
	}
	else
	{
		m_pgaDownDown->LoadResource(pbData);
		m_pgaDownDown->Rotate();
	}
	pbData=&pbData[20];
	if(*pbData==0)
	{
		*m_pgaDownOver=*m_pgaUpOver;
		m_pgaDownOver->FlipHorizontal();
	}
	else
	{
		m_pgaDownOver->LoadResource(pbData);
		m_pgaDownOver->Rotate();
	}
	pbData=&pbData[20];
	m_pgaThumbTop->LoadResource(pbData);
	m_pgaThumbTop->Rotate();
	pbData=&pbData[20];
	if(*pbData==0)
	{
		*m_pgaThumbBottom=*m_pgaThumbTop;
		m_pgaThumbBottom->FlipHorizontal();
	}
	else
	{
		m_pgaThumbBottom->LoadResource(pbData);
		m_pgaThumbBottom->Rotate();
	}
	pbData=&pbData[20];
	m_pgaThumbMid->LoadResource(pbData);
	m_pgaThumbMid->Rotate();
	pbData=&pbData[20];
	m_pgaNormalMid->LoadResource(pbData);
	m_pgaNormalMid->Rotate();
		
	// Create the window
	return CWnd::Create(NULL,"CGraphicHScroll",
		WS_CHILD|(fShow ? WS_VISIBLE : 0),
		rect,
		pwndParent,nID);
}

void CGraphicHScroll::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if(m_iMax==m_iMin) return;

	// Check thumb position
	int iSpace=m_pgBackground->Width()-2*m_iButtonX;
	int iThumbSize=(iSpace*m_iPageSize)/(m_iPageSize+m_iMax-m_iMin);
	if(iThumbSize<2*m_iPartX) iThumbSize=2*m_iPartX;
	int iThumbStart=((m_iPos-m_iMin)*(iSpace-iThumbSize))/(m_iMax-m_iMin);

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
	else if( point.x>=m_iButtonX+iThumbStart && 
		point.x<m_iButtonX+iThumbStart+iThumbSize)
	{
		// Capture mouse
		SetCapture();
		m_fCaptured=TRUE;

		// Store start positions
		m_iStartX=point.x;
		m_iStartPos=m_iPos;

		// Set thumb-drag on
		m_fThumbDrag=TRUE;
	}
	else if(point.x<m_iButtonX+iThumbStart)
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
	else if(point.x>=m_iButtonX+iThumbStart+iThumbSize)
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

void CGraphicHScroll::OnMouseMove(UINT nFlags, CPoint point) 
{
	if(m_iMax==m_iMin) return;

	if(m_fThumbDrag)
	{
		// Work out difference in point position
		int iYDist=point.x-m_iStartX;

		// Convert that into a new position
		int iSpace=m_pgBackground->Width()-2*m_iButtonX;
		int iThumbSize=(iSpace*m_iPageSize)/(m_iPageSize+m_iMax-m_iMin);
		if(iThumbSize<2*m_iPartX) iThumbSize=2*m_iPartX;
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
		if(point.x<m_iButtonX && point.x>=0 && point.y>=0 && point.y<m_iHeight)
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
			CGraphicOnscreen button(m_iButtonX,m_iHeight);
			m_pgBackground->Copy(&button,0,0,0,0,m_iButtonX,m_iHeight);
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
		if(point.x<0 || point.x>m_iButtonX || point.y<0 || point.y>m_iHeight)
		{
			// Set button not-over
			m_fOverTop=FALSE;

			// Repaint button not-over
			CGraphicOnscreen button(m_iButtonX,m_iHeight);
			m_pgBackground->Copy(&button,0,0,0,0,m_iButtonX,m_iHeight);
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
		if(point.x<rect.right && point.x>=rect.right-m_iButtonX && 
			point.y>=0 && point.y<m_iHeight)
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
			CGraphicOnscreen button(m_iButtonX,m_iHeight);
			m_pgBackground->Copy(&button,0,0,rect.right-m_iButtonX,0,
				m_iButtonX,m_iHeight);
			if(m_fDownBottom)
				m_pgaDownDown->Copy(&button);
			else
				m_pgaDownOver->Copy(&button);
			CPaletteClientDC dc(this);
			button.Draw(&dc,rect.right-m_iButtonX,0);
		}
	}
	else	// m_fOverBottom
	{
		// See if mouse has moved out of button
		if(point.x>=rect.right || point.x<rect.right-m_iButtonX ||
			point.y<0 || point.y>=m_iHeight)
		{
			// Set button not-over
			m_fOverBottom=FALSE;

			// Repaint button not-over
			CGraphicOnscreen button(m_iButtonX,m_iHeight);
			m_pgBackground->Copy(&button,0,0,rect.right-m_iButtonX,0,
				m_iButtonX,m_iHeight);
			m_pgaDownUp->Copy(&button);
			CPaletteClientDC dc(this);
			button.Draw(&dc,rect.right-m_iButtonX,0);

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

void CGraphicHScroll::OnPaint() 
{
	CPaletteDC dc(this); // device context for painting

	ASSERT(m_pgBackground);
	CGraphicOnscreen graphic(m_pgBackground->Width(),m_pgBackground->Height());

	// Copy background to temporary graphic
	m_pgBackground->Copy(&graphic);

	// Paint buttons...
	m_pgaUpUp->Copy(&graphic,0,0);
	m_pgaDownUp->Copy(&graphic,m_pgBackground->Width()-m_iButtonX,0);

	// Work out thumb position
	int iSpace=m_pgBackground->Width()-2*m_iButtonX;
	int iThumbSize=(iSpace*m_iPageSize)/(m_iPageSize+m_iMax-m_iMin);
	if(iThumbSize<2*m_iPartX) iThumbSize=2*m_iPartX;
	int iThumbStart;
	if(m_iMax==m_iMin) iThumbStart=0;
	else iThumbStart=((m_iPos-m_iMin)*(iSpace-iThumbSize))/(m_iMax-m_iMin);

	// Draw background, top part
	int iStop=iThumbStart-m_iPartX;
	for(int iX=0;iX<iStop;iX+=m_iPartX)
		m_pgaNormalMid->Copy(&graphic,iX+m_iButtonX,0);
	m_pgaNormalMid->Copy(&graphic,iX+m_iButtonX,0,0,0,iStop+m_iPartX-iX,m_iHeight);

	// Draw background, bottom part
	iStop=m_pgBackground->Width()-m_iButtonX-m_iPartX;
	for(iX=iThumbStart+iThumbSize+m_iButtonX;
		iX<iStop;iX+=m_iPartX)
		m_pgaNormalMid->Copy(&graphic,iX,0);
	m_pgaNormalMid->Copy(&graphic,iX,0,0,0,iStop+m_iPartX-iX,m_iHeight);

	// Draw thumb top and bottom
	m_pgaThumbTop->Copy(&graphic,iThumbStart+m_iButtonX,0);
	m_pgaThumbBottom->Copy(&graphic,iThumbStart+iThumbSize-m_iPartX+m_iButtonX,0);

	// Draw thumb middle bit
	iStop=iThumbStart+iThumbSize-m_iPartX*2;
	for(iX=iThumbStart+m_iPartX;iX<iStop;iX+=m_iPartX)
		m_pgaThumbMid->Copy(&graphic,iX+m_iButtonX,0);
	m_pgaThumbMid->Copy(&graphic,iX+m_iButtonX,0,0,0,iStop+m_iPartX-iX,m_iHeight);

	graphic.Draw(&dc,0,0);
}

void CGraphicHScroll::MoveThumb(BOOL bApplyMovement)
{
	int iBefore=m_iPos;

	if(bApplyMovement)
	{
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
				{
					m_iPos=((m_iPos+(m_iSnap/2))/m_iSnap)*m_iSnap;
					m_iPos-=m_iSnap;
				}
				else if(m_fDownBottom || m_fPageBottom)
				{
					m_iPos=((m_iPos+(m_iSnap/2))/m_iSnap)*m_iSnap;
					m_iPos+=m_iSnap;
				}
			}
		}

		if(m_iPos<m_iMin) m_iPos=m_iMin;
		else if(m_iPos>m_iMax) m_iPos=m_iMax;
	}

	// Repaint thumb in new position
	CRect rect;
	GetClientRect(&rect);
	rect.left+=m_iButtonX;
	rect.right-=m_iButtonX;

	InvalidateRect(&rect,FALSE);

	// Send appropriate message
	if(m_pglList) m_pglList->SetPos(m_iPos);
	else
	{
		GetParent()->SendMessage(WM_HSCROLL,SB_THUMBTRACK | (m_iPos<<16),
			(LPARAM)GetSafeHwnd());
	}
}

void CGraphicHScroll::SetWidth(int iPixels,int iBackgroundSplitX)
{
	if(iPixels==m_pgBackground->Width()) return;

	// Adjust the background graphic
	CGraphic* pgBackground2=new CGraphic(iPixels,m_iHeight);
	m_pgBackground->Copy(pgBackground2,0,0,0,0,iBackgroundSplitX,m_iHeight);
	int iOriginalEnd=(m_iOriginalWidth-iBackgroundSplitX);
	m_pgBackground->Copy(pgBackground2,iPixels-iOriginalEnd,0,
		m_pgBackground->Width()-iOriginalEnd,0,iOriginalEnd,m_iHeight);
	for(int iX=0;iX<iPixels-m_iOriginalWidth;iX++)
	{
		m_pgBackground->Copy(pgBackground2,iBackgroundSplitX+iX,0,
			iBackgroundSplitX,0,1,m_iHeight);
	}
	delete m_pgBackground;
	m_pgBackground=pgBackground2;

	// Resize actual window
	SetWindowPos(NULL,0,0,iPixels,m_iHeight,SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOMOVE|SWP_NOREDRAW|SWP_NOCOPYBITS);
}
