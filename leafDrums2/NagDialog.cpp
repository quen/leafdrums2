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
#include "LeafDrums.h"
#include "NagDialog.h"
#include "htmlhelp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define URL_ORDERING "http://www.leafdigital.com/software/leafdrums/order.html"

/////////////////////////////////////////////////////////////////////////////
// CNagDialog dialog


CNagDialog::CNagDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CNagDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNagDialog)
	//}}AFX_DATA_INIT
}


void CNagDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNagDialog)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNagDialog, CDialog)
	//{{AFX_MSG_MAP(CNagDialog)
	ON_BN_CLICKED(IDC_ORDERONLINE, OnOrderOnline)
	ON_BN_CLICKED(IDC_OTHER, OnOther)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNagDialog message handlers

void CNagDialog::OnOrderOnline() 
{
	CString strURL=URL_ORDERING;

	// Execute URL
	if((int)(::ShellExecute(
		AfxGetMainWnd()->GetSafeHwnd(),"open",
		strURL,NULL,NULL,0))<=32)
		::MessageBox(AfxGetMainWnd()->GetSafeHwnd(),
			"There was a problem opening the "
			"ordering page.\n\nCheck your web "
			"browser is set up correctly; you "
			"should be able to type a URL into "
			"the Run box (from the Windows Start menu) "
			"and have it load.\n\nIf you can't get it "
			"to work, you can manually load the URL:\n\n"
			URL_ORDERING,"URL Error",
			MB_OK|MB_ICONEXCLAMATION);
}

void CNagDialog::OnOther() 
{
	// Obtain path of program
	char cBuffer[512];
	VERIFY(GetModuleFileName(NULL,cBuffer,sizeof(cBuffer))!=0);
	CString strFolder=cBuffer;
	int iPos=strFolder.ReverseFind('\\');
	ASSERT(iPos!=-1);
	strFolder=strFolder.Left(iPos+1);
	strFolder.MakeLower();

	// Run HTML help
	HtmlHelp(
		GetSafeHwnd(),
		strFolder+"leafDrums2.chm::/program/ordering.html>standard",
		HH_DISPLAY_TOPIC,
		NULL);



/*

	// Execute URL
	if((int)(::ShellExecute(
		AfxGetMainWnd()->GetSafeHwnd(),"open",
		strFolder+"help/ordering.html",NULL,NULL,0))<=32)
		::MessageBox(AfxGetMainWnd()->GetSafeHwnd(),
			"There was a problem opening the "
			"help page about ordering methods.\n\nCheck your web "
			"browser is set up correctly; you "
			"should be able to type a URL into "
			"the Run box (from the Windows Start menu) "
			"and have it load.\n\nIf you can't get it "
			"to work, you can manually load the file "
			"ordering.html in the leafDrums/help directory.",
			"URL Error",
			MB_OK|MB_ICONEXCLAMATION);
			*/
}
