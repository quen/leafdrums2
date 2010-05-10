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
// LogoWnd.cpp : implementation file
//

#include "stdafx.h"
#include "leafdrums.h"
#include "LogoWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "PaletteDC.h"

/////////////////////////////////////////////////////////////////////////////
// CLogoWnd

CLogoWnd* CLogoWnd::sm_pCurrent;

CLogoWnd::CLogoWnd()
{
	m_bFirst=TRUE;
	sm_pCurrent=this;
	Create();
}

CLogoWnd::~CLogoWnd()
{
	delete m_pgoLogo;
}


BEGIN_MESSAGE_MAP(CLogoWnd, CWnd)
	//{{AFX_MSG_MAP(CLogoWnd)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CLogoWnd message handlers

BOOL CLogoWnd::Create() 
{
	return CWnd::CreateEx(0,AfxRegisterWndClass(0),
		"leafDrums 2 loading...",WS_POPUP|WS_BORDER|WS_VISIBLE,
		0,0,100,100,NULL,NULL);
}

void CLogoWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	if(m_bFirst)
	{
		m_bFirst=FALSE;
		m_pgoLogo=new CGraphicOnscreen(265,145);
		m_pgoLogo->LoadResource("Logo");
		PostMessage(WM_TIMER,0,0);
	}
	
	CDisplayDevice::sm_pDisplayDevice->Realize(this);
	CDisplayDevice::sm_pDisplayDevice->InitDC(&dc);

	m_pgoLogo->Draw(&dc,0,0);	
}

void CLogoWnd::OnTimer(UINT nIDEvent) 
{
	((CLeafDrumsApp*)AfxGetApp())->ContinueLoading();
}

void CLogoWnd::OnDestroy() 
{
	CWnd::OnDestroy();

	delete this;	
}

void CLogoWnd::OnSize(UINT nType, int cx, int cy) 
{
	CRect rectClient,rectWindow;
	GetClientRect(&rectClient);
	GetWindowRect(&rectWindow);

	int 
		iNewWidth=rectWindow.Width(),
		iNewHeight=rectWindow.Height();

	BOOL bChange=FALSE;
	if(rectClient.Width()!=265)
	{
		iNewWidth+=265-rectClient.Width();
		bChange=TRUE;
	}
	if(rectWindow.Width()!=145)
	{
		iNewHeight+=145-rectClient.Height();
		bChange=TRUE;
	}

	if(bChange)
	{
		RECT rWorkArea;
		SystemParametersInfo(SPI_GETWORKAREA,0,&rWorkArea,0);

		SetWindowPos(NULL,
			(rWorkArea.left+rWorkArea.right-iNewWidth)/2,
			(rWorkArea.top+rWorkArea.bottom-iNewHeight)/2,
			iNewWidth,iNewHeight,SWP_NOZORDER);
	}
}
