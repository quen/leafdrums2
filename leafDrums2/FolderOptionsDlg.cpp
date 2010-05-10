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
#include "leafdrums.h"
#include "FolderOptionsDlg.h"

#include "FileSystem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFolderOptionsDlg dialog


CFolderOptionsDlg::CFolderOptionsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFolderOptionsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFolderOptionsDlg)
	m_sFolder = _T("");
	//}}AFX_DATA_INIT
}


void CFolderOptionsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFolderOptionsDlg)
	DDX_Control(pDX, IDC_FOLDERLIST, m_lbFolders);
	DDX_Text(pDX, IDC_FOLDEREDIT, m_sFolder);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFolderOptionsDlg, CDialog)
	//{{AFX_MSG_MAP(CFolderOptionsDlg)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_DEL, OnDel)
	ON_LBN_SELCHANGE(IDC_FOLDERLIST, OnSelchangeFolderList)
	ON_BN_CLICKED(IDC_SETDEFAULT, OnSetDefault)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFolderOptionsDlg message handlers

void CFolderOptionsDlg::OnAdd() 
{
	UpdateData();
	if(m_sFolder=="")
	{
		MessageBox(
			"To add a folder, type the precise folder name in the blank box "
			"by the side of Add, before you click the Add button.",
			"Folder name not specified",
			MB_OK|MB_ICONINFORMATION);
		GetDlgItem(IDC_FOLDEREDIT)->SetFocus();
		return;
	}

	CFileSystem::sm_pCurrent->AddRoot(m_sFolder);
	m_sFolder="";
	UpdateData(FALSE);
	FillList();
}

void CFolderOptionsDlg::OnDel() 
{
	int iSel=m_lbFolders.GetCurSel();
	CFileSystem::sm_pCurrent->DeleteRoot(iSel);
	FillList();
}

void CFolderOptionsDlg::OnSelchangeFolderList() 
{
	// Enable/disable buttons
	GetDlgItem(IDC_SETDEFAULT)->EnableWindow(
		m_lbFolders.GetCurSel()!=-1);
	GetDlgItem(IDC_DEL)->EnableWindow(
		m_lbFolders.GetCurSel()!=-1 && m_lbFolders.GetCount()>1);	
}

void CFolderOptionsDlg::OnSetDefault() 
{
	int iSel=m_lbFolders.GetCurSel();
	CFileSystem::sm_pCurrent->SetDefaultRoot(iSel);
	FillList();
}

BOOL CFolderOptionsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// Add data to list
	FillList();

	return TRUE;  // return TRUE unless you set the focus to a control
}

void CFolderOptionsDlg::FillList()
{
	UpdateData();

	// Get current selection
	CString sSelected;
	if(m_lbFolders.GetCurSel()!=LB_ERR)
		m_lbFolders.GetText(m_lbFolders.GetCurSel(),sSelected);

	// Clear list and fill it with current data
	m_lbFolders.ResetContent();
	for(int i=0;i<CFileSystem::sm_pCurrent->GetNumRoots();i++)
	{
		m_lbFolders.AddString(
			CFileSystem::sm_pCurrent->GetRoot(i));
	}

	// Select previous selection, if possible
	int iSel=m_lbFolders.FindStringExact(-1,sSelected);
	if(iSel!=-1)
		m_lbFolders.SetCurSel(iSel);

	// Reset button enable state
	OnSelchangeFolderList();		
}

