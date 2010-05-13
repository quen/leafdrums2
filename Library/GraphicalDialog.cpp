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
#include "GraphicalDialog.h"
#include "ResourceData.h"
#include "TextLineReader.h"
#include "GraphicOnscreen.h"
#include "GraphicButton.h"
#include "GraphicCheckbox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CGraphicalDialog

CGraphicalDialog::CGraphicalDialog()
{
	m_fActive=FALSE;
	m_iNumMoveables=0;

	m_pgBackground=NULL;

	m_pbrNormal=new CBrush(RGB(0xff,0xff,0xff));
	m_pbrFaded=new CBrush(RGB(0xff,0xff,0xff));

	m_iR=m_iG=m_iB=0xff;
}

CGraphicalDialog::~CGraphicalDialog()
{
	// If this actually works, then it means you didn't call it in the destructor
	// of your derived dialog, and WM_DESTROY etc. on that object won't work.
	// so sort it out.
	VERIFY(!DestroyWindow());

	// Delete graphics
	if(m_pgBackground) delete m_pgBackground;
	delete m_pbrNormal;
	delete m_pbrFaded;
}


BEGIN_MESSAGE_MAP(CGraphicalDialog, CGraphicalOutputSink)
	//{{AFX_MSG_MAP(CGraphicalDialog)
	ON_WM_ACTIVATE()
	ON_WM_ERASEBKGND()
	ON_WM_NCHITTEST()
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_COMMAND(IDOK,OnOK)
	ON_COMMAND(IDCANCEL,OnCancel)
	ON_MESSAGE(WM_INITDIALOG,OnInitDialogInternal)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CGraphicalDialog message handlers

BOOL CGraphicalDialog::Create(const char* pstrResourceName,CWnd* pParentWnd) 
{
	m_fModal=FALSE;
	m_strResourceName=pstrResourceName;
	CRect rect(0,0,1,1);

	// Create the actual window
	return CWnd::CreateEx(WS_EX_WINDOWEDGE,AfxRegisterWndClass(0),
		"", WS_VISIBLE|WS_POPUP|WS_DLGFRAME|((pParentWnd==NULL) ? WS_SYSMENU: 0),
		rect, pParentWnd, 0, NULL);
}

int CGraphicalDialog::DoModal(const char* pstrResourceName,CWnd* pParentWnd)
{
	m_strResourceName=pstrResourceName;
	CRect rect(0,0,1,1);

	// Create the actual window
	if(!CWnd::CreateEx(WS_EX_WINDOWEDGE,AfxRegisterWndClass(0),
		"CGraphicalDialog", WS_VISIBLE|WS_POPUP|WS_DLGFRAME|((pParentWnd==NULL) ? WS_SYSMENU: 0), 
		rect, pParentWnd, 0, NULL)) return -1;

	// Set up as a modal dialog
	m_fModal=TRUE;
	if(pParentWnd) pParentWnd->EnableWindow(FALSE);

	// Enter the message-processing loop
	m_iEndDialog=999999;
	while(m_iEndDialog==999999)
	{
		MSG msg;
		while ( ::PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE ) ) 
		{ 
			// Process messages to provide RETURN-key support
			if((msg.message==WM_KEYDOWN) && msg.wParam==VK_RETURN)
			{
				// Remove message
				::PeekMessage( &msg, NULL, 0, 0, PM_REMOVE );

				// Post an IDC_OK
				PostMessage(WM_COMMAND,IDOK,0);
			}
			else if ( !AfxGetApp()->PumpMessage( ) ) 
			{ 
				::PostQuitMessage(0); 
				if(pParentWnd) pParentWnd->EnableWindow(TRUE);
				return IDABORT; 
			} 
		} 
		// let MFC do its idle processing
		LONG lIdle = 0;
		while ( AfxGetApp()->OnIdle(lIdle++ ) )
			;  

		if(m_iEndDialog!=999999) break;

		WaitMessage();
	}

	// Re-enable parent
	if(pParentWnd) pParentWnd->EnableWindow(TRUE);

	// Return the code
	return m_iEndDialog;
}

void CGraphicalDialog::EndDialog(int iReturn)
{
	m_iEndDialog=iReturn;
}

void CGraphicalDialog::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	CWnd::OnActivate(nState, pWndOther, bMinimized);
	
	if(nState==WA_INACTIVE)
		m_fActive=FALSE;
	else
		m_fActive=TRUE;

	Invalidate();
}

BOOL CGraphicalDialog::OnEraseBkgnd(CDC* pDC) 
{
	// Background doesn't need erasing
	return TRUE;
}

UINT CGraphicalDialog::OnNcHitTest(CPoint point) 
{
	// Obtain client co-ordinates
	CPoint ptClient=point;
	ScreenToClient(&ptClient);

	// See if they were in desired range
	for(int i=0;i<m_iNumMoveables;i++)
		if(m_rectMoveable[i].PtInRect(ptClient)) return HTCAPTION;

	return CWnd::OnNcHitTest(point);
}

