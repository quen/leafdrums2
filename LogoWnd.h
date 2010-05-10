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
#if !defined(AFX_LOGOWND_H__512379B5_5B0A_4D51_83DC_8C8EC072FA98__INCLUDED_)
#define AFX_LOGOWND_H__512379B5_5B0A_4D51_83DC_8C8EC072FA98__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "GraphicOnscreen.h"

/////////////////////////////////////////////////////////////////////////////
// CLogoWnd window

class CLogoWnd : public CWnd
{
// Construction
public:
	CLogoWnd();

// Attributes
public:
	static CLogoWnd* sm_pCurrent;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLogoWnd)
	public:
	virtual BOOL Create();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CLogoWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CLogoWnd)
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	BOOL m_bFirst;
	CGraphicOnscreen* m_pgoLogo;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOGOWND_H__512379B5_5B0A_4D51_83DC_8C8EC072FA98__INCLUDED_)
