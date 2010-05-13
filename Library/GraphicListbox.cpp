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
#include "GraphicListbox.h"

#include "PaletteDC.h"
#include "GraphicOnscreen.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGraphicListbox

CGraphicListbox::CGraphicListbox(BOOL fAutoDelete)
{
	m_iCount=0; m_iPos=0; m_fCaptured=m_fOver=m_fDown=FALSE;
	m_iSelected=-1;
	m_pgBackground=m_pgBackgroundOver=m_pgBackgroundDown=NULL;
	m_pgsScrollbar=NULL;

	m_pfont=NULL;
	m_fSorted=FALSE;

	m_fAutoDelete=fAutoDelete;
}

CGraphicListbox::~CGraphicListbox()
{
	if(m_pfont) delete m_pfont;
	if(m_pgBackground) delete m_pgBackground;
	if(m_pgBackgroundDown) delete m_pgBackgroundDown;
	if(m_pgBackgroundOver) delete m_pgBackgroundOver;
}


BEGIN_MESSAGE_MAP(CGraphicListbox, CWnd)
	//{{AFX_MSG_MAP(CGraphicListbox)
	ON_WM_ERASEBKGND()
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CGraphicListbox message handlers

BOOL CGraphicListbox::Create(CWnd* pParentWnd,UINT nID,
		int iX,int iY,int iW,int iH,
		const char* strFont,int iPointSize,int iLineHeight,
		CGraphic* pgBackground, int iXBG,int iYBG,COLORREF colour,
		CGraphic* pgBackgroundo, int iXBGo,int iYBGo,COLORREF colouro,
		CGraphic* pgBackgroundd, int iXBGd,int iYBGd,COLORREF colourd,
		CGraphicScrollbar* pgsScrollbar,
		BOOL fSorted)
{
	// Create font
	m_pfont=new CGraphicFont(strFont,iPointSize);

	// Obtain the background graphics
	m_pgBackground=new CGraphic(iW,iH);
	pgBackground->Copy(m_pgBackground,0,0,iXBG,iYBG,iW,iH);
	m_pgBackgroundOver=new CGraphic(iW,iH);
	pgBackgroundo->Copy(m_pgBackgroundOver,0,0,iXBGo,iYBGo,iW,iH);
	m_pgBackgroundDown=new CGraphic(iW,iH);
	pgBackgroundd->Copy(m_pgBackgroundDown,0,0,iXBGd,iYBGd,iW,iH);
	
	// Set the colours etc.
	m_clrNormal=colour;
	m_clrOver=colouro;
	m_clrDown=colourd;
	m_fSorted=fSorted;

	// Obtain the size in #lines and line-height
	if(iLineHeight==0) m_iLineHeight=m_pfont->Height();
	else m_iLineHeight=iLineHeight;

	// Offset to keep text vaguely centred
	m_iLineOffset=(m_iLineHeight-m_pfont->Height())/2;

	m_iSizeLines=iH/m_iLineHeight;
	m_iViewLines=m_iSizeLines;
	if(iH%m_iLineHeight !=0) m_iSizeLines++;

	if(pgsScrollbar) SetScrollbar(pgsScrollbar);

	// Make up the creation rectangle
	CRect rect(iX,iY,iX+iW,iY+iH);

	return CWnd::Create(NULL, "CGraphicListbox", 
		WS_CHILD|WS_VISIBLE, rect, 
		pParentWnd, nID);
}

BOOL CGraphicListbox::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
}

void CGraphicListbox::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if(m_iSelected==-1) return;

	switch(nChar)
	{
	case VK_UP:
		SetSel(m_iSelected-1,TRUE);
		break;
	case VK_DOWN:
		SetSel(m_iSelected+1,TRUE);
		break;
	}
}

void CGraphicListbox::OnLButtonDown(UINT nFlags, CPoint point) 
{
	SetFocus();

	int iPos=point.y/m_iLineHeight + m_iPos;
	if(point.x<0 || point.y<0 || 
		point.x>=m_pgBackground->Width() ||
		point.y>=m_pgBackground->Height() ||
		iPos>=m_iCount) iPos=-1;

	if(iPos==-1 || iPos==m_iSelected) return;

	// Release capture
	if(m_fCaptured)
	{
		ReleaseCapture();
		m_fCaptured=FALSE;
	}

	// Alter selection
	m_fOver=FALSE;
	SetSel(iPos,TRUE);
	InvalidateRect(NULL,FALSE);
}

