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
#if !defined(AFX_CUSTOMEDIT_H__5CAC4607_DB67_48E6_8F1C_94E20ECA6EC3__INCLUDED_)
#define AFX_CUSTOMEDIT_H__5CAC4607_DB67_48E6_8F1C_94E20ECA6EC3__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// CustomEdit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCustomEdit window

class CCustomEdit : public CEdit
{
// Construction
public:
	CCustomEdit();

// Attributes
public:

// Operations
public:
	BOOL Create(int iX,int iY,int iWidth,int iHeight,CWnd* pParentWnd, UINT nID);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCustomEdit)
	public:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCustomEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CCustomEdit)
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnDestroy();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

	CFont m_fDefault;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CUSTOMEDIT_H__5CAC4607_DB67_48E6_8F1C_94E20ECA6EC3__INCLUDED_)
