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
// leafChatUnicode.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "leafChatUnicode.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "InputWndUnicode.h"

//
//	Note!
//
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

DllExport HWND CreateEdit(HWND hwndParent,OwnerProc op,void* opData,UINT uAdditionalStyles)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CInputWndUnicode* pwndEdit=new CInputWndUnicode(op,opData);
	CRect rPosition(0,0,1,1);
	pwndEdit->Create(WS_CHILD|uAdditionalStyles|ES_AUTOHSCROLL,
		rPosition,CWnd::FromHandle(hwndParent),37331);
	return pwndEdit->GetSafeHwnd();
}

DllExport void SetFontName(HWND hwndEdit,const char* pcFont)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CInputWndUnicode* pwndEdit=(CInputWndUnicode*)CWnd::FromHandle(hwndEdit);
	CString sFont(pcFont);
	pwndEdit->SetFontName(sFont);
}

DllExport int GetTextLength(HWND hwndEdit)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CInputWndUnicode* pwndEdit=(CInputWndUnicode*)CWnd::FromHandle(hwndEdit);
	return pwndEdit->GetWindowTextLength();
}

DllExport void GetText(HWND hwndEdit,wchar_t* pwcBuffer,int iBufferSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CInputWndUnicode* pwndEdit=(CInputWndUnicode*)CWnd::FromHandle(hwndEdit);

	CString sText;
	pwndEdit->GetWindowText(sText);
	for(int i=0;i<sText.GetLength() && i<(iBufferSize-1);i++)
	{
		pwcBuffer[i]=sText.GetAt(i);
	}
	pwcBuffer[i]=0;
}

DllExport void SetEnabled(HWND hwndEdit,BOOL bEnabled)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CInputWndUnicode* pwndEdit=(CInputWndUnicode*)CWnd::FromHandle(hwndEdit);

	pwndEdit->EnableWindow(bEnabled);
}

DllExport void Move(HWND hwndEdit,RECT rLocation)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CInputWndUnicode* pwndEdit=(CInputWndUnicode*)CWnd::FromHandle(hwndEdit);

	pwndEdit->MoveWindow(&rLocation);
}

DllExport BOOL PreTranslateMessage(HWND hwndEdit,MSG* pMsg)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CInputWndUnicode* pwndEdit=(CInputWndUnicode*)CWnd::FromHandle(hwndEdit);

	return pwndEdit->PreTranslateMessage(pMsg);
}

DllExport int GetTextWidth(CFont* pFont,wchar_t* pwsText,int& iHeight)
{
	// Obtain width and height
	CDC dc;
	dc.CreateDC(_T("DISPLAY"),NULL,NULL,NULL);
	dc.SelectObject(*pFont);
	CRect rect(0,0,0,0);
	iHeight=dc.DrawText(pwsText,rect,DT_CALCRECT|DT_LEFT|DT_SINGLELINE|DT_NOPREFIX);
	return rect.right;
}

DllExport void DrawText(CBitmap& bitmap,CFont& font,int iWidth,int iHeight,wchar_t* pwsText,RECT* pRect)
{
	// Create memory DC using bitmap
	CDC dcMemory;
	dcMemory.CreateCompatibleDC(NULL);
	dcMemory.SelectObject(&bitmap);
	dcMemory.SelectObject(&font);

	// Clear bitmap
	dcMemory.SelectObject((HBRUSH)GetStockObject(BLACK_BRUSH));
	dcMemory.SelectObject((HPEN)GetStockObject(NULL_PEN));
	dcMemory.Rectangle(0,0,iWidth+16+1,iHeight+1);

	// Draw using font into bitmap
	dcMemory.SetTextColor(RGB(255,255,255));
	dcMemory.SetBkMode(TRANSPARENT);
	dcMemory.DrawText(pwsText,pRect,DT_LEFT|DT_SINGLELINE|DT_NOPREFIX);
}



/////////////////////////////////////////////////////////////////////////////
// CLeafChatUnicodeApp

BEGIN_MESSAGE_MAP(CLeafChatUnicodeApp, CWinApp)
	//{{AFX_MSG_MAP(CLeafChatUnicodeApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLeafChatUnicodeApp construction

CLeafChatUnicodeApp::CLeafChatUnicodeApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CLeafChatUnicodeApp object

CLeafChatUnicodeApp theApp;
