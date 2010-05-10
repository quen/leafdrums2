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
#ifndef ONESHOTDIALOG_H
#define ONESHOTDIALOG_H
#include "resource.h"

class COneShotDialog : public CDialog
{
// Construction
public:
	COneShotDialog(DWORD dwMessageID,
		CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(COneShotDialog)
	enum { IDD = IDD_ONESHOTDLG };
	CString	m_strText;
	BOOL	m_fStopShowing;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COneShotDialog)
	public:
	virtual int DoModal();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	DWORD m_dwID;

	// Generated message map functions
	//{{AFX_MSG(COneShotDialog)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // ONESHOTDIALOG_H
