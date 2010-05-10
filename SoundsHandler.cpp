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

#include "Identifiers.h"
#include "SoundsHandler.h"

#include "GraphicAlpha.h"
#include "GraphicButton.h"
#include "GraphicHScroll.h"

#include "AudioHandler.h"
#include "DrumsHandler.h"

#include "FolderOptionsDlg.h"
#include "FileSystem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CSoundsHandler::~CSoundsHandler()
{
	delete m_pgBackground;

	delete m_pgaMinus;
	delete m_pgaPlus;
}

// Create all the controls for this section
void CSoundsHandler::Create(CLeafDrumsView* pView,int iX,int iY)
{
	CSectionHandler::Create(pView,iX,iY);

	// Create and load background graphics
	CGraphic background(CLIENTWIDTH,491);
	background.LoadResource("DrumScreenBackground");
	m_pgBackground=new CGraphic(GetWidth(),GetHeight());
	// This line necessary because I didn't bother remaking the background
	// bitmaps when I made the screen 8 pixels taller
	m_pgBackground->FillRect(0,0,GetWidth(),GetHeight(),RGB(255,255,255));
	background.Copy(m_pgBackground,0,0);

	// Load buttons and create graphics with those
	CGraphicAlpha gaButtons(770,458);
	gaButtons.LoadResource("DrumScreenButtons"); // Offset is 8,59
	CGraphicAlpha gaButtonsAlt(770,458);
	gaButtonsAlt.LoadResource("DrumScreenButtonsAlt");
	CGraphicAlpha gaButtonsOver(770,458);
	gaButtonsOver.LoadResource("DrumScreenButtonsOver");
	CGraphicAlpha gaButtonsPressed(770,458);
	gaButtonsPressed.LoadResource("DrumScreenButtonsPressed");
	CGraphicAlpha gaButtonsPressedAlt(770,458);
	gaButtonsPressedAlt.LoadResource("DrumScreenButtonsPressedAlt");

	CGraphic 
		gButtons(GetWidth(),GetHeight()),
		gButtonsAlt(GetWidth(),GetHeight()),
		gButtonsOver(GetWidth(),GetHeight()),
		gButtonsPressed(GetWidth(),GetHeight()),
		gButtonsPressedAlt(GetWidth(),GetHeight());

	m_pgBackground->Copy(&gButtons);
	gaButtons.Copy(&gButtons,8,59-39);
	m_pgBackground->Copy(&gButtonsAlt);
	gaButtonsAlt.Copy(&gButtonsAlt,8,59-39);
	m_pgBackground->Copy(&gButtonsOver);
	gaButtonsOver.Copy(&gButtonsOver,8,59-39);
	m_pgBackground->Copy(&gButtonsPressed);
	gaButtonsPressed.Copy(&gButtonsPressed,8,59-39);
	m_pgBackground->Copy(&gButtonsPressedAlt);
	gaButtonsPressedAlt.Copy(&gButtonsPressedAlt,8,59-39);

	// Load incidental graphics (+ and -)
	m_pgaPlus=new CGraphicAlpha(9,9);
	m_pgaPlus->LoadResource("SoundsPlus");
	m_pgaMinus=new CGraphicAlpha(9,9);
	m_pgaMinus->LoadResource("SoundsMinus");

	// Create buttons
	CreateButton(IDC_SOUNDS_REFRESH,8,441,60,12,
		gButtons,gButtonsOver,gButtonsPressed);
	CreateButton(IDC_SOUNDS_OPEN,25,457,43,12,
		gButtons,gButtonsOver,gButtonsPressed);
	CreateButton(IDC_SOUNDS_OPTIONS,72,441,59,12,
		gButtons,gButtonsOver,gButtonsPressed);
	CreateButton(IDC_SOUNDS_SONG,115,466,50,12,
		gButtons,gButtonsOver,gButtonsPressed);

	// Create scrollbar
	CreateVScroll(IDC_SOUNDS_VSCROLL,144,31,406,
		0,0,1,*m_pgBackground);

	// Initialise
	UpdateScrollbar();
}

// Draw this section onto the specified graphic
void CSoundsHandler::Draw(CGraphicOnscreen& goTarget)
{
	PaintBackground(goTarget,m_pgBackground);

	goTarget.Offset(8,31);
	goTarget.Clip(0,0,134,406+GetHeightOffset());
	int iNumButtons=
		m_fhHierarchy.Paint(goTarget,GetPartialButtonCount(),
			m_pgaPlus,m_pgaMinus,
			0,-GetVScroll(),
			m_iActive,m_bOver,m_bPressed,
			m_sSelectedFolder,m_sSelectedFile);
	goTarget.UnClip();
	goTarget.UnOffset();

	if(iNumButtons!=m_iNumButtons)
	{
		// Remember num buttons (for mouse stuff)
		m_iNumButtons=iNumButtons;

		// Update scrollbar
		UpdateScrollbar();
	}
}

void CSoundsHandler::OnVScroll(int iID,int iValue)
{
	RepaintButtons();
}

