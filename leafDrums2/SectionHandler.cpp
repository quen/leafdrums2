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

#include "SectionHandler.h"
#include "LeafDrumsView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Create all the controls for this section
void CSectionHandler::Create(CLeafDrumsView* pView,int iX,int iY)
{
	m_pView=pView;
	m_iX=iX;
	m_iY=iY;
}

// Show/hide all the controls for this section
void CSectionHandler::Show()
{
	for(int iControl=0;iControl<m_vpwControls.Size();iControl++)
	{
		if(m_vpwHidden.Find(m_vpwControls[iControl])==-1)
			m_vpwControls[iControl]->ShowWindow(SW_SHOW);
	}
	m_bHidden=FALSE;
	Repaint();
}

void CSectionHandler::Hide()
{
	for(int iControl=0;iControl<m_vpwControls.Size();iControl++)
	{
		m_vpwControls[iControl]->ShowWindow(SW_HIDE);
	}
	m_bHidden=TRUE;
	Repaint();
}

// Returns true if this section includes the specified control
BOOL CSectionHandler::HasControl(CWnd* pwControl)
{
	for(int iControl=0;iControl<m_vpwControls.Size();iControl++)
		if(m_vpwControls[iControl]->GetSafeHwnd()==pwControl->GetSafeHwnd())
			return TRUE;
	return FALSE;
}

// Returns true if given co-ordinate is in this area
BOOL CSectionHandler::HasPoint(int iX,int iY)
{
	return (iX>=m_iX && iY>=m_iY &&
		iX<m_iX+GetWidth()+GetWidthOffset() && iY<m_iY+GetHeight()+GetHeightOffset());
}


// Shortcut to make creating a button easier
CGraphicButton* CSectionHandler::CreateButton(int iID,int iX,int iY,int iWidth,int iHeight,
	CGraphic& gNormal,CGraphic& gOver,CGraphic& gDown)
{
	CGraphicButton* pgb=new CGraphicButton(TRUE);
	pgb->Create(m_pView,iID,"",iX+GetX(),iY+GetY(),iWidth,iHeight,
		"Verdana",11,
		&gNormal,iX,iY,RGB(0,0,0),
		&gOver,iX,iY,RGB(0,0,0),
		&gDown,iX,iY,RGB(0,0,0));
	AddControl(pgb);
	return pgb;
}

// Shortcut to make creating a checkbox easier
CGraphicCheckbox* CSectionHandler::CreateCheck(int iID,int iX,int iY,int iWidth,int iHeight,
	CGraphic& gNormal,CGraphic& gChecked,CGraphic& gDownNormal,CGraphic& gDownChecked)
{
	CGraphicCheckbox* pgc=new CGraphicCheckbox(TRUE);
	pgc->Create(m_pView,iID,"",iX+GetX(),iY+GetY(),iWidth,iHeight,
		"Verdana",11,
		&gNormal,iX,iY,RGB(0,0,0),
		&gDownNormal,iX,iY,RGB(0,0,0),
		&gDownChecked,iX,iY,RGB(0,0,0),
		&gChecked,iX,iY,RGB(0,0,0));
	AddControl(pgc);
	return pgc;
}

CGraphicHScroll* CSectionHandler::CreateHScroll(int iID,int iX,int iY,int iWidth,
	int iMin,int iMax,int iPage,
	CGraphic& gNormal)
{
	CGraphicHScroll* pgs=new CGraphicHScroll(TRUE);
	pgs->SetRange(iMin,iMax,iPage);
	pgs->Create(m_pView,iID,iX+GetX(),iY+GetY(),iWidth,&gNormal,iX,iY,"Scroll");
	AddControl(pgs);
	return pgs;
}

CGraphicVScroll* CSectionHandler::CreateVScroll(int iID,int iX,int iY,int iHeight,
	int iMin,int iMax,int iPage,
	CGraphic& gNormal)
{
	CGraphicVScroll* pgs=new CGraphicVScroll(TRUE);
	pgs->SetRange(iMin,iMax,iPage);
	pgs->Create(m_pView,iID,iX+GetX(),iY+GetY(),iHeight,&gNormal,iX,iY,"Scroll");
	AddControl(pgs);
	return pgs;
}

CGraphicNumberOption* CSectionHandler::CreateNumberOption(int iID,int iX,int iY,int iWidth,
	int iMin,int iMax,CGraphic& gNormal)
{
	CGraphicNumberOption* pgno=new CGraphicNumberOption(iMin,iMax,TRUE);
	pgno->Create(m_pView,iID,iX+GetX(),iY+GetY(),iWidth,15,
		"Verdana",11,RGB(0,0,0),
		&gNormal,iX,iY,"Scroll",-2,TRUE,FALSE);
	AddControl(pgno);
	return pgno;
}

