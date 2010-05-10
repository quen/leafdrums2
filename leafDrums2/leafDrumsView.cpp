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
#include "LeafDrums.h"
#include "typeinfo.h"
#include "htmlhelp.h"

#include "LeafDrumsDoc.h"
#include "LeafDrumsView.h"
#include "MainFrm.h"

#include "PaletteDC.h"
#include "GraphicOnscreen.h"
#include "GraphicVScroll.h"
#include "GraphicHScroll.h"
#include "GraphicFont.h"
#include "GraphicListbox.h"
#include "GraphicButton.h"
#include "GraphicTextOption.h"

#include "FrameAnimation.h"

#include "OneShotDialog.h"

#include "Identifiers.h"

#include "ControlHandler.h"
#include "SongHandler.h"
#include "DrumsHandler.h"
#include "PatternHandler.h"
#include "SoundsHandler.h"
#include "AudioHandler.h"

#include "RegisterDialog.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define TIMER_AUTOSAVE 500

CLeafDrumsView* CLeafDrumsView::sm_pCurrent;

/////////////////////////////////////////////////////////////////////////////
// CLeafDrumsView

IMPLEMENT_DYNCREATE(CLeafDrumsView, CView)

BEGIN_MESSAGE_MAP(CLeafDrumsView, CView)
	//{{AFX_MSG_MAP(CLeafDrumsView)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_SETCURSOR()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_CANCELMODE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDBLCLK()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_MESSAGE(WM_PLAYPOSITIONCHANGED,OnPlayPositionChanged)
	ON_MESSAGE(WM_PLAYSTOPPED,OnPlayStopped)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLeafDrumsView construction/destruction

CLeafDrumsView::CLeafDrumsView() : m_gCopyright(CLIENTWIDTH,COPYRIGHTHEIGHT)
{
	sm_pCurrent=this;
	m_pshCapture=NULL;
	m_pshLastMouse=NULL;
	m_bIsDoubleClick=FALSE;
	m_dwEatFastClick=0;
	m_bEatenLeft=m_bEatenRight=FALSE;
}

CLeafDrumsView::~CLeafDrumsView()
{
	sm_pCurrent=NULL;
}

BOOL CLeafDrumsView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	cs.style&= ~WS_BORDER;
//	cs.style|=WS_CLIPCHILDREN;
	// get white patches in redraw, looks ick :)

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CLeafDrumsView drawing

void CLeafDrumsView::OnDraw(CDC* pDC)
{
	CLeafDrumsDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CDisplayDevice::sm_pDisplayDevice->Realize(this);
	CDisplayDevice::sm_pDisplayDevice->InitDC(pDC);

	CRect rClient;
	GetClientRect(&rClient);

	CGraphicOnscreen graphic(rClient.right,rClient.bottom);
	RECT rect;
	pDC->GetClipBox(&rect);
	graphic.Clip(rect.left,rect.top,rect.right-rect.left,rect.bottom-rect.top);

	// Get sections to draw
	for(int iSection=0;iSection<m_vshSections.Size();iSection++)
	{
		if(m_vshSections[iSection].IsHidden()) continue;

		if(graphic.EntirelyClipped(
			m_vshSections[iSection].GetX(),
			m_vshSections[iSection].GetY(),
			m_vshSections[iSection].GetX()+m_vshSections[iSection].GetWidth()+m_vshSections[iSection].GetWidthOffset(),
			m_vshSections[iSection].GetY()+m_vshSections[iSection].GetHeight()+m_vshSections[iSection].GetHeightOffset()))
			continue;

		graphic.Offset(
			m_vshSections[iSection].GetX(),
			m_vshSections[iSection].GetY());
		graphic.Clip(0,0,
			m_vshSections[iSection].GetWidth()+m_vshSections[iSection].GetWidthOffset(),
			m_vshSections[iSection].GetHeight()+m_vshSections[iSection].GetHeightOffset());

		m_vshSections[iSection].Draw(graphic);

		graphic.UnClip();
		graphic.UnOffset();
	}

	// Draw copyright
	m_gCopyright.Copy(&graphic,rClient.Width()-CLIENTWIDTH,rClient.Height()-COPYRIGHTHEIGHT);
	graphic.FillRect(0,rClient.Height()-COPYRIGHTHEIGHT,rClient.Width()-CLIENTWIDTH,COPYRIGHTHEIGHT,RGB(255,255,255));

	graphic.UnClip();	

	graphic.Draw(pDC,0,0);	
}

/////////////////////////////////////////////////////////////////////////////
// CLeafDrumsView diagnostics

#ifdef _DEBUG
void CLeafDrumsView::AssertValid() const
{
	CView::AssertValid();
}

