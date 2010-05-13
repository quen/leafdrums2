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

#include "GraphicOption.h"

#include "GraphicScrollbar.h" // For timer constants

#include "PaletteDC.h"
#include "GraphicOnscreen.h"
#include "ResourceData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define CGraphicOption_TIMERPULSE 20
#define CGraphicOption_TIMERINITIAL 20
#define CGraphicOption_TIMERBOOST 50
#define CGraphicOption_TIMERBOOSTSTAGES 5


/////////////////////////////////////////////////////////////////////////////
// CGraphicOption

CGraphicOption::CGraphicOption(const BOOL fAutoDelete)
{
	// Clear variables
	m_iPos=0; 
	m_fCaptured=m_fOverLeft=m_fOverRight=m_fDownLeft=m_fDownRight=FALSE;

	// Null background graphic
	m_pgBackground=NULL;
	m_pgaUpUp=NULL;
	m_pgaUpDown=NULL;
	m_pgaUpOver=NULL;
	m_pgaDownUp=NULL;
	m_pgaDownDown=NULL;
	m_pgaDownOver=NULL;
	m_pgaNormalMid=NULL;
	
	m_fAutoDelete=fAutoDelete;
}

CGraphicOption::~CGraphicOption()
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
	if(m_pgaNormalMid)
		delete m_pgaNormalMid;
	if(m_pfont)
		delete m_pfont;
}

void CGraphicOption::PostNcDestroy() 
{
	CWnd::PostNcDestroy();

	if(m_fAutoDelete) delete this;
}

BEGIN_MESSAGE_MAP(CGraphicOption, CWnd)
	//{{AFX_MSG_MAP(CGraphicOption)
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_PAINT()
	ON_WM_CANCELMODE()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_SETCURSOR()
	//}}AFX_MSG_MAP
	ON_WM_TIMER()
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CGraphicOption message handlers

BOOL CGraphicOption::Create(CWnd* pwndParent,UINT nID,int iX,int iY,int iW,int iH,
	const char* strFont, int iPointSize, COLORREF clrText,
	CGraphic* pgBackground, int iXBG,int iYBG, 
	const char* pstrType,int iBaselineAdjust,BOOL fShow,
	BOOL bAntiAliased)
{
	// Save data
	m_iBaselineAdjust=iBaselineAdjust;

	// Create font
	m_pfont=new CGraphicFont(strFont,iPointSize,bAntiAliased);
	m_clrText=clrText;

	// Obtain the background graphic
	m_pgBackground=new CGraphic(iW,iH);
	pgBackground->Copy(m_pgBackground,0,0,iXBG,iYBG,iW,iH);

	// Make up the creation rectangle
	CRect rect(iX,iY,iX+iW,iY+iH);

	// Load the resource defining scrollbar type
	CResourceData res(pstrType,(char*)RT_RCDATA);

	int* piData=(int*)res.Data();
	ASSERT(piData[0]==0);	// 0 is a "type" value just in case 
	m_iButtonHeight=piData[1];
	m_iButtonX=piData[2];
	m_iPartX=piData[3];

	// Allocate memory for the bitmaps (in vertical mode)
	m_pgaUpUp=new CGraphicAlpha(m_iButtonHeight,m_iButtonX);
	m_pgaUpDown=new CGraphicAlpha(m_iButtonHeight,m_iButtonX);
	m_pgaUpOver=new CGraphicAlpha(m_iButtonHeight,m_iButtonX);
	m_pgaDownUp=new CGraphicAlpha(m_iButtonHeight,m_iButtonX);
	m_pgaDownDown=new CGraphicAlpha(m_iButtonHeight,m_iButtonX);
	m_pgaDownOver=new CGraphicAlpha(m_iButtonHeight,m_iButtonX);
	m_pgaNormalMid=new CGraphicAlpha(m_iButtonHeight,m_iPartX);

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
	// Skip remaining scrollbar-specific graphics
	pbData=&pbData[20];
	pbData=&pbData[20];
	pbData=&pbData[20];
	pbData=&pbData[20];
	m_pgaNormalMid->LoadResource(pbData);
	m_pgaNormalMid->Rotate();

	// Create the window
	return CWnd::Create(NULL,"CGraphicOption",
		WS_CHILD|(fShow ? WS_VISIBLE : 0),
		rect,
		pwndParent,nID);
}

BOOL CGraphicOption::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
}

