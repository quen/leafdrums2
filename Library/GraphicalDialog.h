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
#if !defined(AFX_GRAPHICALDIALOG_H__B9658201_8D16_11D2_B7F3_DCEB5E45FA9B__INCLUDED_)
#define AFX_GRAPHICALDIALOG_H__B9658201_8D16_11D2_B7F3_DCEB5E45FA9B__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "GraphicalOutputSink.h"

#define CGraphicalDialog_MAXMOVEABLES 128

class CGraphicalDialog : public CGraphicalOutputSink
{
// Construction
public:
	CGraphicalDialog();

// Attributes
public:
	int DoModal(const char* pstrResourceName,CWnd* pParentWnd);
	void EndDialog(int iReturn);
	CString ResourceName();

// Operations
public:
	// Set to a new resource file (returns window that would get
	// focus if dialog was newly created)
	HWND SetResource(const char* pstrName);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGraphicalDialog)
	public:
	virtual BOOL Create(const char* pstrResourceName,CWnd* pParentWnd);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGraphicalDialog();

	// Generated message map functions
protected:
	//{{AFX_MSG(CGraphicalDialog)
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg UINT OnNcHitTest(CPoint point);
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClose();
	afx_msg void OnOK();
	afx_msg void OnCancel();
	afx_msg LONG OnInitDialogInternal(UINT,LONG);
	afx_msg HBRUSH OnCtlColor( CDC* pDC, CWnd* pWnd, UINT nCtlColor );
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();


	// Font
	CFont m_Font;

	// Name of resource data
	CString m_strResourceName;

	// Graphics data
	int m_iWidth,m_iHeight;
	CGraphic * m_pgBackground;
	CBrush * m_pbrNormal, * m_pbrFaded;
	int m_iR,m_iG,m_iB;

	// Is window currently active?
	BOOL m_fActive;

	int m_iNumMoveables;
	CRect m_rectMoveable[CGraphicalDialog_MAXMOVEABLES];

	// Modal information
	BOOL m_fModal;
	int m_iEndDialog;
};

void AFXAPI DDX_GraphicCheckbox(CDataExchange* pDX, int nIDC, int& value);

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRAPHICALDIALOG_H__B9658201_8D16_11D2_B7F3_DCEB5E45FA9B__INCLUDED_)
