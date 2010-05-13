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

#include "LeafDrumsDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLeafDrumsDoc

IMPLEMENT_DYNCREATE(CLeafDrumsDoc, CDocument)

BEGIN_MESSAGE_MAP(CLeafDrumsDoc, CDocument)
	//{{AFX_MSG_MAP(CLeafDrumsDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLeafDrumsDoc construction/destruction

CLeafDrumsDoc::CLeafDrumsDoc()
{
	m_pSong=new CSong;
	char acTemp[1024];
	GetTempPath(sizeof(acTemp),acTemp);
	m_sAutosaveFile=acTemp;
	m_sAutosaveFile+="autosave.leafDrums2";
}

CLeafDrumsDoc::~CLeafDrumsDoc()
{
	delete m_pSong;
	ClearAutosave(); // Looks like successful exit
}

BOOL sg_bFirstRun=TRUE;

BOOL CLeafDrumsDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	if(sg_bFirstRun)
	{
		// Look for autosave file
		CFileStatus fs;
		if(CFile::GetStatus(m_sAutosaveFile,fs))
		{
			if(::MessageBox(
				NULL,
				"Last time you ran leafDrums it failed to exit correctly, and there "
				"were unsaved changes in your file.\n\n"
				"Would you like to open the autosaved copy of that file? "
				"(If you choose No, the autosaved data will be deleted.)",
				"Autosaved file available",
				MB_ICONINFORMATION | MB_YESNO)==IDYES)
			{
			CFile fAutosave;
			if(fAutosave.Open(m_sAutosaveFile,CFile::modeRead|CFile::shareDenyWrite))
			{
				CArchive openArchive(&fAutosave, CArchive::load);
				Serialize(openArchive);
				openArchive.Close();
				fAutosave.Close();

				SetModifiedFlag();
			}
		}
			else
			{
				ClearAutosave();
			}
		}

		sg_bFirstRun=FALSE;
	}
	else
	{
		delete m_pSong;
		m_pSong=new CSong;
//	UpdateAllViews(NULL);
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CLeafDrumsDoc serialization

void CLeafDrumsDoc::Serialize(CArchive& ar)
{
	m_pSong->Serialize(ar);
//	if(!ar.IsStoring())
//		UpdateAllViews(NULL);
}

/////////////////////////////////////////////////////////////////////////////
// CLeafDrumsDoc diagnostics

#ifdef _DEBUG
void CLeafDrumsDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CLeafDrumsDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CLeafDrumsDoc commands

// Fix the bug in common dialog boxes w. 3-char extension limit
BOOL CLeafDrumsDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	CString strTemp=lpszPathName;
	if(strTemp.Right(4)==".lea") strTemp+="fDrums2";
	
	return CDocument::OnSaveDocument(strTemp);
}

void CLeafDrumsDoc::SetPathName(LPCTSTR lpszPathName, BOOL bAddToMRU) 
{
	CString strTemp=lpszPathName;
	if(strTemp.Right(4)==".lea") strTemp+="fDrums2";
	
	CDocument::SetPathName(strTemp, bAddToMRU);
}

CSong* g_psSong;

UINT APIENTRY CustomSaveHookProc(HWND hdlg,UINT uiMsg,WPARAM wParam,LPARAM lParam)
{
	switch(uiMsg)
	{
	case WM_INITDIALOG:
		g_psSong->SetEmbedSounds(FALSE);
		break;

	case WM_COMMAND:
		if(HIWORD(wParam)==BN_CLICKED && LOWORD(wParam)==IDC_EMBEDSOUNDS)
			g_psSong->SetEmbedSounds(
				(::IsDlgButtonChecked(hdlg,IDC_EMBEDSOUNDS) & BST_CHECKED)==
				BST_CHECKED);
		break;
	}
	return 0;
}

void CLeafDrumsDoc::Save()
{
	if(GetPathName()=="") SaveAs();
	else
	{
		OnSaveDocument(GetPathName());
		ClearAutosave(); // Get rid of autosave if they saved it properly
	}
}

void CLeafDrumsDoc::ClearAutosave()
{
	try
	{
		CFile::Remove(m_sAutosaveFile);
	}
	catch(CFileException* pfe)
	{
		pfe->Delete();
	}
}

void CLeafDrumsDoc::Autosave()
{
	// If user saved it anyway, don't do anything
	if(!IsModified()) return;

	CFile fAutosave;
	if(fAutosave.Open(m_sAutosaveFile,CFile::modeCreate|CFile::modeWrite|CFile::shareDenyWrite))
	{
		CArchive saveArchive(&fAutosave, CArchive::store | CArchive::bNoFlushOnDelete);
		m_pSong->SetEmbedSounds(FALSE);
		Serialize(saveArchive);
		saveArchive.Close();
		fAutosave.Close();
	}
}
 
void CLeafDrumsDoc::SaveAs()
{
	OPENFILENAME ofn;
	ZeroMemory(&ofn,sizeof(ofn));
	
	ofn.lStructSize=sizeof(ofn);
	ofn.hwndOwner=AfxGetMainWnd()->GetSafeHwnd();
	ofn.hInstance=AfxGetResourceHandle();
	ofn.lpstrFilter="leafDrums 2 files (*.leafDrums2)\0*.leafDrums2\0";
	char acFilename[1024];
	lstrcpy(acFilename,GetPathName());
	ofn.lpstrFile=acFilename;
	ofn.nMaxFile=sizeof(acFilename);
	ofn.Flags=OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT|OFN_NOREADONLYRETURN|
		OFN_ENABLETEMPLATE|OFN_EXPLORER|OFN_ENABLEHOOK;
	ofn.lpstrDefExt="leafDrums2";
	ofn.lpTemplateName=MAKEINTRESOURCE(IDD_CUSTOMSAVE);
	ofn.lpfnHook=CustomSaveHookProc;
	g_psSong=GetSong();

	if(::GetSaveFileName(&ofn))
	{
		SetPathName(ofn.lpstrFile);
		OnSaveDocument(ofn.lpstrFile);
		ClearAutosave(); // Get rid of autosave if they saved it properly
	}
}

BOOL CLeafDrumsDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	CString sName=lpszPathName;
	if(sName.Right(11).CompareNoCase(".leafdrums2")!=0)
	{
		::MessageBox(AfxGetMainWnd()->GetSafeHwnd(),
			"This button is for opening leafDrums 2 song files, not .wav samples or other files.\n\n"
			"To select a drum sample, click on the 'Click to set...' text next to a "
			"drum line. This brings up the audio screen, which you can use to select "
			"a drum file.\n\n"
			"See the online help if you need more information.",
			"Open button only opens song files",MB_OK|MB_ICONINFORMATION);
		return FALSE;
	}

	try
	{
		if (!CDocument::OnOpenDocument(lpszPathName))
			return FALSE;
		return TRUE;
	}
	catch(CString strError)
	{
		strError="There was an error opening the file:\n\n"+strError;
		::MessageBox(AfxGetMainWnd()->GetSafeHwnd(),
			strError,"Error opening file",MB_OK|MB_ICONEXCLAMATION);

		SetModifiedFlag(FALSE);
		((CLeafDrumsApp*)AfxGetApp())->OnButtonNew();
	}
}