void CLeafDrumsView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CLeafDrumsDoc* CLeafDrumsView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CLeafDrumsDoc)));
	return (CLeafDrumsDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CLeafDrumsView message handlers

int CLeafDrumsView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	CControlHandler* pchControl=new CControlHandler();
	m_vshSections.Add(pchControl);
	pchControl->Create(this,0,0);

	CSongHandler* pchSongs=new CSongHandler();
	m_vshSections.Add(pchSongs);
	pchSongs->Create(this,0,pchControl->GetHeight());

	CDrumsHandler* pdhDrums=new CDrumsHandler();
	m_vshSections.Add(pdhDrums);
	pdhDrums->Create(this,0,pchControl->GetHeight()+pchSongs->GetHeight());

	CPatternHandler* pphPatterns=new CPatternHandler();
	m_vshSections.Add(pphPatterns);
	pphPatterns->Create(this,pdhDrums->GetWidth(),pchControl->GetHeight()+pchSongs->GetHeight());

	CSoundsHandler* pphSounds=new CSoundsHandler();
	m_vshSections.Add(pphSounds);
	pphSounds->Create(this,0,pchControl->GetHeight());

	CAudioHandler* pphAudio=new CAudioHandler();
	m_vshSections.Add(pphAudio);
	pphAudio->Create(this,pphSounds->GetWidth(),pchControl->GetHeight());

	pphAudio->Hide();
	pphSounds->Hide();

	m_gCopyright.LoadResource("Copyright");

	m_uTimer=SetTimer(TIMER_AUTOSAVE,5*60*1000,NULL);

	return 0;
}


BOOL CLeafDrumsView::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
}

void CLeafDrumsView::OnHelp()
{
	// Obtain path of program
	char cBuffer[512];
	VERIFY(GetModuleFileName(NULL,cBuffer,sizeof(cBuffer))!=0);
	CString strFolder=cBuffer;
	int iPos=strFolder.ReverseFind('\\');
	ASSERT(iPos!=-1);
	strFolder=strFolder.Left(iPos+1);
	strFolder.MakeLower();

	// Run HTML help
	HtmlHelp(
		GetSafeHwnd(),
		strFolder+"leafDrums2.chm",
		HH_DISPLAY_TOPIC,
		NULL);

/*
	// Run the HTML file
	if((int)(::ShellExecute(
		GetSafeHwnd(),"open",
		strFolder+"help/index.html",NULL,NULL,SW_SHOWNORMAL))<=32)
	{
		CString strError;
		strError.Format("There was a problem opening the "
			"help index file %s.\n\n"
			"You need a Web browser such as Netscape Navigator or Microsoft Internet Explorer "
			"in order to view the help. Make sure that both your browser and leafDrums are "
			"installed correctly.",
			strFolder+"help/index.html");

		MessageBox(strError,"Couldn't open help",MB_OK|MB_ICONEXCLAMATION);
	}
*/
}


void CLeafDrumsView::OnSize(UINT nType, int cx, int cy) 
{
	if(cx>=CLIENTWIDTH && cy>=CLIENTHEIGHT)
	{
		int iHeight0=m_vshSections[0].GetHeight();
		m_vshSections[0].MoveSize(0,0,cx,iHeight0);
		int iHeight1=m_vshSections[1].GetHeight();
		m_vshSections[1].MoveSize(0,iHeight0,cx,iHeight1);
		int iWidth2=m_vshSections[2].GetWidth();
		int iHeight2=m_vshSections[2].GetHeight();
		m_vshSections[2].MoveSize(0,iHeight0+iHeight1,iWidth2,cy-CLIENTHEIGHT+iHeight2);
		int iWidth3=m_vshSections[3].GetWidth();
		m_vshSections[3].MoveSize(iWidth2,iHeight0+iHeight1,cx-CLIENTWIDTH+iWidth3,cy-CLIENTHEIGHT+iHeight2);

		int iWidth4=m_vshSections[4].GetWidth();
		int iHeight4=m_vshSections[4].GetHeight();
		m_vshSections[4].MoveSize(0,iHeight0,iWidth4,cy-CLIENTHEIGHT+iHeight4);
		int iWidth5=m_vshSections[5].GetWidth();
		m_vshSections[5].MoveSize(iWidth4,iHeight0,cx-CLIENTHEIGHT+iWidth5,cy-CLIENTHEIGHT+iHeight4);

		InvalidateRect(NULL,FALSE);
	}
}