void CGraphicalDialog::OnPaint() 
{
	// Fill background graphic
	CGraphicOnscreen go(m_iWidth,m_iHeight);
	m_pgBackground->Copy(&go);

	// Request any children to draw
	PaintChildren(&go);

	// Fade graphic if necessary
	if(!m_fActive)
	{
		int iSize=go.ByteWidth()*go.Height();
		BYTE* pbTarget=go.Data();
		for(int i=0;i<iSize;i++)
		{
			// Fade halfway to white
			pbTarget[i]=(pbTarget[i]+255)>>1;
		}
	}

	// Paint to screen
	CPaintDC dc(this); 
	go.Draw(&dc,0,0);
}

int CGraphicalDialog::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	CMenu* pMenu=GetSystemMenu(FALSE);
	if(pMenu)
	{
		pMenu->EnableMenuItem(SC_MAXIMIZE, MF_GRAYED | MF_BYCOMMAND);
		pMenu->EnableMenuItem(SC_SIZE, MF_GRAYED | MF_BYCOMMAND);        
	}

	HWND hwndFocus=SetResource(m_strResourceName);
	if(!hwndFocus) return -1;

	if(SendMessage(WM_INITDIALOG,(WPARAM)hwndFocus,0))
		CWnd::FromHandle(hwndFocus)->SetFocus();

	return 0;
}

BOOL CALLBACK GDDeleteChildrenProc(HWND hwnd,LPARAM)
{
	DestroyWindow(hwnd);
	return TRUE;
}

CString CGraphicalDialog::ResourceName()
{
	return m_strResourceName;
}

