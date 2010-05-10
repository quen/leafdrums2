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
#include "time.h"
#include "stdlib.h"
#include "objbase.h"

#include "LeafDrums.h"

#include "MainFrm.h"
#include "LeafDrumsDoc.h"
#include "LeafDrumsView.h"

#include "PaletteDC.h"

#include "FXcode\Effect.h"

#include "LicenseDialog.h"
#include "Security.h"

#include "FileSystem.h"

#include "LogoWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLeafDrumsApp

BEGIN_MESSAGE_MAP(CLeafDrumsApp, CWinApp)
	//{{AFX_MSG_MAP(CLeafDrumsApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLeafDrumsApp construction

CLeafDrumsApp::CLeafDrumsApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CLeafDrumsApp object

CLeafDrumsApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CLeafDrumsApp initialization

BOOL CLeafDrumsApp::InitInstance()
{
	srand( (unsigned)time( NULL ) );

	CoInitialize(NULL);

	SetRegistryKey("leafdigital");

	// Handle license agreement
	if(!GetProfileInt("Settings","IAgreeToLicense",0))
	{
		CLicenseDialog dlg;
		CString strLicense;
		dlg.m_strLicense.LoadString(IDS_LICENSE1);
		strLicense.LoadString(IDS_LICENSE2);
		dlg.m_strLicense+=strLicense;
		strLicense.LoadString(IDS_LICENSE3);
		dlg.m_strLicense+=strLicense;
		strLicense.LoadString(IDS_LICENSE4);
		dlg.m_strLicense+=strLicense;
		if(dlg.DoModal()==IDCANCEL)
		{
			::MessageBox(NULL,
				"You may not use this program without "
				"accepting the license agreement.\n\n"
				"If you would like to accept the "
				"agreement, simply re-run leafDrums. If "
				"you do not accept the agreement, "
				"delete leafDrums from your system using Control Panel's Add/Remove Programs option.\n\n"
				"Note: the portions on distribution "
				"apply whether or not you 'accept' the "
				"agreement, so you may still not "
				"distribute leafDrums in any other manner "
				"than permitted there.",
				"License agreement not accepted",
				MB_OK|MB_ICONINFORMATION);

			return FALSE;
		}
		else
			WriteProfileInt(
			"Settings","IAgreeToLicense",1);
	}

	// Save directory name in registry
	WriteProfileString("Installation","Directory",CFileSystem::GetProgramFolder());

    new CRegistration(11,this);

	new CDisplayDevice();

	new CFileSystem();

	// Effect buffers for sin/triangle waves
	CalcWaveBuffers();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CLeafDrumsDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CLeafDrumsView));
	AddDocTemplate(pDocTemplate);

	RegisterShellFileTypes();
	EnableShellOpen();

	// Create loader window
	m_pMainWnd=new CLogoWnd();

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CLeafDrumsApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CLeafDrumsApp commands

int CLeafDrumsApp::ExitInstance() 
{
	delete CDisplayDevice::sm_pDisplayDevice;
	delete CFileSystem::sm_pCurrent;
	delete CRegistration::sm_pCurrent;

	CoUninitialize();

	return CWinApp::ExitInstance();
}

void CLeafDrumsApp::OnButtonNew()
{
	OnFileNew();
}

void CLeafDrumsApp::OnButtonOpen()
{
	// Change the name to something stupid first. 
	// This ensures that if you load the same file
	// again, it reverts, rather than ignoring it
	CDocument* pd=((CFrameWnd*)m_pMainWnd)->GetActiveDocument();
	CString sNameBefore=pd->GetPathName();
	pd->SetPathName(" "+sNameBefore);

	OnFileOpen();

	// If they didn't load it, put the name back
	CString sNameAfter=pd->GetPathName();
	if(sNameAfter==" "+sNameBefore)
		pd->SetPathName(sNameBefore);
}

void CLeafDrumsApp::ContinueLoading()
{
	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	ProcessShellCommand(cmdInfo);
//	if (!ProcessShellCommand(cmdInfo))
//		return FALSE;
}
