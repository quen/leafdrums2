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

#include "Identifiers.h"
#include "PatternHandler.h"

#include "SongHandler.h"

#include "GraphicAlpha.h"
#include "GraphicButton.h"
#include "GraphicHScroll.h"

#include "DrumsHandler.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CPatternHandler::CPatternHandler()
{
	m_iPattern=0;
	m_pbMouseOver=NULL;
	m_pbTriplet1=m_pbTriplet2=m_pbTriplet3=NULL;
	m_pgHelp=NULL;
	m_bTripletMode=FALSE;
}

CPatternHandler::~CPatternHandler()
{
	delete m_pgBackground;
	delete m_pgaHit;
	delete m_pgaAccent;
	delete m_pgaMouseOver;
	delete m_pgHitHelp;
	delete m_pgTripletHelp;
}

// Create all the Patterns for this section
void CPatternHandler::Create(CLeafDrumsView* pView,int iX,int iY)
{
	CSectionHandler::Create(pView,iX,iY);

	// Create and load background graphics
	CGraphic background(CLIENTWIDTH,OTHERHEIGHT);
	background.LoadResource("SongScreenBackground");
	m_pgBackground=new CGraphic(GetWidth(),GetHeight());
	background.Copy(m_pgBackground,-324,-SONGHANDLER_HEIGHT);

	// Load hit graphics
	m_pgaHit=new CGraphicAlpha(11,11);
	m_pgaHit->LoadResource("PatternNormalHit");
	m_pgaAccent=new CGraphicAlpha(11,11);
	m_pgaAccent->LoadResource("PatternAccentHit");
	m_pgaMouseOver=new CGraphicAlpha(11,11);
	m_pgaMouseOver->LoadResource("PatternMouseOver");

	// Load song buttons and create graphics with those
	CGraphicAlpha gaButtons(BMPDETAILS_SONGSCREEN_BUTTONSWIDTH,BMPDETAILS_SONGSCREEN_BUTTONSHEIGHT);
	gaButtons.LoadResource("SongScreenButtons"); // at 8,7
	CGraphicAlpha gaButtonsAlt(BMPDETAILS_SONGSCREEN_BUTTONSWIDTH,BMPDETAILS_SONGSCREEN_BUTTONSHEIGHT);
	gaButtonsAlt.LoadResource("SongScreenButtonsAlt");
	CGraphicAlpha gaButtonsOver(BMPDETAILS_SONGSCREEN_BUTTONSWIDTH,BMPDETAILS_SONGSCREEN_BUTTONSHEIGHT2);
	gaButtonsOver.LoadResource("SongScreenButtonsOver");
	CGraphicAlpha gaButtonsPressed(BMPDETAILS_SONGSCREEN_BUTTONSWIDTH,BMPDETAILS_SONGSCREEN_BUTTONSHEIGHT2);
	gaButtonsPressed.LoadResource("SongScreenButtonsPressed");
	CGraphicAlpha gaButtonsPressedAlt(BMPDETAILS_SONGSCREEN_BUTTONSWIDTH,BMPDETAILS_SONGSCREEN_BUTTONSHEIGHT2);
	gaButtonsPressedAlt.LoadResource("SongScreenButtonsPressedAlt");

	CGraphic 
		gButtons(GetWidth(),GetHeight()),
		gButtonsAlt(GetWidth(),GetHeight()),
		gButtonsOver(GetWidth(),GetHeight()),
		gButtonsPressed(GetWidth(),GetHeight()),
		gButtonsPressedAlt(GetWidth(),GetHeight());

	m_pgBackground->Copy(&gButtons);
	gaButtons.Copy(&gButtons,8-324,7-SONGHANDLER_HEIGHT);
	m_pgBackground->Copy(&gButtonsAlt);
	gaButtonsAlt.Copy(&gButtonsAlt,8-324,7-SONGHANDLER_HEIGHT);
	m_pgBackground->Copy(&gButtonsOver);
	gaButtonsOver.Copy(&gButtonsOver,8-324,7-SONGHANDLER_HEIGHT);
	m_pgBackground->Copy(&gButtonsPressed);
	gaButtonsPressed.Copy(&gButtonsPressed,8-324,7-SONGHANDLER_HEIGHT);
	m_pgBackground->Copy(&gButtonsPressedAlt);
	gaButtonsPressedAlt.Copy(&gButtonsPressedAlt,8-324,7-SONGHANDLER_HEIGHT);

	// Create the help info
	m_pgTripletHelp=new CGraphic(PATTERNHELPWIDTH,PATTERNHELPHEIGHT);
	gButtons.Copy(m_pgTripletHelp,-PATTERNHELPX,-PATTERNHELPY);
	m_pgHitHelp=new CGraphic(PATTERNHELPWIDTH,PATTERNHELPHEIGHT);
	gButtonsAlt.Copy(m_pgHitHelp,-PATTERNHELPX,-PATTERNHELPY);

	// Create checkbox
    CreateCheck(IDC_PATTERN_FINEGRID,67,7,72,12,
		gButtonsAlt,gButtons,gButtonsPressedAlt,gButtonsPressed);

	// Create buttons
	CreateButton(IDC_PATTERN_MAKETRIPLET,387,322,61,12,
		gButtons,gButtonsOver,gButtonsPressed);
	CreateButton(IDC_PATTERN_DELETE,185,7,49,12,
		gButtons,gButtonsOver,gButtonsPressed);
	CreateButton(IDC_PATTERN_DUPLICATE,238,7,69,12,
		gButtons,gButtonsOver,gButtonsPressed);
	CreateButton(IDC_PATTERN_RENAME,311,7,53,12,
		gButtons,gButtonsOver,gButtonsPressed);
	CreateButton(IDC_PATTERN_NEW,146,7,35,12,
		gButtons,gButtonsOver,gButtonsPressed);
	CreateButton(IDC_PATTERN_REMOVEBEAT,345,304,59,12,
		gButtons,gButtonsOver,gButtonsPressed);
	CreateButton(IDC_PATTERN_ADDBEAT,408,304,40,12,
		gButtons,gButtonsOver,gButtonsPressed);

	// Create scrollbars
	CreateHScroll(IDC_PATTERN_HSCROLL,PATTERNX,PATTERNY+PATTERNHEIGHT+2,300,
		0,0,1,*m_pgBackground);
	((CGraphicHScroll*)GetItem(IDC_PATTERN_HSCROLL))->SetStep(24);
	CreateVScroll(IDC_PATTERN_VSCROLL,PATTERNX+PATTERNWIDTH+2,PATTERNY,PATTERNHEIGHT,
		0,0,1,*m_pgBackground);

	// Create pattern spin control
	CGraphicTextOption* pgto=CreateTextOption(
		IDC_PATTERN_SPIN,368,5,80,
		*m_pgBackground);

	// Create the edit box
	CreateEdit(IDC_PATTERN_NAMEEDIT,384,5,48,15);
	ShowControl(IDC_PATTERN_NAMEEDIT,FALSE);

	UpdateDrumsSection();
	UpdatePatternValue();
	SetHelp(NULL);
}

