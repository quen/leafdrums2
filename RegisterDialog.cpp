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
// RegisterDialog.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "RegisterDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRegisterDialog dialog


CRegisterDialog::CRegisterDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CRegisterDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRegisterDialog)
	m_strName = _T("");
	m_iNumber = 0;
	//}}AFX_DATA_INIT
}


void CRegisterDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRegisterDialog)
	DDX_Control(pDX, IDC_NAMEEDIT, m_editName);
	DDX_Text(pDX, IDC_NAMEEDIT, m_strName);
	DDX_Text(pDX, IDC_NUMBEREDIT, m_iNumber);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRegisterDialog, CDialog)
	//{{AFX_MSG_MAP(CRegisterDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRegisterDialog message handlers

BOOL CRegisterDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_editName.SetFocus();
	
	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRegisterDialog::OnOK() 
{
	if(!UpdateData(TRUE)) return;
	
	AfxGetApp()->WriteProfileString("Registration",
		"Name",m_strName);
	AfxGetApp()->WriteProfileInt("Registration",
		"Number",m_iNumber);

	AfxMessageBox("Your registration details have "
		"been saved. Next time you load the "
		"program, no nag screens will appear "
		"(assuming the information was entered "
		"correctly).\n\n"
		"If you entered the information "
		"incorrectly, you can simply re-enter it.\n\n"
		"Please make sure you've written down the "
		"information somewhere, in case you need "
		"to reinstall this program.");
	
	CDialog::OnOK();
}