void CLeafDrumsView::OnHScroll(UINT nSBCode, UINT, CScrollBar* pScrollBar) 
{
	CGraphicHScroll* pgs=(CGraphicHScroll*)pScrollBar;
	int iPos=pgs->GetPos();

	for(int iSection=0;iSection<m_vshSections.Size();iSection++)
	{
		if(m_vshSections[iSection].HasControl(pgs))
		{
			m_vshSections[iSection].OnHScroll(pgs->GetDlgCtrlID(),iPos);
			return;
		}
	}
}

void CLeafDrumsView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CGraphicVScroll* pgs=(CGraphicVScroll*)pScrollBar;
	int iPos=pgs->GetPos();

	for(int iSection=0;iSection<m_vshSections.Size();iSection++)
	{
		if(m_vshSections[iSection].HasControl(pgs))
		{
			m_vshSections[iSection].OnVScroll(pgs->GetDlgCtrlID(),iPos);
			return;
		}
	}
}

BOOL CLeafDrumsView::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	CWnd* pwSender=CWnd::FromHandle((HWND)lParam);

	for(int iSection=0;iSection<m_vshSections.Size();iSection++)
	{
		if(m_vshSections[iSection].HasControl(pwSender))
		{
			if(HIWORD(wParam)==BN_CLICKED)
				m_vshSections[iSection].OnButton(pwSender->GetDlgCtrlID());
			if(HIWORD(wParam)==CBN_SELCHANGE)
			{
				CGraphicOption* pgo=(CGraphicOption*)pwSender;
				m_vshSections[iSection].OnSelchange(
					pwSender->GetDlgCtrlID(),pgo->GetSel());
			}
			return TRUE;
		}
	}

	return CView::OnCommand(wParam, lParam);
}

LONG CLeafDrumsView::OnPlayPositionChanged(UINT,LONG)
{
	for(int iSection=0;iSection<m_vshSections.Size();iSection++)
	{
		m_vshSections[iSection].OnPlayPositionChanged();
	}
	return 0;
}

LONG CLeafDrumsView::OnPlayStopped(UINT,LONG)
{
	for(int iSection=0;iSection<m_vshSections.Size();iSection++)
	{
		m_vshSections[iSection].OnPlayStopped();
	}
	return 0;
}

void CLeafDrumsView::OnMouseMove(UINT nFlags, CPoint point) 
{
	if(m_pshCapture)
	{
		m_pshCapture->OnMouseMove(
		  point.x-m_pshCapture->GetX(),point.y-m_pshCapture->GetY());
		return;
	}
	for(int iSection=0;iSection<m_vshSections.Size();iSection++)
	{
		if(m_vshSections[iSection].IsHidden()) continue;
		if(m_vshSections[iSection].HasPoint(point.x,point.y))
		{
			if(m_pshLastMouse!=&m_vshSections[iSection])
			{
				if(m_pshLastMouse!=NULL)
					m_pshLastMouse->OnMouseOut();
				m_pshLastMouse=&m_vshSections[iSection];
			}
			m_vshSections[iSection].OnMouseMove(
				point.x-m_vshSections[iSection].GetX(),point.y-m_vshSections[iSection].GetY());
			return;
		}
	}

}

void CLeafDrumsView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if(m_dwEatFastClick!=0)
	{
		DWORD dwNow=GetTickCount(),dwDifference;
		if(dwNow < m_dwEatFastClick)
			dwDifference=0xffffffff - (m_dwEatFastClick-dwNow);
		else
			dwDifference=dwNow-m_dwEatFastClick;

		m_dwEatFastClick=0;

		if(dwDifference<=GetDoubleClickTime())
		{
			m_bEatenLeft=TRUE;
			return;
		}
	}

	SetFocus();

	if(m_pshCapture)
	{
		m_pshCapture->OnLButtonDown(
		  point.x-m_pshCapture->GetX(),point.y-m_pshCapture->GetY());
		return;
	}
	for(int iSection=0;iSection<m_vshSections.Size();iSection++)
	{
		if(m_vshSections[iSection].IsHidden()) continue;
		if(m_vshSections[iSection].HasPoint(point.x,point.y))
		{
			m_vshSections[iSection].OnLButtonDown(
				point.x-m_vshSections[iSection].GetX(),point.y-m_vshSections[iSection].GetY());
			return;
		}
	}
}

void CLeafDrumsView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if(m_bEatenLeft)
	{
		m_bEatenLeft=FALSE;
		return;
	}
	if(m_pshCapture)
	{
		m_pshCapture->OnLButtonUp(
		  point.x-m_pshCapture->GetX(),point.y-m_pshCapture->GetY());
		return;
	}
	for(int iSection=0;iSection<m_vshSections.Size();iSection++)
	{
		if(m_vshSections[iSection].IsHidden()) continue;
		if(m_vshSections[iSection].HasPoint(point.x,point.y))
		{
			m_vshSections[iSection].OnLButtonUp(
				point.x-m_vshSections[iSection].GetX(),point.y-m_vshSections[iSection].GetY());
			return;
		}
	}
}

