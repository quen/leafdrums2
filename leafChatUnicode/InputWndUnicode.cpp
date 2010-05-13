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
// InputWndUnicode.cpp : implementation file
//

#include "stdafx.h"
#include "leafChatUnicode.h"
#include "InputWndUnicode.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define CHARCODE_COLOUR		0x03	// Ctrl-C
#define CHARCODE_BOLD		0x02	// Ctrl-B
#define CHARCODE_ITALIC		0x1F	// Ctrl-_
#define CHARCODE_REVERSE	0x16	// Ctrl-V
#define CHARCODE_UNDERLINE	0x10	// not available
#define CHARCODE_NORMAL		0x0f	// Ctrl-O
#define CHARCODE_NICKNAME	0x08	// Shouldn't be recv'd
#define CHARCODE_SMALL		0x04	// Shouldn't be recv'd



/////////////////////////////////////////////////////////////////////////////
// CInputWndUnicode

CInputWndUnicode::CInputWndUnicode(OwnerProc op,void* opData)
{
	m_iHistoryNext=0;
	m_iHistoryPos=0;
	m_op=op;
	m_opData=opData;
	m_pBrush=new CBrush;
	m_pFont=NULL;
}

CInputWndUnicode::~CInputWndUnicode()
{
	delete m_pBrush;
	if(m_pFont!=NULL) delete m_pFont;
}

void* CInputWndUnicode::NotifyOwner(int iMessage,const void* pData)
{
	return (*m_op)(m_opData,iMessage,pData);
}

void CInputWndUnicode::SetFontName(CString strFontName)
{
	RECT rectClient;
	GetClientRect(&rectClient);

	// Get height
	int iHeight;
	if(strFontName.Find(_T(":"))!=-1)
	{
		CString strNumber=strFontName.Left(strFontName.Find(_T(":")));
		iHeight=-_wtoi(strNumber);
		strFontName=strFontName.Mid(strFontName.Find(_T(":"))+1);
	}
	else
	{
		iHeight=-15;
	}

	// Create a font by that name
	if(m_pFont!=NULL) delete m_pFont;
	m_pFont=new CFont();
	VERIFY(m_pFont->CreateFont(iHeight,0,0,0,
		FW_NORMAL,
		0,
		0,
		0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,DEFAULT_PITCH|FF_DONTCARE,strFontName));

	// Set this
	SetFont(m_pFont);		

	Invalidate();
}

void CInputWndUnicode::AddHistory(const TCHAR* pstrLine)
{
	// Set next history position to this
	m_strHistory[m_iHistoryNext++]=pstrLine;

	// Wrap around buffer
	if(m_iHistoryNext>=HISTORYBUFFER) 
		m_iHistoryNext=0;

	// Reset history-pos
	m_iHistoryPos=m_iHistoryNext;
}


BEGIN_MESSAGE_MAP(CInputWndUnicode, CEdit)
	//{{AFX_MSG_MAP(CInputWndUnicode)
	ON_WM_DESTROY()
	ON_WM_CREATE()
	ON_WM_CHAR()
	ON_WM_KEYDOWN()
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInputWndUnicode message handlers

void CInputWndUnicode::OnDestroy() 
{
	CEdit::OnDestroy();
	
	delete this;	
}

int CInputWndUnicode::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CEdit::OnCreate(lpCreateStruct) == -1)
		return -1;

	PostMessage(WM_TIMER,1234);
	
	return 0;
}