void CGraphicOption::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if(m_fOverLeft)
	{
		// Set the button as down and repaint it
		m_fOverLeft=FALSE;
		m_fDownLeft=TRUE;
		OnMouseMove(nFlags,point);

		// Change the text string
		m_iPos--;
		if(m_iPos<GetMin()) m_iPos=GetMax();

		// Repaint text
		CRect rect(m_iButtonX,0,m_pgBackground->Width()-m_iButtonX,
			m_pgBackground->Height());
		InvalidateRect(&rect,FALSE);

		// Begin timer to make more moves
		m_iTick=0;
		m_iTimerDirection=-1;
		SetTimer(1,CGraphicOption_TIMERPULSE,NULL);

		// Notify parent
		SetSel(m_iPos,TRUE);
	}
	if(m_fOverRight)
	{
		// Set the button as down and repaint it
		m_fOverRight=FALSE;
		m_fDownRight=TRUE;
		OnMouseMove(nFlags,point);

		// Change the text string
		m_iPos++;
		if(m_iPos>GetMax()) m_iPos=GetMin();

		// Repaint text
		CRect rect(m_iButtonX,0,m_pgBackground->Width()-m_iButtonX,
			m_pgBackground->Height());
		InvalidateRect(&rect,FALSE);

		// Begin timer to make more moves
		m_iTick=0;
		m_iTimerDirection=1;
		SetTimer(1,CGraphicOption_TIMERPULSE,NULL);

		// Notify parent
		SetSel(m_iPos,TRUE);
	}
	
	CWnd::OnLButtonDown(nFlags, point);
}

void CGraphicOption::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if(m_fDownLeft)
	{
		if(m_fOverLeft)
		{
			// Repaint to "over" status
			m_fDownLeft=FALSE;
			m_fOverLeft=FALSE;
			OnMouseMove(nFlags,point);
		}
		else if(m_fCaptured)
		{
			m_fCaptured=FALSE;
			ReleaseCapture();
		}
		KillTimer(1);
	}
	if(m_fDownRight)
	{
		if(m_fOverRight)
		{
			// Repaint to "over" status
			m_fDownRight=FALSE;
			m_fOverRight=FALSE;
			OnMouseMove(nFlags,point);
		}
		else if(m_fCaptured)
		{
			m_fCaptured=FALSE;
			ReleaseCapture();
		}
		KillTimer(1);
	}

	// Neither button is now down
	m_fDownLeft=m_fDownRight=FALSE;
	
	CWnd::OnLButtonUp(nFlags, point);
}

void CGraphicOption::OnPaint() 
{
	CPaletteDC dc(this); // device context for painting

	ASSERT(m_pgBackground);
	CGraphicOnscreen graphic(m_pgBackground->Width(),m_pgBackground->Height());

	// Copy background to temporary graphic
	m_pgBackground->Copy(&graphic);

	int iYOffset=(m_pgBackground->Height() - m_pgaUpUp->Height())/2;

	// Paint buttons...
	m_pgaUpUp->Copy(&graphic,0,iYOffset);
	m_pgaDownUp->Copy(&graphic,m_pgBackground->Width()-m_iButtonX,iYOffset);

	// Paint background
	int iStop=m_pgBackground->Width()-m_iButtonX*2-m_iPartX;
	for(int iX=0;iX<iStop;iX+=m_iPartX)
		m_pgaNormalMid->Copy(&graphic,iX+m_iButtonX,0);
	m_pgaNormalMid->Copy(&graphic,iX+m_iButtonX,0,0,0,iStop+m_iPartX-iX,m_iButtonHeight);

	// Reset position
	if(m_iPos < GetMin()) m_iPos=GetMin();
	if(m_iPos > GetMax()) m_iPos=GetMax();

	if(m_iPos >= GetMin()) 
	{
		m_pfont->Draw(&graphic,
			(m_pgBackground->Width()-m_pfont->Width(GetText(m_iPos)))/2,
			(m_pgBackground->Height()-m_pfont->Height())+m_iBaselineAdjust,
			GetText(m_iPos),m_clrText);
	}

	graphic.Draw(&dc,0,0);
}

int CGraphicOption::GetSel()
{
	return m_iPos;
}

void CGraphicOption::SetSel(const int iIndex,const BOOL fNotify)
{
	ASSERT(iIndex>=GetMin() && iIndex<=GetMax());
	m_iPos=iIndex;

	// Post a combo-box selchange message
	if(fNotify)
		GetParent()->PostMessage(WM_COMMAND,
			(CBN_SELCHANGE << 16) | GetDlgCtrlID(),
			(LPARAM)GetSafeHwnd());
}


void CGraphicOption::OnCancelMode() 
{
	if(m_fCaptured)
	{
		ReleaseCapture();
		m_fCaptured=FALSE;
		KillTimer(1);
	}
}