void CSoundsHandler::OnButton(int iID)
{
	switch(iID)
	{
	case IDC_SOUNDS_SONG:
		GetView()->ShowMainScreen();
		// Also update drum names
		((CDrumsHandler*)
			(GetView()->GetSection(typeid(CDrumsHandler))))->
			Update();
		break;
	case IDC_SOUNDS_REFRESH:
		m_fhHierarchy.MarkInvalid();
		UpdateScrollbar();
		RepaintButtons();
		break;
	case IDC_SOUNDS_OPTIONS:
		{
			CFolderOptionsDlg dlg;
			dlg.DoModal();
			OnButton(IDC_SOUNDS_REFRESH);
		} break;
	case IDC_SOUNDS_OPEN:
		{
			// Get right folder
			CString sFolder=
				CFileSystem::sm_pCurrent->GetRoot(0);
			if(!(sFolder.Find(':')!=-1 || sFolder.Left(1).FindOneOf("\\/")!=-1))
				sFolder=CFileSystem::GetProgramFolder()+"/"+sFolder;

			// Execute folder
			if((int)(::ShellExecute(
				AfxGetMainWnd()->GetSafeHwnd(),"open",
				sFolder,NULL,NULL,SW_SHOWNORMAL))<=32)
			{
				CString strError;
				strError.Format("There was a problem opening the "
					"folder %s.\n\nPerhaps it does not exist? "
					"You can change the folder leafDrums uses in the "
					"folder options dialog.",sFolder);

				::MessageBox(
					AfxGetMainWnd()->GetSafeHwnd(),
					strError,"Couldn't open folder",MB_OK|MB_ICONEXCLAMATION);
			}
		} break;
	}
}

int CSoundsHandler::GetVScroll()
{
	CGraphicVScroll* pgvs=(CGraphicVScroll*)GetItem(IDC_SOUNDS_VSCROLL);
	return pgvs->GetPos();
}

void CSoundsHandler::UpdateScrollbar()
{
	// Vertical
	CGraphicVScroll* pgvs=(CGraphicVScroll*)GetItem(IDC_SOUNDS_VSCROLL);
	pgvs->SetHeight(406+GetHeightOffset(),200);
	pgvs->SetRange(0,m_fhHierarchy.GetHeight()-GetFullButtonCount(),GetFullButtonCount());
}

void CSoundsHandler::OnLButtonDown(int iX,int iY)
{
	if(m_iActive==-1) return;

	m_bPressed=TRUE;
	RepaintButton(m_iActive);

	m_bJumpBack=IsDoubleClick();
}

void CSoundsHandler::OnLButtonUp(int iX,int iY)
{
	m_bPressed=FALSE;
	RepaintButton(m_iActive);

	if(m_bOver)
	{
		// Get folder/filename
		CString sFolder,sFile;
		m_fhHierarchy.ButtonClicked(0,0,m_iActive+GetVScroll(),
			sFolder,sFile);

		// If it's just a folder click, then just repaint
		if(sFolder=="!FOLDER")
		{
			RepaintButtons();
			UpdateScrollbar();
		}
		else
		{
			// Set folder/filename as new selected sound
			try
			{
				m_pSound->SetFile(sFile,sFolder);
				MarkChange();
				m_sSelectedFolder=sFolder; m_sSelectedFile=sFile;
				RepaintButtons();
				((CAudioHandler*)
					(GetView()->GetSection(typeid(CAudioHandler))))->
					SetSound(m_pSound);

				if(m_bJumpBack)
					OnButton(IDC_SOUNDS_SONG);
			}
			catch(CString strError)
			{
				::MessageBox(GetView()->GetSafeHwnd(),
					strError,"Error loading sound file",MB_ICONEXCLAMATION|MB_OK);
			}
		}
	}

	OnMouseMove(iX,iY);
}

void CSoundsHandler::OnMouseMove(int iX,int iY)
{
	// Get location in terms of button number or -1
	int iOverButton;
	if(iX<8 || iX>=142 || iY<31 || (iY-31)/17 >=m_iNumButtons || (iY-31)%17>=15)
		iOverButton=-1;
	else
		iOverButton=(iY-31)/17;

	// See if we've currently got it pressed
	if(m_bPressed)
	{
		// Set over to true if over current button, false if not
		BOOL bNewOver=(iOverButton == m_iActive);
		if(m_bOver!=bNewOver)
		{
			m_bOver=bNewOver;
			RepaintButton(m_iActive);
		}
	}
	else
	{
		if(iOverButton!=m_iActive)
		{
			RepaintButton(m_iActive);
			m_iActive=iOverButton;
			m_bOver=(m_iActive!=-1);
			RepaintButton(m_iActive);
		}
	}
}

void CSoundsHandler::OnMouseOut()
{
	OnMouseMove(-1,-1);
}

void CSoundsHandler::RepaintButton(int iButton)
{
	Repaint(8,31+iButton*17,136,15);
}

void CSoundsHandler::RepaintButtons()
{
	Repaint(8,31,136,17*24+GetHeightOffset());
}

void CSoundsHandler::SetSound(CSound* pSound)
{ 
	m_pSound=pSound;
	m_sSelectedFile=m_pSound->GetFile();
	m_sSelectedFolder=m_pSound->GetFolder();

	((CAudioHandler*)
		(GetView()->GetSection(typeid(CAudioHandler))))->
		SetSound(m_pSound);

	// Expand file hierarchy to show file
	m_fhHierarchy.ExpandToShow(m_sSelectedFolder,m_sSelectedFile);
}

// Checks whether the sound line data is still in current song
BOOL CSoundsHandler::IsSoundValid()
{
	for(int i=0;i<GetSong()->GetNumSounds();i++)
	{
		if(&GetSong()->GetSound(i)==m_pSound)
			return TRUE;
	}
	return FALSE;
}

// Sets the current sound again
void CSoundsHandler::ResetCurrentSound()
{
	SetSound(m_pSound);
}

void CSoundsHandler::NewSong() 
{ 
	SetSound(&GetSong()->GetSound(0)); 
	m_fhHierarchy.MarkInvalid(); 
}

// Tell window when it's resized
void CSoundsHandler::InformResize(int iPreviousWOffset,int iPreviousHOffset)
{
	UpdateScrollbar();
}