// Draw this section onto the specified graphic
void CPatternHandler::Draw(CGraphicOnscreen& goTarget)
{
	PaintBackground(goTarget,m_pgBackground);

	// Stupid code to test bug
/*	static BOOL bFirst=TRUE;
	if(bFirst)
	{
		CBeat *pb1,*pb2,*pb3;
		CLine& l=Line(PATTERNY);

		CBeat *pbTripletStart=TripletStart(
			l,50,1,pb1,pb2,pb3);
		l.GetBeatContainer().GetParent(pbTripletStart)->MakeTriplet(pbTripletStart);

		pbTripletStart=TripletStart(
			l,300,1,pb1,pb2,pb3);
		l.GetBeatContainer().GetParent(pbTripletStart)->MakeTriplet(pbTripletStart);

		TripletStart(l,171,1,pb1,pb2,pb3);
		bFirst=FALSE;
	}*/

	// Draw help
	if(m_pgHelp)
		m_pgHelp->Copy(&goTarget,PATTERNX,PATTERNHELPY+GetHeightOffset());

	// Get scrollbars
	int iHScroll=GetHScroll(),iVScroll=GetVScrollPos();

	// Get fine-grid
	BOOL bFine=IsFineGrid();

	// Since we're going to refer to pattern lines, make sure it
	// has enough
	CPattern& pattern=GetSong()->GetPattern(m_iPattern);
	pattern.CheckLines(*GetSong());

	// Calculate width information
	// 96*beat+ 6 at start, 1 at end 
	int iNumBeats=pattern.GetTimeSignature();

	// Clip to pattern
	goTarget.Clip(PATTERNX,PATTERNY,PATTERNWIDTH+GetWidthOffset(),PATTERNHEIGHT+GetHeightOffset());

	// Draw each line
	for(int iLine=0;iLine<GetPartialDisplayLines();iLine++)
	{
		// See if we should stop now
		if(iLine+iVScroll>=GetSong()->GetNumSounds()) break;

		// Draw background
		goTarget.FillRect(PATTERNX+6,PATTERNY+iLine*17,96*iNumBeats-iHScroll,15,
			RGB(255,255,255),(60*256)/100);

		// Clip line
		goTarget.Offset(PATTERNX,PATTERNY+iLine*17);
		goTarget.Clip(0,0,96*iNumBeats+7-iHScroll,17);

		// Get line & beat container
		CLine& line=pattern.GetLine(GetModifiedLineNumber(iLine/*+iVScroll*/));
		CBeat& beats=line.GetBeatContainer();

		// Get beat information
		vec<CBeat*> vpb;
		vec<int> viX,viImportance;
		beats.GetScreenData(vpb,viX,viImportance,
			6-iHScroll,96*iNumBeats,0,1);

		// Paint gridlines
		for(int iBeat=0;iBeat<vpb.Size();iBeat++)
		{
			int iAlpha;
			switch(viImportance[iBeat])
			{
				case 1 : iAlpha=256; break;
				case 2 : iAlpha=128; break;
				case 3 : 
					if(!bFine) continue;
					iAlpha=50; 
					break;
			}

			goTarget.FillRect(viX[iBeat],0,1,17,RGB(0,0,0),iAlpha);
		}

		// Paint beats
		for(iBeat=0;iBeat<vpb.Size();iBeat++)
		{
			// Paint beat
			if(vpb[iBeat]->IsAccent() && vpb[iBeat]->IsHit())
				m_pgaAccent->Copy(&goTarget,viX[iBeat]-5,2);
			else if(vpb[iBeat]->IsHit())
				m_pgaHit->Copy(&goTarget,viX[iBeat]-5,2);

			if(m_bTripletMode)
			{
				if( vpb[iBeat]==m_pbTriplet1 || 
					vpb[iBeat]==m_pbTriplet2 || 
					vpb[iBeat]==m_pbTriplet3)
					m_pgaMouseOver->Copy(&goTarget,viX[iBeat]-5,2);
			}
			else
			{
				if(vpb[iBeat]==m_pbMouseOver)
					m_pgaMouseOver->Copy(&goTarget,viX[iBeat]-5,2);
			}
		}

		goTarget.UnClip();
		goTarget.UnOffset();
	}

	goTarget.UnClip();
}

