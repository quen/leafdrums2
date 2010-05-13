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
#if !defined(AFX_INPUTWNDUNICODE_H__D6F95C37_E9ED_4CE5_83BA_D63536CCE3F7__INCLUDED_)
#define AFX_INPUTWNDUNICODE_H__D6F95C37_E9ED_4CE5_83BA_D63536CCE3F7__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// InputWndUnicode.h : header file
//

#include "leafChatUnicodeExport.h"

/////////////////////////////////////////////////////////////////////////////
// CInputWndUnicode window

#define HISTORYBUFFER 32

class CInputWndUnicode : public CEdit
{
// Construction
public:
	CInputWndUnicode(OwnerProc op,void* opData);

// Attributes
public:

// Operations
public:
	void SetFontName(CString strFontName);
	void Paste();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInputWndUnicode)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CInputWndUnicode();

	// Generated message map functions
protected:
	//{{AFX_MSG(CInputWndUnicode)
	afx_msg void OnDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

	CString m_strFont;
	CFont* m_pFont;

	CBrush* m_pBrush;

	CString m_strHistory[HISTORYBUFFER];
	int m_iHistoryPos;
	int m_iHistoryNext;
	void AddHistory(const TCHAR* pstrLine);

	void* NotifyOwner(int iMessage,const void* pData);

	OwnerProc m_op;
	void* m_opData;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INPUTWNDUNICODE_H__D6F95C37_E9ED_4CE5_83BA_D63536CCE3F7__INCLUDED_)