void CInputWndUnicode::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CString strEntry="!";

	switch(nChar)
	{
	case 13:	// Enter
		GetWindowText(strEntry);
		if(strEntry==_T("")) break;


		AddHistory(strEntry);

		NotifyOwner(IW_RETURN,0);

		// Let parent process contents
//		CIdle::sm_pCurrent->InformUserAction();
//		GetParent()->SendMessage(WM_CHAR,
//			13,0);

		// Clear window contents
		SetWindowText(_T(""));
		break;
	case 2 :	// ^B
		strEntry.SetAt(0,CHARCODE_BOLD);
		ReplaceSel(strEntry);
		break;
	case 18:	// ^R
		strEntry.SetAt(0,CHARCODE_REVERSE);
		ReplaceSel(strEntry);
		break;
	case 19:	// ^S
		strEntry.SetAt(0,CHARCODE_NORMAL);
		ReplaceSel(strEntry);
		break;
	case 11:	// ^K
		strEntry.SetAt(0,CHARCODE_COLOUR);
		ReplaceSel(strEntry);
		break;
	case 9:	// Tab
		{
			// Obtain the text...
			CString strText;
			GetWindowText(strText);

			// ...and the cursor position
			int iStart,iEnd;
			GetSel(iStart,iEnd);
			if(iStart!=iEnd) 
			{
				MessageBeep(-1);
				break;
			}

			// Look from the cursor position 
			// backward to find a space or start
			// of string
			for(int iPos=iEnd-1;iPos>=0 && 
				strText[iPos]!=' ' &&
				strText[iPos]!=',';iPos--) ;
			iPos++;

			// Extract that text
			strText=strText.Mid(iPos,iEnd-iPos);

			char* pcText=new char[strText.GetLength()+1];
			for(int i=0;i<strText.GetLength();i++)
			{
				wchar_t wc=strText.GetAt(i);
				if(wc>254) 
					pcText[i]='?';
				else
					pcText[i]=(char)wc;
			}
			pcText[i]=0;

			char* pcExtra=(char*)NotifyOwner(IW_COMPLETENICK,pcText);
			if(pcExtra!=NULL)
			{
				CString sExtra(pcExtra);
				ReplaceSel(sExtra);
			}
			else
			{
				MessageBeep(-1);
			}

/*			// Note: the CompleteNick functions
			// return TRUE and an empty string if
			// the nick is not unique, FALSE if
			// the nick is not found

			// Get parent window
			CChanWnd* pwndChan=
				dynamic_cast<CChanWnd*>(GetParent());
			if(pwndChan!=NULL &&
				// Ask channel for completions
				pwndChan->CompleteNick(strText))
			{
				ReplaceSel(strText);
				break;
			}

			CMessageWnd* pwndMessage=
				dynamic_cast<CMessageWnd*>(GetParent());
			if(pwndMessage!=NULL &&
				// Ask query for completions
				pwndMessage->CompleteNick(strText))
			{
				ReplaceSel(strText);
				break;
			}

			CDCCChatWnd* pwndDCC=
				dynamic_cast<CDCCChatWnd*>(GetParent());
			if(pwndDCC!=NULL &&
				// Ask DCC wnd for completions
				pwndDCC->CompleteNick(strText))
			{
				ReplaceSel(strText);
				break;
			}

			if(CRecentSenders::sm_pCurrent->
				CompleteNick(strText))
			{
				ReplaceSel(strText);
				break;
			}*/
		}
		break;

	case VK_BACK : // Backspace - always allow
		CEdit::OnChar(nChar, nRepCnt, nFlags);
		break;

	default:
		GetWindowText(strEntry);
		if(strEntry.GetLength()>=450)
			MessageBeep(-1);
		else
			CEdit::OnChar(nChar, nRepCnt, nFlags);
		break;
	}
}

void CInputWndUnicode::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	switch(nChar)
	{
	case 38:	// Up
		// See if we're at the top, if so, store this in buffer
		if(m_iHistoryPos==m_iHistoryNext)
		{
			CString strEntry;
			GetWindowText(strEntry);
			if(strEntry!="") 
			{
				AddHistory(strEntry);
				m_iHistoryPos--;
			}
		}

		// Move pos
		m_iHistoryPos--;
		if(m_iHistoryPos<0) m_iHistoryPos+=HISTORYBUFFER;

		// If we reached a blank bit or max, move it back :p
		if((m_iHistoryPos==m_iHistoryNext) ||
		   (m_strHistory[m_iHistoryPos]==""))
		{
			m_iHistoryPos++;
			m_iHistoryPos%=HISTORYBUFFER;
			return;
		}

		// Set text appropriately
		SetWindowText(m_strHistory[m_iHistoryPos]);
		SetSel(0,-1);

		return;
	case 40:	// Down
		// If we already reached base, just clear
		if(m_iHistoryPos==m_iHistoryNext)
		{
			// Save the current line if it wasn't from history
			CString strEntry;
			GetWindowText(strEntry);
			if(strEntry!="" && strEntry!=m_strHistory[m_iHistoryPos]) 
				AddHistory(strEntry);

			SetWindowText(_T(""));
			return;
		}

		// Otherwise, move pos back down
		m_iHistoryPos++;
		m_iHistoryPos%=HISTORYBUFFER;

		// Set text appropriately
		SetWindowText(m_strHistory[m_iHistoryPos]);
		SetSel(0,-1);
		return;
	case 33:
		NotifyOwner(IW_PAGEUP,0);
//		if(m_pwndOutput) m_pwndOutput->PageUp();
		break;
	case 34:
		NotifyOwner(IW_PAGEDOWN,0);
//		if(m_pwndOutput) m_pwndOutput->PageDown();
		break;
	case 36 :	// Home
		if(GetKeyState(VK_CONTROL)&0xf000)
			NotifyOwner(IW_CTRLHOME,0);
//		if(m_pwndOutput) 
//				m_pwndOutput->Home();
		break;
	case 35:	// End
		if(GetKeyState(VK_CONTROL)&0xf000)
			NotifyOwner(IW_CTRLEND,0);
//		if(m_pwndOutput) 
//			if(GetKeyState(VK_CONTROL)&0xf000)
//				m_pwndOutput->End();
		break;
	default:
		break;
	}

	CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL CInputWndUnicode::PreTranslateMessage(MSG* pMsg) 
{
	CString strEntry;

	if(pMsg->message==WM_KEYDOWN)
	{
		switch(pMsg->wParam)
		{
		// Current copy/paste shortcuts

		case 'C' :	// ^C
			if(GetKeyState(VK_CONTROL)&0xf000)
			{
				Copy();
				return TRUE;
			}
			break;
		case 'X':	// ^X
			if(GetKeyState(VK_CONTROL)&0xf000)
			{
				Cut();
				return TRUE;
			}
			break;
		case 'V':	// ^V
			if(GetKeyState(VK_CONTROL)&0xf000)
			{
				Paste();

				return TRUE;
			}
			break;

		// Win 3.0 copy/paste shortcuts (back by dope demand)

		case VK_INSERT:	
			if(GetKeyState(VK_CONTROL)&0xf000) // Ctrl-Ins
			{
				Copy();
				return TRUE;
			}
			else if(GetKeyState(VK_SHIFT)&0xf000) // Shift-Ins
			{
				Paste();

				return TRUE;
			}
			break;

		case VK_DELETE: 
			if(GetKeyState(VK_SHIFT)&0xf000) // Shift-Del
			{
				Cut();
				return TRUE;
			}
			break;


		// Ctrl-I (so we can treat it differently to tab)

		case 'I' :	// ^I
			if(GetKeyState(VK_CONTROL)&0xf000)
			{
				strEntry=_T(" ");
				strEntry.SetAt(0,CHARCODE_ITALIC);
				ReplaceSel(strEntry);

				return TRUE;
			}
			break;
		}
	}
	
	return CEdit::PreTranslateMessage(pMsg);
}