HWND CGraphicalDialog::SetResource(const char* pstrName)
{
	BOOL fFirstTime=(m_pgBackground==NULL);

	// If this isn't the first time, clear up from the old one
	int idFocus=-1;
	if(!fFirstTime)
	{
		// Get variables up-to-date
		UpdateData(TRUE);

		// Find current focus
		CWnd* pFocus=CWnd::GetFocus();
		if(pFocus!=NULL && pFocus->GetParent()!=NULL && pFocus->GetParent()==this)
			idFocus=pFocus->GetDlgCtrlID();

		// Delete old background
		delete m_pgBackground;

		// Destroy all the old controls
		VERIFY(EnumChildWindows(GetSafeHwnd(),
			&GDDeleteChildrenProc,NULL));
		m_Font.DeleteObject();
	}

	m_strResourceName=pstrName;

	// Open resource file as text lines
	CResourceData rd(m_strResourceName,"GraphicalDialog");
	CTextLineReader tlr(rd.Data(),rd.Size());

	// Read first line with dimensions
	CString strLine;
	tlr.NextLine(strLine);
	CString strWidth=strLine.Left(strLine.Find(','));
	CString strHeight=strLine.Mid(strLine.Find(',')+1);
	m_iWidth=atoi(strWidth);
	m_iHeight=atoi(strHeight);

	// Load graphics
	CGraphic gNormal(m_iWidth,m_iHeight);
	if(!gNormal.LoadCompressedResource(m_strResourceName+"_norm"))
		return NULL; // Normal graphic must be there!

	// Copy graphic to screen
	m_pgBackground=new CGraphic(m_iWidth,m_iHeight);
	gNormal.Copy(m_pgBackground);

	CGraphic gOver(m_iWidth,m_iHeight);
	if(!gOver.LoadCompressedResource(m_strResourceName+"_over"))
		gNormal.Copy(&gOver); // Use a copy of normal graphic
		
	CGraphic gDown(m_iWidth,m_iHeight);
	if(!gDown.LoadCompressedResource(m_strResourceName+"_down"))
		gNormal.Copy(&gDown); // Use a copy of normal graphic

	// Read font
	VERIFY(tlr.NextLine(strLine));
	CString strFont=strLine.Left(strLine.Find(','));
	CString strSize=strLine.Mid(strLine.Find(',')+1);
	m_Font.CreatePointFont(atoi(strSize)*10,strFont);

	BOOL fPickedFocus=FALSE;
	HWND hwndFocus;

	CGraphicButton* pgbLastRadio=NULL;

	// Read remaining lines of resource description
	while(tlr.NextLine(strLine))
	{
		// Obtain object type
		if(strLine.Find(':')==-1) return NULL;
		CString strType=strLine.Left(strLine.Find(':'));

		// Obtain parameters
		CString strRemainder=strLine.Mid(strLine.Find(':')+1);
		int iParameter[10],iPos=0;
		while(iPos<9 && strRemainder.Find(',')!=-1)
		{
			iParameter[iPos++]=atoi(strRemainder.Left(strRemainder.Find(',')));
			strRemainder=strRemainder.Mid(strRemainder.Find(',')+1);
		}
		iParameter[iPos]=atoi(strRemainder);

		// Take action depending on type
		if(strType=="moveable")
		{
			m_rectMoveable[m_iNumMoveables++].SetRect(
				iParameter[0],iParameter[1],
				  iParameter[2]+iParameter[0],
				  iParameter[3]+iParameter[1]);
		}
		else if(strType=="background")
		{
			// Set the background colour (used for text thingies)
			delete m_pbrNormal;
			delete m_pbrFaded;
			m_iR=iParameter[0]; m_iG=iParameter[1]; m_iB=iParameter[2];
			m_pbrNormal=new CBrush(RGB(m_iR,m_iG,m_iB));
			m_pbrFaded=new CBrush(RGB((m_iR+255)/2,(m_iG+255)/2,(m_iB+255)/2));
		}
		else if(strType=="name")
		{
			SetWindowText(strRemainder);
		}
		else if(strType=="button")
		{
			// Create a new graphic button with autodelete
			CGraphicButton* pButton=new CGraphicButton(TRUE);
			pButton->Create(this,iParameter[0], // ID
				"",iParameter[1],iParameter[2],iParameter[3],iParameter[4], // x,y,w,h
				"Arial",10, // font (unused)
				&gNormal,iParameter[1],iParameter[2],RGB(0,0,0),
				&gOver,iParameter[1],iParameter[2],RGB(0,0,0),
				&gDown,iParameter[1],iParameter[2],RGB(0,0,0),
				NULL);
		}
		else if(strType=="radiobutton")
		{
			// Create a new graphic button with autodelete
			CGraphicButton* pButton=new CGraphicButton(TRUE);
			pButton->Create(this,iParameter[0], // ID
				"",iParameter[1],iParameter[2],iParameter[3],iParameter[4], // x,y,w,h
				"Arial",10, // font (unused)
				&gNormal,iParameter[1],iParameter[2],RGB(0,0,0),
				&gOver,iParameter[1],iParameter[2],RGB(0,0,0),
				&gDown,iParameter[1],iParameter[2],RGB(0,0,0),
				pgbLastRadio);
			pgbLastRadio=pButton;
		}
		else if(strType=="checkbox")
		{
			// Create a new graphic checkbox with autodelete
			CGraphicCheckbox* pCheck=new CGraphicCheckbox(TRUE);
			pCheck->Create(this,iParameter[0], // ID
				"",iParameter[1],iParameter[2],iParameter[3],iParameter[4], // x,y,w,h
				"Arial",10, // font (unused)
				&gNormal,iParameter[1],iParameter[2],RGB(0,0,0),
				&gOver,iParameter[1],iParameter[2],RGB(0,0,0),
				&gNormal,iParameter[1],iParameter[2],RGB(0,0,0),
				&gOver,iParameter[1],iParameter[2],RGB(0,0,0)
				);
		}
		else if(strType=="win-edit")
		{
			VERIFY(CreateWindowEx(WS_EX_CLIENTEDGE,"edit","",
				iParameter[5]|WS_VISIBLE|WS_CHILD|ES_AUTOHSCROLL,
				iParameter[1],iParameter[2],iParameter[3],iParameter[4],
				GetSafeHwnd(),(HMENU)iParameter[0],AfxGetInstanceHandle(),NULL));
			GetDlgItem(iParameter[0])->SetFont(&m_Font);
		}
		else if(strType=="win-text")
		{
			VERIFY(CreateWindowEx(0,"static","",WS_VISIBLE|WS_CHILD|SS_LEFT,
				iParameter[1],iParameter[2],iParameter[3],iParameter[4],
				GetSafeHwnd(),(HMENU)iParameter[0],AfxGetInstanceHandle(),NULL));
			GetDlgItem(iParameter[0])->SetFont(&m_Font);
		}
		else if(strType=="win-treelist")
		{
			VERIFY(CreateWindowEx(WS_EX_CLIENTEDGE,"SysTreeView32","",WS_VISIBLE|WS_CHILD|WS_BORDER|iParameter[5],
				iParameter[1],iParameter[2],iParameter[3],iParameter[4],
				GetSafeHwnd(),(HMENU)iParameter[0],AfxGetInstanceHandle(),NULL));
			GetDlgItem(iParameter[0])->SetFont(&m_Font);
		}
		else if(strType=="win-listbox")
		{
			VERIFY(CreateWindowEx(WS_EX_CLIENTEDGE,"listbox","",WS_VISIBLE|WS_CHILD|WS_BORDER|
				LBS_NOTIFY|LBS_NOINTEGRALHEIGHT|WS_VSCROLL|iParameter[5],
				iParameter[1],iParameter[2],iParameter[3],iParameter[4],
				GetSafeHwnd(),(HMENU)iParameter[0],AfxGetInstanceHandle(),NULL));
			GetDlgItem(iParameter[0])->SetFont(&m_Font);
		}
		else if(strType=="win-combobox")
		{
			VERIFY(CreateWindowEx(WS_EX_CLIENTEDGE,"combobox","",WS_VISIBLE|WS_CHILD|WS_BORDER|
				CBS_NOINTEGRALHEIGHT|WS_VSCROLL|iParameter[5],
				iParameter[1],iParameter[2],iParameter[3],iParameter[4],
				GetSafeHwnd(),(HMENU)iParameter[0],AfxGetInstanceHandle(),NULL));
			GetDlgItem(iParameter[0])->SetFont(&m_Font);
		}

		if(strType!="radiobutton")
			pgbLastRadio=NULL;

		// Focus goes to first control
		if(!fPickedFocus && strType!="moveable" && strType!="background"
			&& strType!="name")
		{
			fPickedFocus=TRUE;
			hwndFocus=GetDlgItem(iParameter[0])->GetSafeHwnd();
		}
	}

	// Set up initial control data
	UpdateData(FALSE);

	// Resize if necessary
	if(!fFirstTime) OnSize(999,0,0);

	// Set focus if necessary
	if(idFocus!=-1) 
	{
		CWnd* pFocus=GetDlgItem(idFocus);
		if(pFocus) pFocus->SetFocus();
		else
			CWnd::FromHandle(hwndFocus)->SetFocus();
	}

	// Make sure to repaint everything
	InvalidateRect(NULL,TRUE);

	return hwndFocus;
}