void CPatternHandler::OnHScroll(int iID,int iValue)
{
	if(iID==IDC_PATTERN_HSCROLL)
		RepaintPattern();
}

void CPatternHandler::OnVScroll(int iID,int iValue)
{
	UpdateDrumsSection();
	RepaintPattern();
}

void CPatternHandler::OnCancelMode()
{
	if(m_bTripletMode)
	{
		m_bTripletMode=FALSE;
		Release();
		RefreshMouse();
	}
}

void CPatternHandler::OnButton(int iID)
{
	switch(iID)
	{
	case IDC_PATTERN_FINEGRID:
		RepaintPattern();
		break;
	case IDC_PATTERN_MAKETRIPLET:
		m_bTripletMode=TRUE;
		Capture();
		RefreshMouse();
		break;
	case IDC_PATTERN_RENAME:
		{
			CCustomEdit* pce=(CCustomEdit*)GetItem(IDC_PATTERN_NAMEEDIT);
			pce->SetWindowText(
				GetSong()->GetPattern(m_iPattern).GetName());
			pce->SetWindowPos(&CWnd::wndTop,0,0,0,0,
				SWP_SHOWWINDOW|SWP_NOREPOSITION|SWP_NOMOVE|SWP_NOSIZE);
			pce->SetSel(0,-1);
			ShowControl(IDC_PATTERN_NAMEEDIT,TRUE);
			pce->SetFocus();
		} break;
	case IDC_PATTERN_NAMEEDIT:
		{
			CCustomEdit* pce=(CCustomEdit*)GetItem(IDC_PATTERN_NAMEEDIT);
			ShowControl(IDC_PATTERN_NAMEEDIT,FALSE);
			CString sName;
			pce->GetWindowText(sName);
			GetSong()->GetPattern(m_iPattern).SetName(sName);
			UpdatePatternValue();
			((CSongHandler*)
				(GetView()->GetSection(typeid(CSongHandler))))->
				RepaintSong();
			MarkChange();
		} break;
	case IDC_PATTERN_DELETE:
		{
			// Check there are enough patterns
			if(GetSong()->GetNumPatterns()<=1) break;

			// Get confirmation
			if(::MessageBox(GetView()->GetSafeHwnd(),
				"Are you sure you want to delete this pattern? Once it's deleted, "
				"it is gone for good.",
				"Confirm pattern delete",
				MB_YESNO|MB_ICONQUESTION)==IDYES)
			{
				// Delete pattern
				GetSong()->RemovePattern(GetSong()->GetPattern(m_iPattern));

				// Set new pattern number
				int iPattern=m_iPattern;
				if(iPattern>=GetSong()->GetNumPatterns())
					iPattern=GetSong()->GetNumPatterns()-1;

				// Update display
				m_iPattern=-1;
				SetPatternIndex(iPattern);
				MarkChange();

				// Get song to repaint
				((CSongHandler*)
					(GetView()->GetSection(typeid(CSongHandler))))->
					RepaintSong();
			}
		} break;
	case IDC_PATTERN_NEW:
		{
			// Create new pattern
			CPattern& pNew=GetSong()->AddPattern();

			// Make up number by looking through song for existing numbers
			CString sNumber;
			for(int iNumber=1;;iNumber++)
			{
				sNumber.Format("%i",iNumber);
				BOOL bOK=TRUE;
				for(int iPattern=0;iPattern<GetSong()->GetNumPatterns();iPattern++)
				{
					if(GetSong()->GetPattern(iPattern).GetName()==sNumber)
					{
						bOK=FALSE;
						break;
					}
				}
				if(bOK) break;
			}

			// Set pattern name
			pNew.SetName(sNumber);

			// Refresh with new pattern
			SetPatternIndex(GetSong()->GetNumPatterns()-1);
			MarkChange();
		} break;
	case IDC_PATTERN_DUPLICATE:
		{
			// Create new pattern
			CPattern& pNew=GetSong()->AddPattern();
			pNew=GetSong()->GetPattern(m_iPattern);

			// Look for numbers at start of name
			CString strName=pNew.GetName();
			for(int iFirstText=0;iFirstText<strName.GetLength();iFirstText++)
				if(!isdigit(strName[iFirstText])) break;

			CString 
				strText=strName.Mid(iFirstText),
				strNumber=strName.Left(iFirstText);

			// If no number, add a '2' else add one to the number
			int iNewNumber=2;
			if(strNumber.GetLength()>0)
				iNewNumber=atoi(strNumber)+1;

			// Check this pattern doesn't already exist
			CString sNewName;
			for(;;iNewNumber++)
			{
				sNewName.Format("%i%s",iNewNumber,strText);
				BOOL bOK=TRUE;
				for(int iPattern=0;iPattern<GetSong()->GetNumPatterns();iPattern++)
				{
					if(GetSong()->GetPattern(iPattern).GetName()==sNewName)
					{
						bOK=FALSE;
						break;
					}
				}
				if(bOK) break;
			}

			pNew.SetName(sNewName);

			// Refresh with new pattern
			SetPatternIndex(GetSong()->GetNumPatterns()-1);
			MarkChange();
		}
		break;
	case IDC_PATTERN_ADDBEAT:
		{
			if(m_iPattern==-1) return;

			CPattern& p=GetPattern();
			int iBefore=p.GetTimeSignature();
			if(iBefore<12)
			{
				UpdateTimeSignature(p,iBefore+1);
			}
		}
		break;
	case IDC_PATTERN_REMOVEBEAT:
		{
			if(m_iPattern==-1) return;

			CPattern& p=GetPattern();
			int iBefore=p.GetTimeSignature();
			if(iBefore>1)
			{
				UpdateTimeSignature(p,iBefore-1);				
			}
		}
		break;
	}
}

