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
#if !defined(AFX_SETTEMPODLG_H__027E0EF7_FCDD_46FF_A311_B86F1DA9D03F__INCLUDED_)
#define AFX_SETTEMPODLG_H__027E0EF7_FCDD_46FF_A311_B86F1DA9D03F__INCLUDED_

#include "Song.h"
#include "SongHandler.h"

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// SetTempoDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSetTempoDlg dialog

class CSetTempoDlg : public CDialog
{
// Construction
public:
	CSetTempoDlg(CSongHandler* pshHandler,CSong* psSong,int iBar,CWnd* pwParent);   

	CSong* m_psSong;
	CSongHandler* m_pshHandler;
	int m_iBar;

// Dialog Data
	//{{AFX_DATA(CSetTempoDlg)
	enum { IDD = IDD_SETTEMPO };
	int		m_iTempo;
	int		m_iTimeSignature;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetTempoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSetTempoDlg)
	afx_msg void OnApply();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETTEMPODLG_H__027E0EF7_FCDD_46FF_A311_B86F1DA9D03F__INCLUDED_)
