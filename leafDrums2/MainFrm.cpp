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
#include "leafDrums.h"

#include "MainFrm.h"

#include "PaletteDC.h"
#include "GraphicOnscreen.h"

#include "GraphicButton.h"

#include "Security.h"
#include "NagDialog.h"
#include "RegisterDialog.h"
#include "OneShotDialog.h"

#include "LogoWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_QUERYNEWPALETTE()
	ON_COMMAND(ID_REGISTER, OnRegister)
	ON_WM_TIMER()
	ON_WM_GETMINMAXINFO()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_SETTEXT,OnSetText)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// Obtain the user's settings for font & size
	NONCLIENTMETRICS ncm;
	FillMemory(&ncm,sizeof(ncm),0);
	ncm.cbSize=sizeof(ncm);
	VERIFY(SystemParametersInfo(SPI_GETNONCLIENTMETRICS,
		0,&ncm,0));
}

CMainFrame::~CMainFrame()
{
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	BOOL fOK=CFrameWnd::PreCreateWindow(cs);
	//cs.style=FWS_ADDTOTITLE;

	return fOK;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

void CMainFrame::OnSize(UINT nType, int cx, int cy) 
{
	CFrameWnd::OnSize(nType, cx, cy);
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	// Set icon
	HICON hIcon=AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	SetIcon(hIcon,FALSE);

	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Get rid of menu
	CMenu* pmenu=GetMenu();
	SetMenu(NULL);
	pmenu->DestroyMenu();

	// Get rid of the loading window
	((CLeafDrumsApp*)AfxGetApp())->SetMainWnd(this);
	CLogoWnd::sm_pCurrent->DestroyWindow();

	// If it isn't registered...
	if(!CRegistration::sm_pCurrent->Registered())
	{
		// Display nag dialog
		CNagDialog dlgNag(this);
		dlgNag.DoModal();

		// Display another dialog after 30 days
		if(CRegistration::sm_pCurrent->DaysUsed()>=30)
		{
			COneShotDialog dlg(IDS_TIMEOUT);
			dlg.DoModal();
		}
	}		

	// Display 256-colour warning
	CClientDC dc(this);
	if(dc.GetDeviceCaps(RASTERCAPS) & RC_PALETTE)
	{
		COneShotDialog dlg(IDS_256COLOUR);
		dlg.DoModal();
	}

	// Display the welcome dialog a bit later
	PostMessage(WM_TIMER,0,0);

	// Obtain client and window rectangles 
	CRect rectClient,rectWindow;
	GetClientRect(&rectClient);
	GetWindowRect(&rectWindow);

	// Check the difference from the required client size
	int iXGrow=CLIENTWIDTH-rectClient.Width();
	int iYGrow=CLIENTHEIGHT-rectClient.Height();

	// Enlarge the window by that amount
	rectWindow.right+=iXGrow;
	rectWindow.bottom+=iYGrow;

	// Check we're onscreen, if not, centre
	if( rectWindow.right>GetSystemMetrics(SM_CXSCREEN) ||
		rectWindow.bottom>GetSystemMetrics(SM_CYSCREEN)-30)
	{
		int iXPos=(GetSystemMetrics(SM_CXSCREEN)-rectWindow.Width())/2;
		int iYPos=(GetSystemMetrics(SM_CYSCREEN)-30-rectWindow.Height())/2;

		rectWindow.OffsetRect(
			iXPos-rectWindow.left,iYPos-rectWindow.top);
	}

	MoveWindow(&rectWindow);

	return 0;
}

void CMainFrame::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CFrameWnd::OnLButtonDown(nFlags, point);
}

LRESULT CMainFrame::OnSetText(WPARAM wp, LPARAM lp)
{
	CString strCaption=(char*)lp;

	// Remove filename extension
	if(strCaption.Find(".leafDrums2")!=-1)
	{
		int iPos=strCaption.Find(".leafDrums2");
		strCaption=strCaption.Left(iPos)+strCaption.Mid(iPos+11);
	}

	// Add version number
	strCaption+=" 2.3 beta";

	// Alter title to add registration status
	if(CRegistration::sm_pCurrent->Registered())
		strCaption.Format("%s (registered to %s)",strCaption,
			CRegistration::sm_pCurrent->RegisteredName());
	else strCaption+=" (unregistered trial version)";

	return DefWindowProc(WM_SETTEXT,wp,(LPARAM)(const char*)strCaption);
}
 

BOOL CMainFrame::OnQueryNewPalette() 
{
	CDisplayDevice::sm_pDisplayDevice->Realize(this);
	
	return TRUE;
}

void CMainFrame::OnRegister() 
{
	CRegisterDialog dlg;
	dlg.DoModal();
}

void CMainFrame::OnTimer(UINT nIDEvent) 
{
	// Display welcome dialog
	COneShotDialog dlg(IDS_WELCOME);
	dlg.DoModal();
}

void CMainFrame::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	// Obtain client and window rectangles 
	CRect rectClient,rectWindow;
	GetClientRect(&rectClient);
	GetWindowRect(&rectWindow);

	lpMMI->ptMinTrackSize.x=CLIENTWIDTH + (rectWindow.Width() - rectClient.Width());
	lpMMI->ptMinTrackSize.y=CLIENTHEIGHT + (rectWindow.Height() - rectClient.Height());
}