void CGraphicListbox::OnMouseMove(UINT nFlags, CPoint point) 
{
	int iPos=point.y/m_iLineHeight + m_iPos;
	if(point.x<0 || point.y<0 || 
		point.x>=m_pgBackground->Width() ||
		point.y>=m_pgBackground->Height() ||
		iPos>=m_iCount) iPos=-1;

	if(m_fOver && iPos!=m_iHighlight)
	{
		// Repaint not-over
		CPaletteClientDC dc(this);
		CGraphicOnscreen gr(m_pgBackground->Width(),m_iLineHeight);

		// Draw background and text
		m_pgBackground->Copy(&gr,0,0,
			0,(m_iHighlight-m_iPos)*m_iLineHeight,
			m_pgBackground->Width(),m_iLineHeight);
		m_pfont->Draw(&gr,2,m_iLineOffset,m_strText[m_iHighlight],
				m_clrNormal);

		// Put graphic to output
		gr.Draw(&dc,0,(m_iHighlight-m_iPos)*m_iLineHeight);

		m_fOver=FALSE;
	}

	if(!m_fOver && iPos!=-1 && 
		(m_iSelected==-1 || iPos!=m_iSelected))
	{
		m_iHighlight=iPos;
		
		// Repaint over
		CPaletteClientDC dc(this);
		CGraphicOnscreen gr(m_pgBackground->Width(),m_iLineHeight);

		// Draw background and text
		m_pgBackgroundOver->Copy(&gr,0,0,
			0,(m_iHighlight-m_iPos)*m_iLineHeight,
			m_pgBackground->Width(),m_iLineHeight);
		m_pfont->Draw(&gr,2,m_iLineOffset,m_strText[m_iHighlight],
				m_clrOver);

		// Put graphic to output
		gr.Draw(&dc,0,(m_iHighlight-m_iPos)*m_iLineHeight);
		m_fOver=TRUE;
	}

	if(m_fOver && !m_fCaptured)
	{
		SetCapture();
		m_fCaptured=TRUE;
	}
	else if(!m_fOver && m_fCaptured)
	{
		ReleaseCapture();
		m_fCaptured=FALSE;
	}
	
	CWnd::OnMouseMove(nFlags, point);
}

void CGraphicListbox::OnPaint() 
{
	CPaletteDC dc(this); // device context for painting

	// Create graphic to calculate display in, init with background
	CGraphicOnscreen gr(m_pgBackground->Width(),m_pgBackground->Height());
	m_pgBackground->Copy(&gr);

	for(int i=0;i<m_iSizeLines;i++)
	{
		// If this is the selected one, draw selected background
		if(m_iPos+i==m_iSelected)
			m_pgBackgroundDown->Copy(&gr,0,i*m_iLineHeight,
				0,m_iLineHeight,m_pgBackground->Width(),m_iLineHeight);

		// Draw text
		if(m_iPos+i < m_iCount)
			m_pfont->Draw(&gr,2,i*m_iLineHeight+m_iLineOffset,
			m_strText[m_iPos+i],
				(m_iPos+i == m_iSelected) ? m_clrDown : m_clrNormal);
	}

	// Draw graphic to screen
	gr.Draw(&dc,0,0);
}

// Add item, sorted if appropriate
void CGraphicListbox::AddItem(const char* strText)
{
	int i;
	if(m_fSorted)
	{
		// Find first position greater than this text
		for(i=0;i<m_iCount;i++)
			if(m_strText[i].CompareNoCase(strText)==1) break;

		// Shuffle along other texts
		for(int iShuffle=m_iCount+1;iShuffle>i;iShuffle--)
			m_strText[iShuffle]=m_strText[iShuffle-1];
		if(m_iSelected>=i) m_iSelected++;
	}
	else i=m_iCount;

	// Insert this text
	m_strText[i]=strText;
	m_iCount++;

	// Repaint
	if(m_pgBackground)
		InvalidateRect(NULL,FALSE);

	// Inform scrollbar
	if(m_pgsScrollbar)
		m_pgsScrollbar->SetRange(0,m_iCount-m_iViewLines,m_iViewLines);
}