void CPatternHandler::UpdateTimeSignature(CPattern& p,int iTimeSignature)
{
	p.SetTimeSignature(iTimeSignature);
	MarkChange();
	// Repaint the song as we may now need to update the 
	// 'does it fit' icons
	((CSongHandler*)
		(GetView()->GetSection(typeid(CSongHandler))))->
		RepaintSong();

	// Repaint pattern and fix scrollbars
	UpdateScrollbars();
	RepaintPattern();
}

int CPatternHandler::GetVScrollPos()
{
	CGraphicVScroll* pgvs=(CGraphicVScroll*)GetItem(IDC_PATTERN_VSCROLL);
	return pgvs->GetPos();
}

int CPatternHandler::GetHScroll()
{
	CGraphicHScroll* pghs=(CGraphicHScroll*)GetItem(IDC_PATTERN_HSCROLL);
	return pghs->GetPos();
}

BOOL CPatternHandler::IsFineGrid()
{
	CGraphicCheckbox* pgc=(CGraphicCheckbox*)GetItem(IDC_PATTERN_FINEGRID);
	return pgc->Checked();
}

void CPatternHandler::OnMouseMove(int iX,int iY)
{
	// See if cursor is outside song area
	if(!InPatternArea(iX,iY))
	{
		if(m_bTripletMode)
		{
			::SetCursor(::LoadCursor(AfxGetResourceHandle(),MAKEINTRESOURCE(IDC_POINTERNO)));		
			if(m_pbTriplet1 || m_pbTriplet2 || m_pbTriplet3)
			{
				m_pbTriplet1=m_pbTriplet2=m_pbTriplet3=NULL;
				RepaintPattern();
			}
		}
		else
		{
			::SetCursor(::LoadCursor(NULL,IDC_ARROW));
			if(m_pbMouseOver)
			{
				m_pbMouseOver=NULL;
				RepaintPattern();
			}
		}
		SetHelp(NULL);

		return;
	}

	// See if it's in valid area
	if(!InValidPattern(iX,iY))
	{
		::SetCursor(::LoadCursor(AfxGetResourceHandle(),MAKEINTRESOURCE(IDC_POINTERNO)));		
		SetHelp(NULL);
		if(m_bTripletMode)
		{
			if(m_pbTriplet1 || m_pbTriplet2 || m_pbTriplet3)
			{
				m_pbTriplet1=m_pbTriplet2=m_pbTriplet3=NULL;
				RepaintPattern();
			}
		}
		else
		{
			if(m_pbMouseOver)
			{
				m_pbMouseOver=NULL;
				RepaintPattern();
			}
		}
		return;
	}

	// Get the line it's over...
	CLine& l=Line(iY);

	if(m_bTripletMode)
	{
		// Get current level based on key controls
		int iLevel=
			((GetKeyState(VK_CONTROL)&0x8000)==0x8000) ? 3 :
			(((GetKeyState(VK_SHIFT)&0x8000)==0x8000) ? 1 : 2);

		// Get the triplet start for this mouse location
		CBeat *pbDisplay1,*pbDisplay2,*pbDisplay3;
		TripletStart(l,iX,iLevel,pbDisplay1,pbDisplay2,pbDisplay3);
		if(m_pbTriplet1!=pbDisplay1 || m_pbTriplet2!=pbDisplay2 || m_pbTriplet3!=pbDisplay3)
		{
			m_pbTriplet1=pbDisplay1;
			m_pbTriplet2=pbDisplay2;
			m_pbTriplet3=pbDisplay3;
			RepaintPattern();

			// Set cursor according to triplet presence or otherwise
			if(m_pbTriplet3!=NULL)
				::SetCursor(::LoadCursor(AfxGetResourceHandle(),MAKEINTRESOURCE(IDC_POINTERDEL)));		
			else
				::SetCursor(::LoadCursor(AfxGetResourceHandle(),MAKEINTRESOURCE(IDC_POINTERADD)));		
		}
	}
	else
	{
		// ... now get the beat
		CBeat& b=Beat(l,iX);

		// If it didn't change, don't bother doing anything else
		if(&b==m_pbMouseOver)
			return;

		// Set this as the mouseover beat and repaint
		m_pbMouseOver=&b;
		RepaintPattern();

		// Set help
		SetHelp(m_pgHitHelp);

		// Set cursor according to beat presence or otherwise
		if(b.IsHit())
			::SetCursor(::LoadCursor(AfxGetResourceHandle(),MAKEINTRESOURCE(IDC_POINTERDEL)));		
		else
			::SetCursor(::LoadCursor(AfxGetResourceHandle(),MAKEINTRESOURCE(IDC_POINTERADD)));		
	}
}