void CLeafDrumsView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	if(m_dwEatFastClick!=0)
	{
		DWORD dwNow=GetTickCount(),dwDifference;
		if(dwNow < m_dwEatFastClick)
			dwDifference=0xffffffff - (m_dwEatFastClick-dwNow);
		else
			dwDifference=dwNow-m_dwEatFastClick;

		m_dwEatFastClick=0;

		if(dwDifference<=GetDoubleClickTime())
		{
			m_bEatenRight=TRUE;
			return;
		}
	}

	if(m_pshCapture)
	{
		m_pshCapture->OnRButtonDown(
		  point.x-m_pshCapture->GetX(),point.y-m_pshCapture->GetY());
		return;
	}
	for(int iSection=0;iSection<m_vshSections.Size();iSection++)
	{
		if(m_vshSections[iSection].IsHidden()) continue;
		if(m_vshSections[iSection].HasPoint(point.x,point.y))
		{
			m_vshSections[iSection].OnRButtonDown(
				point.x-m_vshSections[iSection].GetX(),point.y-m_vshSections[iSection].GetY());
			return;
		}
	}
}

void CLeafDrumsView::OnRButtonUp(UINT nFlags, CPoint point) 
{
	if(m_bEatenRight)
	{
		m_bEatenRight=FALSE;
		return;
	}
	if(m_pshCapture)
	{
		m_pshCapture->OnRButtonUp(
		  point.x-m_pshCapture->GetX(),point.y-m_pshCapture->GetY());
		return;
	}
	for(int iSection=0;iSection<m_vshSections.Size();iSection++)
	{
		if(m_vshSections[iSection].IsHidden()) continue;
		if(m_vshSections[iSection].HasPoint(point.x,point.y))
		{
			m_vshSections[iSection].OnRButtonUp(
				point.x-m_vshSections[iSection].GetX(),point.y-m_vshSections[iSection].GetY());
			return;
		}
	}
}

void CLeafDrumsView::Capture(CSectionHandler* pshCapture)
{
	::SetCapture(GetSafeHwnd());
	m_pshCapture=pshCapture;
}

void CLeafDrumsView::Release(CSectionHandler* pshCapture)
{
	ReleaseCapture();
	m_pshCapture=NULL;
}

BOOL CLeafDrumsView::HasCapture(CSectionHandler* pshCapture)
{
	return (m_pshCapture==pshCapture);
}


CSectionHandler* CLeafDrumsView::GetSection(const type_info &type)
{
	for(int iSection=0;iSection<m_vshSections.Size();iSection++)
	{
		if(typeid(m_vshSections[iSection])==type)
			return &m_vshSections[iSection];
	}
	ASSERT(FALSE);
	return &m_vshSections[0];
}

BOOL CLeafDrumsView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	return TRUE;
	// TODO: Add your message handler code here and/or call default
	
//	return CView::OnSetCursor(pWnd, nHitTest, message);
}

void CLeafDrumsView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CView::OnKeyDown(nChar, nRepCnt, nFlags);

	if(nChar==VK_CONTROL || nChar==VK_SHIFT)
		OnSystemKeyChange();
}

void CLeafDrumsView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CView::OnKeyUp(nChar, nRepCnt, nFlags);

	if(nChar==VK_CONTROL || nChar==VK_SHIFT)
		OnSystemKeyChange();
}

void CLeafDrumsView::OnSystemKeyChange()
{
	POINT point;
	GetCursorPos(&point);
	ScreenToClient(&point);

	for(int iSection=0;iSection<m_vshSections.Size();iSection++)
	{
		if(m_vshSections[iSection].HasPoint(point.x,point.y))
		{
			m_vshSections[iSection].OnSystemKeyChange(
				point.x-m_vshSections[iSection].GetX(),point.y-m_vshSections[iSection].GetY());
			return;
		}
	}

}

void CLeafDrumsView::ShowMainScreen()
{
	// Double-click when changing screen is confusing
	EatFastClick();

	(GetSection(typeid(CSoundsHandler)))->Hide();
	(GetSection(typeid(CAudioHandler)))->Hide();

	(GetSection(typeid(CSongHandler)))->Show();
	(GetSection(typeid(CDrumsHandler)))->Show();
	(GetSection(typeid(CPatternHandler)))->Show();

	CRect rect;
	GetClientRect(&rect);
	OnSize(0,rect.right,rect.bottom);
}

