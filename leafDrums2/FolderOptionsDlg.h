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
#if !defined(AFX_FOLDEROPTIONSDLG_H__225247B2_FC4A_40F5_B330_3AFC64B4471C__INCLUDED_)
#define AFX_FOLDEROPTIONSDLG_H__225247B2_FC4A_40F5_B330_3AFC64B4471C__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// FolderOptionsDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFolderOptionsDlg dialog

class CFolderOptionsDlg : public CDialog
{
// Construction
public:
	CFolderOptionsDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFolderOptionsDlg)
	enum { IDD = IDD_FOLDEROPTIONSDIALOG };
	CListBox	m_lbFolders;
	CString	m_sFolder;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFolderOptionsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFolderOptionsDlg)
	afx_msg void OnAdd();
	afx_msg void OnDel();
	afx_msg void OnSelchangeFolderList();
	afx_msg void OnSetDefault();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	void FillList();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FOLDEROPTIONSDLG_H__225247B2_FC4A_40F5_B330_3AFC64B4471C__INCLUDED_)