void CPatternHandler::OnLButtonDown(int iX,int iY)
{
	if(!InValidPattern(iX,iY)) return;
	CLine& l=Line(iY);

	if(m_bTripletMode)
	{
		// Get current level based on key controls
		int iLevel=
			((GetKeyState(VK_CONTROL)&0x8000)==0x8000) ? 3 :
			(((GetKeyState(VK_SHIFT)&0x8000)==0x8000) ? 1 : 2);

		// Get the triplet start for this mouse location
		CBeat *pbDisplay1,*pbDisplay2,*pbDisplay3;
		CBeat *pbTripletStart=TripletStart(
			l,iX,iLevel,pbDisplay1,pbDisplay2,pbDisplay3);

		// If it's already a triplet, remove
		if(pbTripletStart->IsTriplet())
		{
			l.GetBeatContainer().GetParent(
				pbTripletStart)->RemoveTriplet(pbTripletStart);
			l.InvalidateBeatTimes();
		}
		else // Not already one, add
		{
			l.GetBeatContainer().GetParent(
				pbTripletStart)->MakeTriplet(pbTripletStart);
			l.InvalidateBeatTimes();
		}

		// Turn off triplet mode, repaint
		m_bTripletMode=FALSE;
		Release();
		m_pbMouseOver=NULL;
		OnMouseMove(iX,iY); // Update cursor
		RepaintPattern();
	}
	else
	{
		CBeat& b=Beat(l,iX);

		if(b.IsHit())
		{
			MarkChange();

			// Turn off hit
			b.SetHit(FALSE);
			b.SetAccent(FALSE);
		}
		else
		{
			MarkChange();

			// Add hit
			b.SetHit(TRUE);
			b.SetAccent(FALSE);

			// If this is the last line, add another one
			if(&l == &(GetSong()->GetPattern(m_iPattern).GetLine(GetSong()->GetNumSounds()-1)))
			{
				GetSong()->AddSound();
				UpdateScrollbars();
				UpdateDrumsSection();
			}
		}

		m_pbMouseOver=NULL;
		OnMouseMove(iX,iY); // Update cursor
		RepaintPattern();
	}
}