HBRUSH CInputWndUnicode::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
/*	// Get scheme, give up if we don't have one
	CScheme* pScheme=CSchemeManager::sm_pCurrent->Scheme(GetParent());
	if(pScheme==NULL) return NULL;

	// Set text colour
	pDC->SetTextColor(pScheme->GetColourMap().Colour(1));
	pDC->SetBkColor(pScheme->GetBackground());

	// Set up our brush
	delete m_pBrush;
	m_pBrush=new CBrush(pScheme->GetBackground());*/

	COLORREF 
		clrFG=(COLORREF)NotifyOwner(IW_GETFOREGROUND,0),
		clrBG=(COLORREF)NotifyOwner(IW_GETBACKGROUND,0);

	// Set text colour
	pDC->SetTextColor(clrFG);
	pDC->SetBkColor(clrBG);

	// Set up our brush
	delete m_pBrush;
	m_pBrush=new CBrush(clrBG);

	return (HBRUSH)(*m_pBrush);
}

// Override built-in paste so that we support multi-line paste nicely
void CInputWndUnicode::Paste()
{
	// Obtain text from clipboard
	if (!OpenClipboard())
	{
		AfxMessageBox( _T("Cannot open the Clipboard") );
		return;
	}
	CString sText;

	HANDLE hText=GetClipboardData(CF_UNICODETEXT);
	if(hText==NULL) 
	{
		hText=GetClipboardData(CF_TEXT);
		if(hText==NULL)
		{
			// If no text format, we don't support
			CloseClipboard();
			return;
		}
		// Extract text
		sText=(char*)GlobalLock(hText);
		GlobalUnlock(hText);
		CloseClipboard();
	}
	else
	{
		// Extract text
		sText=(wchar_t*)GlobalLock(hText);
		GlobalUnlock(hText);
		CloseClipboard();
	}

	// See if text is a single line
	int iCR=sText.Find(_T("\r\n"));
	if(iCR==-1)
	{
		// Single line, just call built-in paste to correctly handle
		// replacing the selection etc.
		CEdit::Paste();

		// Trim line
		CString strEntry;
		GetWindowText(strEntry);
		if(strEntry.GetLength()>450)
			SetWindowText(strEntry.Left(450));

		return;
	}

	// Multiple lines, what fun. Handle first line separately to include
	// any existing text properly
	CString sFirst=sText.Left(iCR);
	sText=sText.Mid(iCR+2);

	ReplaceSel(sFirst);

	// Trim line
	CString strEntry;
	GetWindowText(strEntry);
	if(strEntry.GetLength()>450)
		SetWindowText(strEntry.Left(450));

	// Send it
	SendMessage(WM_CHAR,13,0);

	// Find next CR
	iCR=sText.Find(_T("\r\n"));

	// Paste remaining lines
	while(iCR!=-1)
	{
		// Extract line and cut off line/CR
		CString sFirst=sText.Left(iCR);
		sText=sText.Mid(iCR+2);

		// If text is empty, make it a space so it transfers
		if(sFirst.GetLength()==0) sFirst=" ";

		// Enforce max length
		if(sFirst.GetLength()>450) sFirst=sFirst.Left(450);

		// Set text and carry out Return key
		SetWindowText(sFirst);
		SendMessage(WM_CHAR,13,0);

		// Find next CR
		iCR=sText.Find(_T("\r\n"));
	}

	// See if there's anything left on the last line
	if(sText.GetLength()!=0)
	{
		// Set text and carry out Return key
		SetWindowText(sText);
		SendMessage(WM_CHAR,13,0);
	}
}

void CInputWndUnicode::OnTimer(UINT nIDEvent) 
{
	if(nIDEvent==1234)
		NotifyOwner(IW_CREATED,0);
	else
		CEdit::OnTimer(nIDEvent);
}