void CGraphicOption::OnMouseMove(UINT nFlags, CPoint point) 
{
	int iYOffset=(m_pgBackground->Height() - m_pgaUpUp->Height())/2;

	if(!m_fOverLeft)
	{
		// See if mouse is in Left button
		if(point.x<m_iButtonX && point.x>=0 && 
		  point.y>=iYOffset && point.y<iYOffset+m_iButtonHeight)
		{
			// Set button over
			m_fOverLeft=TRUE;

			// Capture mouse if not already
			if(!m_fCaptured)
			{
				SetCapture();
				m_fCaptured=TRUE;
			}

			// Draw Left button mouse-over or down
			CGraphicOnscreen button(m_iButtonX,m_iButtonHeight);
			m_pgBackground->Copy(&button,0,0,0,iYOffset,
				m_iButtonX,m_iButtonHeight);
			if(m_fDownLeft)
				m_pgaUpDown->Copy(&button);
			else
				m_pgaUpOver->Copy(&button);
			CPaletteClientDC dc(this);
			button.Draw(&dc,0,iYOffset);
		}
	}
	else	// m_fOverLeft
	{
		// See if mouse has moved out of button
		if(point.x<0 || point.x>m_iButtonX || 
			point.y<iYOffset || point.y>iYOffset+m_iButtonHeight)
		{
			// Set button not-over
			m_fOverLeft=FALSE;

			// Repaint button not-over
			CGraphicOnscreen button(m_iButtonX,m_iButtonHeight);
			m_pgBackground->Copy(&button,0,0,0,iYOffset,
				m_iButtonX,m_iButtonHeight);
			m_pgaUpUp->Copy(&button);
			CPaletteClientDC dc(this);
			button.Draw(&dc,0,iYOffset);

			// If not pressed, release capture and fix flags
			if(!m_fDownLeft)
			{
				ReleaseCapture();
				m_fCaptured=FALSE;
			}
		}
	}

	CRect rect;
	GetClientRect(&rect);
	
	if(!m_fOverRight)
	{
		// See if mouse is in Right button
		if(point.x<rect.right && point.x>=rect.right-m_iButtonX && 
			point.y>=iYOffset && point.y<iYOffset+m_iButtonHeight)
		{
			// Set button over
			m_fOverRight=TRUE;

			// Capture mouse if not already
			if(!m_fCaptured)
			{
				SetCapture();
				m_fCaptured=TRUE;
			}

			// Draw Right button mouse-over or down
			CGraphicOnscreen button(m_iButtonX,m_iButtonHeight);
			m_pgBackground->Copy(&button,0,0,rect.right-m_iButtonX,iYOffset,
				m_iButtonX,m_iButtonHeight);
			if(m_fDownRight)
				m_pgaDownDown->Copy(&button);
			else
				m_pgaDownOver->Copy(&button);
			CPaletteClientDC dc(this);
			button.Draw(&dc,rect.right-m_iButtonX,iYOffset);
		}
	}
	else	// m_fOverRight
	{
		// See if mouse has moved out of button
		if(point.x>=rect.right || point.x<rect.right-m_iButtonX ||
			point.y<iYOffset || point.y>iYOffset+m_iButtonHeight)
		{
			// Set button not-over
			m_fOverRight=FALSE;

			// Repaint button not-over
			CGraphicOnscreen button(m_iButtonX,m_iButtonHeight);
			m_pgBackground->Copy(&button,0,0,rect.right-m_iButtonX,iYOffset,
				m_iButtonX,m_iButtonHeight);
			m_pgaDownUp->Copy(&button);
			CPaletteClientDC dc(this);
			button.Draw(&dc,rect.right-m_iButtonX,iYOffset);

			// If not pressed, release capture and fix flags
			if(!m_fDownRight)
			{
				ReleaseCapture();
				m_fCaptured=FALSE;
			}
		}
	}
	
	CWnd::OnMouseMove(nFlags, point);
}

void CGraphicOption::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	OnLButtonDown(nFlags, point);
}

void CGraphicOption::OnTimer(UINT nIDEvent) 
{
	if(nIDEvent==1)
	{
		m_iTick++;
		if(m_iTick>CGraphicOption_TIMERINITIAL)
		{
			// We begin doing 1/4 the pulses, then move to 1/3 after TIMERBOOST pulses, and so on
			int iBoostState=
				CGraphicOption_TIMERBOOSTSTAGES-(m_iTick-CGraphicOption_TIMERINITIAL)/CGraphicOption_TIMERBOOST;
			if(iBoostState>1 && (m_iTick%iBoostState!=0))
				return;

			// Update position
			m_iPos+=m_iTimerDirection;
			if(m_iPos<GetMin()) m_iPos=GetMax();
			if(m_iPos>GetMax()) m_iPos=GetMin();

			// Repaint text
			CRect rect(m_iButtonX,0,m_pgBackground->Width()-m_iButtonX,
				m_pgBackground->Height());
			InvalidateRect(&rect,FALSE);
		}
	}
	else
		CWnd::OnTimer(nIDEvent);
}

BOOL CGraphicOption::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	::SetCursor(::LoadCursor(NULL,IDC_ARROW));
	return TRUE;
}