void CPatternHandler::OnRButtonDown(int iX,int iY)
{
	if(!InValidPattern(iX,iY)) return;
	CLine& l=Line(iY);
	CBeat& b=Beat(l,iX);

	if(b.IsHit())
	{
		MarkChange();

		// Toggle accent
		b.SetAccent(!b.IsAccent());
	}
	else
	{
		MarkChange();

		// Add hit
		b.SetHit(TRUE);
		b.SetAccent(TRUE);

		// If this is the last line, add another one
		if(&l == &(GetSong()->GetPattern(m_iPattern).GetLine(GetSong()->GetNumSounds()-1)))
		{
			GetSong()->AddSound();
			UpdateScrollbars();
			UpdateDrumsSection();
		}
	}

	m_pbMouseOver=NULL;
	OnMouseMove(iX,iY); // Update cursor
	RepaintPattern();
}

void CPatternHandler::OnMouseOut()
{
	if(m_pbMouseOver!=NULL)
	{
		m_pbMouseOver=NULL;
		RepaintPattern();
	}

	// Clear cursor
	::SetCursor(::LoadCursor(NULL,IDC_ARROW));

	// Clear help
	SetHelp(NULL);
}

BOOL CPatternHandler::InPatternArea(int iX,int iY)
{
	return !(iX<PATTERNX || iX>=PATTERNX+PATTERNWIDTH+GetWidthOffset() || iY<PATTERNY || iY>=PATTERNY+PATTERNHEIGHT+GetHeightOffset());
}

BOOL CPatternHandler::InValidPattern(int iX,int iY)
{
	// Check if it's within pattern at all
	if(!InPatternArea(iX,iY)) return FALSE;

	// Ok, within pattern. Check it's within length
	int iNumBeats=GetSong()->GetPattern(m_iPattern).GetTimeSignature();
	if(iX-PATTERNX+GetHScroll() > iNumBeats*96+7) return FALSE;

	// Check it's within valid lines
	if((iY-PATTERNY)/17+GetVScrollPos() >= GetSong()->GetNumSounds())
		return FALSE;

	return TRUE;
}