void CLeafDrumsView::ShowSoundScreen()
{
	// Double-click when changing screen is confusing
	EatFastClick();

	(GetSection(typeid(CSongHandler)))->Hide();
	(GetSection(typeid(CDrumsHandler)))->Hide();
	(GetSection(typeid(CPatternHandler)))->Hide();

	(GetSection(typeid(CSoundsHandler)))->Show();
	(GetSection(typeid(CAudioHandler)))->Show();

	CRect rect;
	GetClientRect(&rect);
	OnSize(0,rect.right,rect.bottom);
}

void CLeafDrumsView::OnInitialUpdate() 
{
	for(int iSection=0;iSection<m_vshSections.Size();iSection++)
	{
		m_vshSections[iSection].NewSong();
	}
}

void CLeafDrumsView::OnCancelMode() 
{
	if(m_pshCapture)
		m_pshCapture->OnCancelMode();
}

void CLeafDrumsView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	m_bIsDoubleClick=TRUE;
	OnLButtonDown(nFlags,point);
	m_bIsDoubleClick=FALSE;
}

BOOL CLeafDrumsView::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message==WM_KEYDOWN)
	{
		switch(pMsg->wParam)
		{
		case VK_F5:
			// Jump to main screen
			ShowMainScreen();
			return TRUE;

		case VK_F6:
			// Jump to sound screen if it's got data that is valid
			{
				CSoundsHandler* psh=(CSoundsHandler*)
					GetSection(typeid(CSoundsHandler));

				if(psh->IsSoundValid())
				{
					psh->ResetCurrentSound();
					ShowSoundScreen();
				}

			} return TRUE;

		case VK_F7:
			// If on audio screen, play original
			{
				CAudioHandler* pah=(CAudioHandler*)
					GetSection(typeid(CAudioHandler));

				if(!pah->IsHidden())
					pah->OnButton(IDC_AUDIO_TESTORIGINAL);
			} return TRUE;

		case VK_F8:
			// If on audio screen, play modified
			{
				CAudioHandler* pah=(CAudioHandler*)
					GetSection(typeid(CAudioHandler));

				if(!pah->IsHidden())
					pah->OnButton(IDC_AUDIO_TESTOUTPUT);
			} return TRUE;

		case VK_F9:
			// Play
			(GetSection(typeid(CControlHandler)))->OnButton(
				(GetDocument()->GetSong()->IsPlaying()) ? 
				IDC_CONTROL_STOP : IDC_CONTROL_PLAY);
			return TRUE;

		case 's':
			if(GetKeyState(VK_CONTROL)&0x8000)
			{
				GetDocument()->Save();
				return TRUE;
			}
			break;

		case 'r':
			if( (GetKeyState(VK_CONTROL)&0x8000) || (GetKeyState(VK_MENU)&0x8000))
			{
				CRegisterDialog dlg(this);
				dlg.DoModal();
				return TRUE;
			}
			break;

		case VK_ESCAPE:
			// Cancel mode etc. if appropriate (this turns off triplet mode and such)
			OnCancelMode();
			break;

		case ' ':
			// Don't treat space if it's inside an edit box
			if(dynamic_cast<CCustomEdit*>(GetFocus()))
				break;

			// Play or Stop
			if(GetDocument()->GetSong()->IsPlaying())
			{
				PostMessage(WM_COMMAND,
					BN_CLICKED<<16 | IDC_CONTROL_STOP,
					(LPARAM)::GetDlgItem(GetSafeHwnd(),IDC_CONTROL_STOP));
			}
			else
			{
				PostMessage(WM_COMMAND,
					BN_CLICKED<<16 | IDC_CONTROL_PLAY,
					(LPARAM)::GetDlgItem(GetSafeHwnd(),IDC_CONTROL_PLAY));
			}


		default:
			break;
		}
	}

	return CView::PreTranslateMessage(pMsg);
}

void CLeafDrumsView::OnRButtonDblClk(UINT nFlags, CPoint point) 
{
	m_bIsDoubleClick=TRUE;
	OnRButtonDown(nFlags,point);
	m_bIsDoubleClick=FALSE;
}

void CLeafDrumsView::OnDestroy() 
{
	if(GetDocument()->GetSong()->IsPlaying())
		GetDocument()->GetSong()->Stop();

	CView::OnDestroy();	

	KillTimer(m_uTimer);
}

void CLeafDrumsView::OnTimer(UINT nIDEvent) 
{
	if(nIDEvent==TIMER_AUTOSAVE)
	{
		GetDocument()->Autosave();
	}
	else	
		CView::OnTimer(nIDEvent);
}
