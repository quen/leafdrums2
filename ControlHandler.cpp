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
#include "ControlHandler.h"

#include "GraphicAlpha.h"

#include "WriteDlg.h"

#include "SongHandler.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CControlHandler::~CControlHandler()
{
	delete m_pgBackground;
	delete m_pgLevelIndicator;
}

// Create all the controls for this section
void CControlHandler::Create(CLeafDrumsView* pView,int iX,int iY)
{
	CSectionHandler::Create(pView,iX,iY);

	// Create and load background graphics
	m_pgBackground=new CGraphic(GetWidth(),GetHeight());
	m_pgBackground->LoadResource("ControlBarBackground");

	// Load control buttons and create graphics with those
	CGraphicAlpha gNormal(764,23);
	gNormal.LoadResource("ControlBarButtons");
	CGraphicAlpha gOver(764,23);
	gOver.LoadResource("ControlBarButtonsOver");
	CGraphicAlpha gDown(764,23);
	gDown.LoadResource("ControlBarButtonsPressed");
	CGraphic 
		gControlsOver(GetWidth(),GetHeight()),
		gControls(GetWidth(),GetHeight()),
		gControlsDown(GetWidth(),GetHeight());

	m_pgBackground->Copy(&gControlsOver);
	gOver.Copy(&gControlsOver,8,8);
	m_pgBackground->Copy(&gControls);
	gNormal.Copy(&gControls,8,8);
	m_pgBackground->Copy(&gControlsDown);
	gDown.Copy(&gControlsDown,8,8);

	// Create buttons
	CreateButton(IDC_CONTROL_PLAY,8,8,64,23,
		gControls,gControlsOver,gControlsDown);
	CreateButton(IDC_CONTROL_NEW,  368+68*0,8,64,23,
		gControls,gControlsOver,gControlsDown);
	CreateButton(IDC_CONTROL_OPEN, 368+68*1,8,64,23,
		gControls,gControlsOver,gControlsDown);
	CreateButton(IDC_CONTROL_SAVE, 368+68*2,8,64,23,
		gControls,gControlsOver,gControlsDown);
	CreateButton(IDC_CONTROL_WRITE,368+68*3,8,64,23,
		gControls,gControlsOver,gControlsDown);
	CreateButton(IDC_CONTROL_HELP, 368+68*4,8,64,23,
		gControls,gControlsOver,gControlsDown);
	CreateButton(IDC_CONTROL_QUIT, 368+68*5,8,64,23,
		gControls,gControlsOver,gControlsDown);

	// Create scrollbar
	CreateHScroll(IDC_CONTROL_LEVELSCROLL,84,16,112,
		0,256,64,*m_pgBackground);

	// Prepare level indicator
	m_pgLevelIndicator=new CGraphic(14*8,7);
	gControls.Copy(m_pgLevelIndicator,0,0,210,20,14*8,7);

	// Handle stop button as special case
	m_pgBackground->Copy(&gControls);
	CGraphicAlpha gaStop(64,23);
	gaStop.LoadResource("StopButton");
	gaStop.Copy(&gControls,8,8);

	m_pgBackground->Copy(&gControlsOver);
	CGraphicAlpha gaStopOver(64,23);
	gaStopOver.LoadResource("StopButtonOver");
	gaStopOver.Copy(&gControlsOver,8,8);

	m_pgBackground->Copy(&gControlsDown);
	CGraphicAlpha gaStopPressed(64,23);
	gaStopPressed.LoadResource("StopButtonPressed");
	gaStopPressed.Copy(&gControlsDown,8,8);

	CreateButton(IDC_CONTROL_STOP,8,8,64,23,
		gControls,gControlsOver,gControlsDown);
	ShowControl(IDC_CONTROL_STOP,FALSE);

	UpdateLevel();
}

int sg_iTempLevel=20000;

// Draw this section onto the specified graphic
void CControlHandler::Draw(CGraphicOnscreen& goTarget)
{
	PaintBackground(goTarget,m_pgBackground);

	// Obtain level somehow
	int iLevel;
	BOOL bShowClip=FALSE;
	CSong* pSong=GetView()->GetDocument()->GetSong();
	if(pSong->IsPlaying())
	{
		iLevel=pSong->GetCurrentMaxLevel();
		if(iLevel>=32767) m_iClipCount=8;
		if(m_iClipCount>0)
		{
			bShowClip=TRUE;
			m_iClipCount--;
		}
	}
	else
	{
		iLevel=0;
	}

	int iLevelScaled=(iLevel*112)/32768;

	if(iLevelScaled>0)
		m_pgLevelIndicator->Copy(&goTarget,GetX()+210,GetY()+20,
			0,0,iLevelScaled,7);
	if(bShowClip)
		m_pgLevelIndicator->Copy(&goTarget,GetX()+210+7*14,GetY()+20,
			7*14,0,14,7);
}