// Del item
BOOL CGraphicListbox::DelItem(const char* strText)
{
	// Find text
	for(int i=0;i<m_iCount;i++)
		if(m_strText[i].CompareNoCase(strText)==0) break;
	if(i==m_iCount) return FALSE;

	// Deselect if need be
	if(i==m_iSelected) SetSel(-1);
	else if(m_iSelected>i) m_iSelected--;

	// Delete text
	for(;i<m_iCount-1;i++)
		m_strText[i]=m_strText[i+1];
	m_iCount--;

	// Repaint
	if(m_pgBackground)
		InvalidateRect(NULL,FALSE);

	// Inform scrollbar
	if(m_pgsScrollbar)
		m_pgsScrollbar->SetRange(0,m_iCount-m_iViewLines,m_iViewLines);

	return TRUE;
}

void CGraphicListbox::SetScrollbar(CGraphicScrollbar* pgsScrollbar)
{
	m_pgsScrollbar=pgsScrollbar;

	if(m_pgsScrollbar)
	{
		m_pgsScrollbar->SetListbox(this);
		m_pgsScrollbar->SetRange(0,m_iCount-m_iViewLines,m_iViewLines);
		m_pgsScrollbar->SetPos(m_iPos);
	}
}

void CGraphicListbox::SetPos(int iPos)
{
	// Set position
	if(iPos==m_iPos) return;
	m_iPos=iPos;

	// Update scrollbar, too
	if(m_pgsScrollbar)
		m_pgsScrollbar->SetPos(m_iPos);

	// Repaint
	InvalidateRect(NULL,FALSE);

	// Inform parent
	GetParent()->PostMessage(WM_COMMAND,(LBN_SELCANCEL<<16) |
		GetDlgCtrlID(),(LPARAM)GetSafeHwnd());
}

BOOL CGraphicListbox::GetSel(CString &strSel)
{
	if(m_iSelected==-1) return FALSE;

	strSel=m_strText[m_iSelected];
	return TRUE;
}

void CGraphicListbox::SetSel(int iPos,BOOL fNotify)
{
	if(iPos<-1 || iPos>=m_iCount) return;
	if(m_iSelected==iPos) return;

	BOOL m_fChanged=FALSE;

	m_iSelected=iPos;

	if(m_iSelected!=-1)
	{
		iPos=m_iPos;
		if(m_iSelected-iPos>=m_iViewLines)
		{
			iPos=m_iSelected-m_iViewLines+1;
			m_fChanged=TRUE;
		}

		if(iPos>m_iSelected) 
		{
			iPos=m_iSelected;
			m_fChanged=TRUE;
		}

		if(m_fChanged)
			SetPos(iPos);
	}

	if(fNotify)
		GetParent()->PostMessage(WM_COMMAND,
			(LBN_SELCHANGE << 16) | GetDlgCtrlID(),
			(LPARAM)GetSafeHwnd());

	if(m_pgBackground)
		InvalidateRect(NULL,FALSE);
}

int CGraphicListbox::GetPos()
{
	return m_iPos;
}

int CGraphicListbox::GetSel()
{
	return m_iSelected;
}

void CGraphicListbox::Clear()
{
	m_iCount=0;
	SetSel(-1);

	InvalidateRect(NULL,FALSE);
}

int CGraphicListbox::SetSel(const CString strSel)
{
	for(int i=0;i<m_iCount;i++)
		if(m_strText[i]==strSel) break;

	if(i==m_iCount) return -1;

	SetSel(i);
	return i;
}

void CGraphicListbox::PostNcDestroy() 
{
	CWnd::PostNcDestroy();

	if(m_fAutoDelete) delete this;
}

void CGraphicListbox::SetItem(int iPos,const char* strText)
{
	// Check pos okay
	ASSERT(iPos>=0 && iPos<m_iCount);

	// Set text and repaint
	m_strText[iPos]=strText;
	InvalidateRect(NULL,FALSE);
}
