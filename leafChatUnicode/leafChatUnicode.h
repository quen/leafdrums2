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
// leafChatUnicode.h : main header file for the LEAFCHATUNICODE DLL
//

#if !defined(AFX_LEAFCHATUNICODE_H__81BA3A17_988E_4549_A1EE_082465730BEF__INCLUDED_)
#define AFX_LEAFCHATUNICODE_H__81BA3A17_988E_4549_A1EE_082465730BEF__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

#include "leafChatUnicodeExport.h"

// Exports
#define DllExport __declspec( dllexport ) 
DllExport HWND CreateEdit(HWND hwndParent,OwnerProc op,void* opData,UINT uAdditionalStyles);
DllExport int GetTextLength(HWND hwndEdit);
DllExport void GetText(HWND hwndEdit,wchar_t* pwcBuffer,int iBufferSize);
DllExport void SetFontName(HWND hwndEdit,const char* pcFont);
DllExport void SetEnabled(HWND hwndEdit,BOOL bEnabled);
DllExport void Move(HWND hwndEdit,RECT rLocation);
DllExport BOOL PreTranslateMessage(HWND hwndEdit,MSG* pMsg);
DllExport int GetTextWidth(CFont* pFont,wchar_t* pwsText,int& iHeight);
DllExport void DrawText(CBitmap& bitmap,CFont& font,int iWidth,int iHeight,wchar_t* pwsText,RECT* pRect);

/////////////////////////////////////////////////////////////////////////////
// CLeafChatUnicodeApp
// See leafChatUnicode.cpp for the implementation of this class
//

class CLeafChatUnicodeApp : public CWinApp
{
public:
	CLeafChatUnicodeApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLeafChatUnicodeApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CLeafChatUnicodeApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LEAFCHATUNICODE_H__81BA3A17_988E_4549_A1EE_082465730BEF__INCLUDED_)