void CControlHandler::OnHScroll(int iID,int iValue)
{
	GetView()->GetDocument()->GetSong()->SetLevel(iValue);
	MarkChange();
}

void CControlHandler::OnButton(int iID)
{
	switch(iID)
	{
	case IDC_CONTROL_QUIT : 
		{
			if(GetSong()->IsPlaying())
				OnButton(IDC_CONTROL_STOP);

			AfxGetMainWnd()->PostMessage(WM_CLOSE,0,0); 
		}
		break;
	case IDC_CONTROL_NEW:
		{
			if(GetSong()->IsPlaying())
				OnButton(IDC_CONTROL_STOP);

			// Create new document (default behaviour)
			((CLeafDrumsApp*)AfxGetApp())->OnButtonNew();
		} break;
	case IDC_CONTROL_OPEN:
		{
			if(GetSong()->IsPlaying())
				OnButton(IDC_CONTROL_STOP);

			// Create new document (default behaviour)
			((CLeafDrumsApp*)AfxGetApp())->OnButtonOpen();
		} break;
	case IDC_CONTROL_SAVE:
		{
			// Default file save handling
			try
			{
				GetView()->GetDocument()->SaveAs();
			}
			catch(CString strError)
			{
				strError="There was an error opening the file:\n\n"+strError;
				::MessageBox(AfxGetMainWnd()->GetSafeHwnd(),
					strError,"Error opening file",MB_OK|MB_ICONEXCLAMATION);
			}
		} break;
	case IDC_CONTROL_WRITE:
		{
			// Get confirmation and count
			CWriteDlg wd(*(GetView()->GetDocument()->GetSong()),AfxGetMainWnd());
			if(wd.DoModal()!=IDOK) return;

			// Work out default filename
			CString strDefaultName=GetView()->GetDocument()->GetPathName();
			if(strDefaultName.Right(11)==".leafDrums2")
			{
				strDefaultName=strDefaultName.Left(strDefaultName.GetLength()-11)+
					(wd.m_iFormat==WRITEFORMAT_WAV ? ".wav" : ".ogg");
			}

			// Request a filename
			CFileDialog fd(FALSE,"wav",strDefaultName,
				OFN_HIDEREADONLY |
				((wd.m_iOutputStyle == CSong_WRITEWHOLE) ? OFN_OVERWRITEPROMPT : 0) |
				OFN_NOREADONLYRETURN,
				wd.m_iFormat == WRITEFORMAT_WAV 
				? "Uncompressed audio (*.wav)|*.wav||"
				: "Ogg Vorbis audio (*.ogg)|*.ogg||"
				);
			if(wd.m_iOutputStyle != CSong_WRITEWHOLE)
				fd.m_ofn.lpstrTitle="Save As (base filename)";

			// Write the file
			if(fd.DoModal()==IDOK)
			{
				CWaitCursor wc;
				GetView()->GetDocument()->GetSong()->
					Write(fd.GetPathName(),wd.m_iCount,wd.m_iOutputStyle,wd.m_iFormat);
			}
		} break;
	case IDC_CONTROL_HELP:
		{
			GetView()->OnHelp();
		} break;
	case IDC_CONTROL_PLAY:
		{
			m_iClipCount=0;
			CSong* pSong=GetView()->GetDocument()->GetSong();
			if(pSong->GetBarCount()==0) break;

			ShowControl(IDC_CONTROL_PLAY,FALSE);
			ShowControl(IDC_CONTROL_STOP,TRUE);

			pSong->Start();
		} break;
	case IDC_CONTROL_STOP:
		{
			CSong* pSong=GetView()->GetDocument()->GetSong();
			pSong->Stop();
		} break;
	}
}

// Repaint the levels indicator when there's a play-position change
void CControlHandler::OnPlayPositionChanged()
{
	Repaint(210,20,14*8,7);
}

void CControlHandler::OnPlayStopped()
{
	Repaint(210,20,14*8,7);
	ShowControl(IDC_CONTROL_STOP,FALSE);
	ShowControl(IDC_CONTROL_PLAY,TRUE);
}


void CControlHandler::UpdateLevel()
{
	GetHScroll(IDC_CONTROL_LEVELSCROLL)->SetPos(GetSong()->GetLevel());
}

// Section should reset its controls etc.
void CControlHandler::NewSong()
{
	UpdateLevel();
}
