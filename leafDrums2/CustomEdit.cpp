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
#include "stdafx.h"
#include "leafdrums.h"
#include "CustomEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCustomEdit

CCustomEdit::CCustomEdit()
{
	m_fDefault.CreateFont(-11,0,0,0,FW_NORMAL,FALSE,FALSE,FALSE,DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH|
		FF_DONTCARE,"Verdana");
}

CCustomEdit::~CCustomEdit()
{
}


BEGIN_MESSAGE_MAP(CCustomEdit, CEdit)
	//{{AFX_MSG_MAP(CCustomEdit)
	ON_WM_KILLFOCUS()
	ON_WM_KEYUP()
	ON_WM_DESTROY()
	ON_WM_KEYDOWN()
	ON_WM_CHAR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCustomEdit message handlers

void CCustomEdit::OnKillFocus(CWnd* pNewWnd) 
{
	CEdit::OnKillFocus(pNewWnd);
	
	GetParent()->PostMessage(WM_COMMAND,(BN_CLICKED << 16) | GetDlgCtrlID(),
		(LPARAM)GetSafeHwnd());
}

void CCustomEdit::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if(nChar==VK_RETURN)
		GetParent()->PostMessage(WM_COMMAND,(BN_CLICKED << 16) | GetDlgCtrlID(),
			(LPARAM)GetSafeHwnd());
	else
		CEdit::OnKeyUp(nChar, nRepCnt, nFlags);
}

// Autodelete
void CCustomEdit::OnDestroy() 
{
	CEdit::OnDestroy();

	delete this;	
}

// Create sensibly
BOOL CCustomEdit::Create(int iX,int iY,int iWidth,int iHeight,CWnd* pParentWnd, UINT nID)
{
	CRect r(iX,iY,iX+iWidth,iY+iHeight);
	BOOL bOK=CEdit::Create(WS_CHILD|WS_VISIBLE|ES_CENTER,r,pParentWnd,nID);
	ModifyStyleEx(0,WS_EX_TRANSPARENT);
	if(!bOK) return FALSE;

	SetFont(&m_fDefault);
	return TRUE;
}

void CCustomEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if(nChar!=VK_RETURN)
		CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CCustomEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if(nChar!=VK_RETURN)
		CEdit::OnChar(nChar, nRepCnt, nFlags);
}
