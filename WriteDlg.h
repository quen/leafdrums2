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
#if !defined(AFX_WRITEDLG_H__74DA959A_44F9_48B4_9AFF_D9F224B4F116__INCLUDED_)
#define AFX_WRITEDLG_H__74DA959A_44F9_48B4_9AFF_D9F224B4F116__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "Track.h"

/////////////////////////////////////////////////////////////////////////////
// CWriteDlg dialog

class CWriteDlg : public CDialog
{
// Construction
public:
	CWriteDlg(CSong& song,CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CWriteDlg)
	enum { IDD = IDD_WRITE };
	int		m_iCount;
	int		m_iOutputStyle;
	int		m_iFormat;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWriteDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CSong& m_song;

	// Generated message map functions
	//{{AFX_MSG(CWriteDlg)
	afx_msg void OnChangeCount();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WRITEDLG_H__74DA959A_44F9_48B4_9AFF_D9F224B4F116__INCLUDED_)