void CGraphicalDialog::OnSize(UINT nType, int cx, int cy) 
{
	if(nType==999 || nType==SIZE_RESTORED)
	{
		CRect rectWindow,rectClient;
		GetWindowRect(&rectWindow);
		GetClientRect(&rectClient);
		if(rectClient.Width()!=m_iWidth || rectClient.Height()!=m_iHeight)
		{
			// Make correct size for client area
			rectWindow.right=rectWindow.right+(m_iWidth-rectClient.Width());
			rectWindow.bottom=rectWindow.bottom+(m_iHeight-rectClient.Height());

			// (slightly off) Centre with respect to parent or screen if no parent
			if(nType!=999)
			{
				CRect rectParent;
				if(GetOwner())
					GetOwner()->GetWindowRect(&rectParent);
				else
					rectParent.SetRect(0,0,GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN));
				int iNewX=(rectParent.left+rectParent.right-rectWindow.Width())/2,
					iNewY=(rectParent.top+rectParent.bottom-rectWindow.Height())/2;
				rectWindow.OffsetRect(iNewX-rectWindow.left+10,iNewY-rectWindow.top+10);
			}

			MoveWindow(&rectWindow);
		}
	}
	else CWnd::OnSize(nType,cx,cy);
}

void CGraphicalDialog::OnClose()
{
	EndDialog(IDCANCEL);
}

void CGraphicalDialog::OnOK()
{
	UpdateData(TRUE);
	EndDialog(IDOK);
}

void CGraphicalDialog::OnCancel()
{
	EndDialog(IDCANCEL);
}

BOOL CGraphicalDialog::OnInitDialog()
{
	return TRUE;
}

LONG CGraphicalDialog::OnInitDialogInternal(UINT,LONG)
{
	return (LONG)(this->OnInitDialog());
}

HBRUSH CGraphicalDialog::OnCtlColor( CDC* pDC, CWnd* pWnd, UINT nCtlColor )
{
	if(nCtlColor==CTLCOLOR_STATIC)
	{
		if(m_fActive)
		{
			pDC->SetBkColor(RGB(m_iR,m_iG,m_iB));
			pDC->SetTextColor(RGB(0,0,0));
			return (HBRUSH)(*m_pbrNormal);
		}
		else
		{
			pDC->SetBkColor(RGB((m_iR+255)/2,(m_iG+255)/2,(m_iB+255)/2));
			pDC->SetTextColor(RGB(0x80,0x80,0x80));
			return (HBRUSH)(*m_pbrFaded);
		}
	}
	else
	{
		if(!m_fActive)
			pDC->SetTextColor(RGB(0x80,0x80,0x80));
		return (HBRUSH)::GetStockObject(WHITE_BRUSH);
	}
}

void AFXAPI DDX_GraphicCheckbox(CDataExchange* pDX, int nIDC, int& value)
{
	CGraphicCheckbox* pgc=dynamic_cast<CGraphicCheckbox*>
		(pDX->m_pDlgWnd->GetDlgItem(nIDC));
	ASSERT(pgc);

	if(pDX->m_bSaveAndValidate)
		value=pgc->Checked();
	else 
		pgc->SetChecked(value);
}