CGraphicTextOption* CSectionHandler::CreateTextOption(int iID,int iX,int iY,int iWidth,
	CGraphic& gNormal)
{
	CGraphicTextOption* pgto=new CGraphicTextOption(TRUE);
	pgto->Create(m_pView,iID,iX+GetX(),iY+GetY(),iWidth,15,
		"Verdana",11,RGB(0,0,0),
		&gNormal,iX,iY,"Scroll",-2,TRUE,FALSE);
	AddControl(pgto);
	return pgto;
}

CCustomEdit* CSectionHandler::CreateEdit(int iID,int iX,int iY,int iWidth,int iHeight)
{
	CCustomEdit* pe=new CCustomEdit();
	pe->Create(iX+GetX(),iY+GetY(),iWidth,iHeight,GetView(),iID);
	AddControl(pe);
	return pe;
}

// Repaints this section or a section of it
void CSectionHandler::Repaint(int iX,int iY,int iWidth,int iHeight)
{
	if(iWidth==-1) iWidth=GetWidth();
	if(iHeight==-1) iHeight=GetHeight();

	CRect r(iX+GetX(),iY+GetY(),iX+GetX()+iWidth,iY+GetY()+iHeight);
	m_pView->InvalidateRect(&r,FALSE);
}

CWnd* CSectionHandler::GetItem(int iID)
{
	return m_pView->GetDlgItem(iID);
}

void CSectionHandler::Capture()
{
	m_pView->Capture(this);
}

void CSectionHandler::Release()
{
	m_pView->Release(this);
}

BOOL CSectionHandler::HasCapture()
{
	return m_pView->HasCapture(this);
}


CSong* CSectionHandler::GetSong()
{ 
	return GetView()->GetDocument()->GetSong(); 
}

BOOL CSectionHandler::IsDoubleClick() 
{ 
	return GetView()->IsDoubleClick(); 
}

void CSectionHandler::ShowControl(int iID,BOOL bShow)
{
	if(!m_bHidden)
		GetItem(iID)->ShowWindow(bShow ? SW_SHOW : SW_HIDE);

	if(bShow)
	{
		int iPos=m_vpwHidden.Find(GetItem(iID));
		if(iPos!=-1)
			m_vpwHidden.RemoveAt(iPos);
	}
	else
	{
		int iPos=m_vpwHidden.Find(GetItem(iID));
		if(iPos==-1)
			m_vpwHidden.Add(GetItem(iID));
	}
}

BOOL CSectionHandler::GetCheck(int iID)
{
	CGraphicCheckbox* pgc=
		(CGraphicCheckbox*)(GetItem(iID));
	return pgc->Checked();
}

void CSectionHandler::SetCheck(int iID,BOOL bChecked)
{
	CGraphicCheckbox* pgc=
		(CGraphicCheckbox*)(GetItem(iID));
	pgc->SetChecked(bChecked);
}

// Call to tell mfc that document has changed
void CSectionHandler::MarkChange()
{
	GetView()->GetDocument()->SetModifiedFlag();
}

// Call to re-send a mousemove for current mouse position
void CSectionHandler::RefreshMouse()
{
	POINT p;
	GetCursorPos(&p);
	m_pView->ScreenToClient(&p);
	OnMouseMove(p.x-GetX(),p.y-GetY());
}

// Move all controls after/including the specific coordinates
void CSectionHandler::MoveControls(int iMoveX,int iMoveY,int iAfterX,int iAfterY)
{
	for(int i=0;i<m_vpwControls.Size();i++)
	{
		CWnd* pw=m_vpwControls[i];
		if(!AutomoveControl(pw)) continue;
		RECT rCurrent;
		pw->GetWindowRect(&rCurrent);
		RECT rParent;
		GetView()->GetWindowRect(&rParent);
		rCurrent.left-=rParent.left;
		rCurrent.right-=rParent.left;
		rCurrent.top-=rParent.top;
		rCurrent.bottom-=rParent.top;

		BOOL bMove=FALSE;
		if(rCurrent.left-GetX()>=iAfterX && iAfterX!=-1)
		{
			rCurrent.left+=iMoveX;
			rCurrent.right+=iMoveX;
			bMove=TRUE;
		}
		if(rCurrent.top-GetY()>=iAfterY && iAfterY!=-1)
		{
			rCurrent.top+=iMoveY;
			rCurrent.bottom+=iMoveY;
			bMove=TRUE;
		}
		if(bMove) pw->MoveWindow(&rCurrent,FALSE);
	}
}

