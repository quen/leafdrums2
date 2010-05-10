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
#ifndef CUTEDRUMSVIEW_H
#define CUTEDRUMSVIEW_H

#include "GraphicAlpha.h"
#include "Graphic.h"
#include "GraphicListbox.h"

#include "LeafDrumsDoc.h"

#include "SectionHandler.h"

class CLeafDrumsView : public CView
{
protected: // create from serialization only
	CLeafDrumsView();
	DECLARE_DYNCREATE(CLeafDrumsView)

// Attributes
public:
	static CLeafDrumsView* sm_pCurrent;
	CLeafDrumsDoc* GetDocument();

// Operations
public:
	void OnHelp();

	void Capture(CSectionHandler* pCapture);
	void Release(CSectionHandler* pCapture);
	BOOL HasCapture(CSectionHandler* pCapture);

	CSectionHandler* GetSection(const type_info &type);

	void ShowMainScreen();
	void ShowSoundScreen();

	BOOL IsDoubleClick() { return m_bIsDoubleClick; }

	void EatFastClick() { m_dwEatFastClick=GetTickCount(); }

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLeafDrumsView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CLeafDrumsView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CLeafDrumsView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy) ;
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnCancelMode();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	afx_msg LONG OnPlayPositionChanged(UINT,LONG);
	afx_msg LONG OnPlayStopped(UINT,LONG);
 	DECLARE_MESSAGE_MAP()

	vec<CSectionHandler> m_vshSections;
	CSectionHandler* m_pshCapture;
	CSectionHandler* m_pshLastMouse;

	CGraphic m_gCopyright;

	void OnSystemKeyChange();

	BOOL m_bIsDoubleClick;
	DWORD m_dwEatFastClick;
	BOOL m_bEatenLeft,m_bEatenRight;

	UINT m_uTimer;
};

#ifndef _DEBUG  // debug version in LeafDrumsView.cpp
inline CLeafDrumsDoc* CLeafDrumsView::GetDocument()
   { return (CLeafDrumsDoc*)m_pDocument; }
#endif



/////////////////////////////////////////////////////////////////////////////
#endif // CUTEDRUMSVIEW_H