int CPatternHandler::GetModifiedLineNumber(int iScreenLine)
{
	return ((CDrumsHandler*)
		(GetView()->GetSection(typeid(CDrumsHandler))))->
		GetSoundNumber(iScreenLine);
}

CLine& CPatternHandler::Line(int iY)
{
	GetPattern().CheckLines(*GetSong());
	return GetPattern().GetLine(
		GetModifiedLineNumber((iY-PATTERNY)/17/*+GetVScrollPos()*/));
}

CBeat& CPatternHandler::Beat(CLine& line,int iX)
{
	// Get X relative to pattern
	int iRelativeX=iX+GetHScroll()-PATTERNX;

	// Get beat information
	vec<CBeat*> vpb;
	vec<int> viX,viImportance;
	line.GetBeatContainer().GetScreenData(vpb,viX,viImportance,
		6,96*GetPattern().GetTimeSignature(),0,1);

	// If fine is turned off, remove all the unimportant ones
	if(!IsFineGrid())
	{
		for(int i=vpb.Size()-1;i>=0;i--)
		{
			if(viImportance[i]==3)
			{
				vpb.RemoveAt(i);
				viImportance.RemoveAt(i);
				viX.RemoveAt(i);
			}
		}
	}

	// Find nearest beat
	CBeat* pbNearest;
	int iNearestDistance=-1;

	for(int iBeat=0;iBeat<vpb.Size();iBeat++)
	{
		int iDistance=viX[iBeat]-iRelativeX;
		if(iDistance<0) iDistance=-iDistance;

		if(iNearestDistance==-1 || iDistance<iNearestDistance)
		{
			iNearestDistance=iDistance;
			pbNearest=vpb[iBeat];
		}
	}

	return *pbNearest;
}

CBeat* CPatternHandler::TripletStart(
	CLine& l,int iX,int iLevel,
	CBeat* &pbDisplay1,CBeat* &pbDisplay2,CBeat* &pbDisplay3)
{
	// Get X relative to pattern
	int iRelativeX=iX+GetHScroll()-PATTERNX;

	// Get beat information
	vec<CBeat*> vpb;
	vec<int> viX,viImportance;
	l.GetBeatContainer().GetScreenData(vpb,viX,viImportance,
		6,96*GetPattern().GetTimeSignature(),0,1);

	// Remove everything > the desired level
	for(int i=vpb.Size()-1;i>=0;i--)
	{
		if(viImportance[i]>iLevel)
		{
			vpb.RemoveAt(i);
			viImportance.RemoveAt(i);
			viX.RemoveAt(i);
		}
	}

	// Find nearest beat
	int iNearestIndex;
	int iNearestDistance=-1;

	for(int iBeat=0;iBeat<vpb.Size();iBeat++)
	{
		int iDistance=viX[iBeat]-iRelativeX;
		if(iDistance<0) iDistance=-iDistance;

		if(iNearestDistance==-1 || iDistance<iNearestDistance)
		{
			iNearestDistance=iDistance;
			iNearestIndex=iBeat;
		}
	}

	// Find version of beat at desired level
	CBeat* pbLevel=vpb[iNearestIndex]->GetThisParent(
		&l.GetBeatContainer(),3-iLevel);
	
	// See if it's a triplet
	if(pbLevel->IsTriplet())
	{
		// Go back to first beat in triplet
		int iOriginalIndex=iNearestIndex;
		while(iNearestIndex>0 && 
			viImportance[iNearestIndex-1]<=viImportance[iNearestIndex] &&
			vpb[iNearestIndex-1]->GetThisParent(
				&l.GetBeatContainer(),3-iLevel)->IsTriplet() 
			) iNearestIndex--;

		// If we went back too far then there must be two triplets in this
		// beat and we should've selected the second one
		if(iOriginalIndex-iNearestIndex>=3)
			iNearestIndex+=3;//iOriginalIndex+3;

		// Return all three for display
		pbDisplay1=vpb[iNearestIndex];
		pbDisplay2=vpb[iNearestIndex+1];
		pbDisplay3=vpb[iNearestIndex+2];

		// Return the first beat, at appropriate level
		pbLevel=vpb[iNearestIndex]->GetThisParent(
				&l.GetBeatContainer(),3-iLevel);
		return pbLevel;
	}
	else
	{
		// Check the following beat; this is intended to prevent you 
		// selecting a cross-beat pair, or a pair which includes one that's
		// already a triplet
		if(iNearestIndex==vpb.Size()-1 || 
			viImportance[iNearestIndex+1]<viImportance[iNearestIndex] ||
			vpb[iNearestIndex+1]->GetThisParent(&l.GetBeatContainer(),3-iLevel)->
			  IsTriplet())
			iNearestIndex--;

		// THE ABOVE LINE CAN SET iNearestIndex to -1 which fucks things right up

		// Get beat at correct level
		pbLevel=vpb[iNearestIndex]->GetThisParent(
			&l.GetBeatContainer(),3-iLevel); 

		// If it's now a triplet, we're ufcked and can't create one, return null
		if(pbLevel->IsTriplet()) return NULL;

		// Set display beats
		pbDisplay1=vpb[iNearestIndex];
		pbDisplay2=vpb[iNearestIndex+1];
		pbDisplay3=NULL;

		// Return beat
		return pbLevel;
	}

}

