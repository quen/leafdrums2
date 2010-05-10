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
// LeafDrums.h : main header file for the CUTEDRUMS application
//

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CLeafDrumsApp:
// See LeafDrums.cpp for the implementation of this class
//

class CLeafDrumsApp : public CWinApp
{
public:
	CLeafDrumsApp();
	void OnButtonNew();
	void OnButtonOpen();
	void OnButtonSave();

	void ContinueLoading();
	void SetMainWnd(CWnd* pMain) { m_pMainWnd=pMain; }

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLeafDrumsApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CLeafDrumsApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

#define CLIENTWIDTH 780
#define CLIENTHEIGHT 538

#define COPYRIGHTHEIGHT 13
#define CONTROLHEIGHT 39
#define OTHERHEIGHT 499 // Main area below scrollbar

#define BMPDETAILS_SONGSCREEN_BUTTONSWIDTH 772
#define BMPDETAILS_SONGSCREEN_BUTTONSHEIGHT 479
#define BMPDETAILS_SONGSCREEN_BUTTONSHEIGHT2 472



#define SONGHANDLER_HEIGHT 145