void CSectionHandler::PaintBackground(CGraphicOnscreen& goTarget,CGraphic* pgBackground)
{
	if(GetWidthOffset()==0 && GetHeightOffset()==0)
		pgBackground->Copy(&goTarget,0,0);
	else if(GetHeightOffset()==0)
	{
		pgBackground->Copy(&goTarget,0,0,0,0,GetSplitX(),GetHeight());
		for(int i=0;i<GetWidthOffset();i++)
		{
			pgBackground->Copy(&goTarget,GetSplitX()+i,0,GetSplitX(),0,1,GetHeight());
		}
		pgBackground->Copy(&goTarget,GetSplitX()+GetWidthOffset(),0,GetSplitX(),0,GetWidth()-GetSplitX(),GetHeight());
	}
	else if(GetWidthOffset()==0)
	{
		pgBackground->Copy(&goTarget,0,0,0,0,GetWidth(),GetSplitY());
		for(int i=0;i<GetHeightOffset();i++)
		{
			pgBackground->Copy(&goTarget,0,GetSplitY()+i,0,GetSplitY(),GetWidth(),1);
		}
		pgBackground->Copy(&goTarget,0,GetSplitY()+GetHeightOffset(),0,GetSplitY(),GetWidth(),GetHeight()-GetSplitY());
	}
	else
	{
		// Draw order

		//  1    5a    2
		//
		//
		//  6a   7     6b
		//
		//
		//  3    5b    4


		// 1-4
		pgBackground->Copy(&goTarget,0,0,0,0,GetSplitX(),GetSplitY());
		pgBackground->Copy(&goTarget,GetSplitX()+GetWidthOffset(),0,GetSplitX(),0,GetWidth()-GetSplitX(),GetSplitY());
		pgBackground->Copy(&goTarget,0,GetSplitY()+GetHeightOffset(),0,GetSplitY(),GetSplitX(),GetHeight()-GetSplitY());
		pgBackground->Copy(&goTarget,GetSplitX()+GetWidthOffset(),GetSplitY()+GetHeightOffset(),GetSplitX(),GetSplitY(),GetWidth()-GetSplitX(),GetHeight()-GetSplitY());

		// 5a,b
		for(int iX=0;iX<GetWidthOffset();iX++)
		{
			pgBackground->Copy(&goTarget,GetSplitX()+iX,0,GetSplitX(),0,1,GetSplitY());
			pgBackground->Copy(&goTarget,GetSplitX()+iX,GetSplitY()+GetHeightOffset(),GetSplitX(),GetSplitY(),1,GetHeight()-GetSplitY());
		}
		// 6a,b
		for(int iY=0;iY<GetHeightOffset();iY++)
		{
			pgBackground->Copy(&goTarget,0,GetSplitY()+iY,0,GetSplitY(),GetSplitX(),1);
			pgBackground->Copy(&goTarget,GetSplitX()+GetWidthOffset(),GetSplitY()+iY,GetSplitX(),GetSplitY(),GetWidth()-GetSplitX(),1);
		}

		// 7
		goTarget.FillRect(GetSplitX(),GetSplitY(),GetWidthOffset(),GetHeightOffset(),pgBackground->GetPixel(GetSplitX(),GetSplitY()));
	}
}

void CSectionHandler::ShiftControls(int iMoveX,int iMoveY)
{
	for(int i=0;i<m_vpwControls.Size();i++)
	{
		CWnd* pw=m_vpwControls[i];
		RECT rCurrent;
		pw->GetWindowRect(&rCurrent);
		RECT rParent;
		GetView()->GetWindowRect(&rParent);
		rCurrent.left-=rParent.left;
		rCurrent.right-=rParent.left;
		rCurrent.top-=rParent.top;
		rCurrent.bottom-=rParent.top;

		rCurrent.left+=iMoveX;
		rCurrent.right+=iMoveX;
		rCurrent.top+=iMoveY;
		rCurrent.bottom+=iMoveY;

		pw->MoveWindow(&rCurrent,FALSE);
	}
}

void CSectionHandler::MoveSize(int iX,int iY,int iWidth,int iHeight)
{
	int iXOffset=iX-GetX(),iYOffset=iY-GetY();
	if(iXOffset!=0 || iYOffset!=0)
	{
		ShiftControls(iXOffset,iYOffset);
		m_iX=iX;
		m_iY=iY;
	}

	int iWidthOffset=iWidth-GetWidth(),iHeightOffset=iHeight-GetHeight();
	if(iWidthOffset!=m_iWidthOffset || iHeightOffset!=m_iHeightOffset) 
	{
		MoveControls(
			iWidthOffset-m_iWidthOffset,iHeightOffset-m_iHeightOffset,
			GetSplitX(),GetSplitY());
		int iPreviousWOffset=m_iWidthOffset,iPreviousHOffset=m_iHeightOffset;
		m_iWidthOffset=iWidthOffset;
		m_iHeightOffset=iHeightOffset;
		InformResize(iPreviousWOffset,iPreviousHOffset);
	}
}