void CPatternHandler::SetHelp(CGraphic* pgHelp)
{
	if(pgHelp==NULL) 
		pgHelp=(m_bTripletMode ? m_pgTripletHelp : m_pgHitHelp);

	if(m_pgHelp==pgHelp) return;
	m_pgHelp=pgHelp;
	Repaint(PATTERNHELPX,PATTERNHELPY+GetHeightOffset(),PATTERNHELPWIDTH,PATTERNHELPHEIGHT);
}

void CPatternHandler::UpdateScrollbars()
{
	// Horizontal scrollbar
	CGraphicHScroll* pghs=(CGraphicHScroll*)GetItem(IDC_PATTERN_HSCROLL);
	int iWidth=300+GetWidthOffset();
	pghs->SetWidth(iWidth,200);
	int iTimeSignature=GetPattern().GetTimeSignature();
	pghs->SetRange(0,iTimeSignature*96+7-iWidth,iWidth);

	// Vertical
	CGraphicVScroll* pgvs=(CGraphicVScroll*)GetItem(IDC_PATTERN_VSCROLL);
	pgvs->SetHeight(PATTERNHEIGHT+GetHeightOffset(),200);
	pgvs->SetRange(0,(GetSong()->GetNumSounds()-GetFullDisplayLines()),GetFullDisplayLines());
}


void CPatternHandler::UpdateDrumsSection()
{
	// Also update drum names
	((CDrumsHandler*)
		(GetView()->GetSection(typeid(CDrumsHandler))))->
		Update();
}


void CPatternHandler::UpdatePatternValue()
{
	CGraphicTextOption* pgto=(CGraphicTextOption*)GetItem(IDC_PATTERN_SPIN);

	// Make sure list is current
	pgto->Clear();
	for(int i=0;i<GetSong()->GetNumPatterns();i++)
		pgto->AddItem(GetSong()->GetPattern(i).GetName());

	// Select current pattern
	pgto->SetSel(m_iPattern);
	pgto->InvalidateRect(NULL,FALSE);
}

void CPatternHandler::SetPatternIndex(int iPattern)
{
	if(m_iPattern==iPattern) return;
	m_iPattern=iPattern;
	CGraphicHScroll* pghs=(CGraphicHScroll*)GetItem(IDC_PATTERN_HSCROLL);
	pghs->SetPos(0);
	UpdatePatternValue();
	RepaintPattern();
}

void CPatternHandler::OnSelchange(int iID,int iValue)
{
	if(iID==IDC_PATTERN_SPIN)
	{
		if(m_iPattern!=iValue)
			SetPatternIndex(iValue);
	}
}

void CPatternHandler::NewSong()
{ 
	UpdateScrollbars();
	GetVScroll(IDC_PATTERN_VSCROLL)->SetPos(0);
	UpdateDrumsSection();
	m_iPattern=-1;
	SetPatternIndex(0); 
}

void CPatternHandler::InformResize(int iPreviousWOffset,int iPreviousHOffset)
{
	UpdateScrollbars();
}
