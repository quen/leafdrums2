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
#include "FileHierarchy.h"

#include "FileSystem.h"
#include "GraphicButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Adds a file to this or to subfolders (creating as necessary)
void CFileHierarchy::AddFile(CString sFolder,CString sFile,BOOL bFromMemory)
{
	if(sFolder=="/" || sFolder=="\\" || sFolder =="")
	{
		m_vsFilename.Add(sFile);
		m_vbFromMemory.Add(bFromMemory);
	}
	else
	{
		// Find the next folder on list
		int iSeparator;
		while(TRUE)
		{
			iSeparator=sFolder.FindOneOf("\\/");
			ASSERT(iSeparator!=-1);
			if(iSeparator!=0) break;
			sFolder=sFolder.Mid(1);
		}
		CString sNextFolder=sFolder.Left(iSeparator);
		CString sRemainingFolders=sFolder.Mid(iSeparator+1);

		// See if we have a child dealing with that folder
		BOOL bHandled=FALSE;
		for(int i=0;i<m_vfhSubFolders.Size();i++)
		{
			if(m_vfhSubFolders[i].m_sName.CompareNoCase(sNextFolder)==0)
			{
				m_vfhSubFolders[i].AddFile(sRemainingFolders,sFile,bFromMemory);
				bHandled=TRUE;
			}
		}

		// If not, create new folder
		if(!bHandled)
		{
			CFileHierarchy* pfh=new CFileHierarchy(sNextFolder);
			pfh->AddFile(sRemainingFolders,sFile,bFromMemory);
			m_vfhSubFolders.Add(pfh);
		}
	}
}

// Adds a list of files
void CFileHierarchy::AddFiles(vec<CString> &vsFolder,vec<CString> &vsFile,vec<BOOL> &vbFromMemory)
{
	for(int i=0;i<vsFolder.Size();i++)
	{
		AddFile(vsFolder[i],vsFile[i],vbFromMemory[i]);
	}
}

// Refreshes from filesystem
void CFileHierarchy::Refresh()
{
	// Clear existing data
	m_vsFilename.Clear();
	m_vbFromMemory.Clear();
	m_vfhSubFolders.Clear();
	m_sName="";
	m_bExpanded=TRUE;
	m_bValid=TRUE;

	// Get data from filesystem
	vec<CString> vsFolder,vsFile;
	vec<BOOL> vbFromMemory;
	CFileSystem::sm_pCurrent->ListFiles(vsFolder,vsFile,vbFromMemory);

	// Set to this
	AddFiles(vsFolder,vsFile,vbFromMemory);

	// Sort
	Sort();
}

int __cdecl FileSort(const CString** pv1,const CString** pv2)
{
	return (*pv1)->CompareNoCase((**pv2));
}

int __cdecl FolderSort(const CFileHierarchy** pv1,const CFileHierarchy** pv2)
{
	return (*pv1)->m_sName.CompareNoCase((*pv2)->m_sName);
}

// Sort (and sort subfolders)
void CFileHierarchy::Sort()
{
	// Sort files
	m_vsFilename.Sort(FileSort);

	// Sort folders
	m_vfhSubFolders.Sort(FolderSort);

	// Now sort within folders
	for(int i=0;i<m_vfhSubFolders.Size();i++)
		m_vfhSubFolders[i].Sort();
}

// Paints all buttons as required (returns index of next unset button)
// assuming that the graphic has already been offset and clipped
int CFileHierarchy::Paint(CGraphic& gTarget,int iNumButtons,
	CGraphicAlpha* pgaPlus,CGraphicAlpha* pgaMinus,
	int iDepth,int iButtonIndex,
	int iActive,BOOL bOver,BOOL bPressed, // -1 if no mouseover/pressed
	const CString &sSelectedFolder,const CString &sSelectedFile,
	CString sThisFolder)
{
	// Normalise folder
	CFileSystem::NormaliseInternalFolder(sThisFolder);

	// Check validity
	if(!m_bValid) 
	{
		Refresh();
		ExpandDefault(iNumButtons);
	}

	// Bail out, if no more room
	if(iButtonIndex>=iNumButtons) return iButtonIndex;

	// Display 'none' if this is the level 0
	if(iDepth==0)
	{
		if(iButtonIndex>=0)
		{
			PaintFile(gTarget,iButtonIndex,"(No sound)",
				iActive,bOver,bPressed,
				sSelectedFolder=="" &&
				sSelectedFile=="");
		}
		iButtonIndex++;
		if(iButtonIndex>=iNumButtons) return iButtonIndex;
	}

	// First paint our files
	for(int iFile=0;iFile<m_vsFilename.Size();iFile++)
	{
		if(iButtonIndex>=0)
		{
			PaintFile(gTarget,iButtonIndex,m_vsFilename[iFile],
				iActive,bOver,bPressed,
				sSelectedFolder.CompareNoCase(sThisFolder)==0 &&
				sSelectedFile.CompareNoCase(m_vsFilename[iFile])==0);
		}
		iButtonIndex++;
		if(iButtonIndex>=iNumButtons) return iButtonIndex;
	}

	// Now set each subfolder
	for(int iFolder=0;iFolder<m_vfhSubFolders.Size();iFolder++)
	{
		// See if the folder is expanded or not
		if(m_vfhSubFolders[iFolder].m_bExpanded)
		{
			if(iButtonIndex>=0)
			{
				PaintFolder(gTarget,iButtonIndex,m_vfhSubFolders[iFolder].m_sName,
					iDepth,pgaPlus,pgaMinus,
					TRUE,iActive,bOver,bPressed);
			}
			iButtonIndex++;
			if(iButtonIndex>=iNumButtons) return iButtonIndex;

			iButtonIndex=
				m_vfhSubFolders[iFolder].Paint(gTarget,iNumButtons,
					pgaPlus,pgaMinus,
					iDepth+1,iButtonIndex,
					iActive,bOver,bPressed,
					sSelectedFolder,sSelectedFile,
					sThisFolder+m_vfhSubFolders[iFolder].m_sName);
			if(iButtonIndex>=iNumButtons) return iButtonIndex;
		}
		else
		{
			if(iButtonIndex>=0)
			{
				PaintFolder(gTarget,iButtonIndex,m_vfhSubFolders[iFolder].m_sName,
					iDepth,pgaPlus,pgaMinus,
					FALSE,iActive,bOver,bPressed);
			}
			iButtonIndex++;
			if(iButtonIndex>=iNumButtons) return iButtonIndex;
		}
	}

	return iButtonIndex;
}

// Responds to button request; sets folder,file if it's a file, 
// sets folder to !FOLDER (and caller should re-do SetButtons) if 
// it's a folder
int CFileHierarchy::ButtonClicked(
	int iDepth,
	int iButtonIndex,
	int iClickedIndex,
	CString& sFolder,CString& sFile,
	CString sThisFolder)
{
	// Normalise folder
	CFileSystem::NormaliseInternalFolder(sThisFolder);

	// Bail out, if no more room
	if(iButtonIndex>iClickedIndex) return iButtonIndex;

	// Display 'none' if this is the level 0
	if(iDepth==0)
	{
		if(iButtonIndex==iClickedIndex)
		{
			sFolder="";
			sFile="";
			return 1000000;
		}
		iButtonIndex++;
		if(iButtonIndex>iClickedIndex) return iButtonIndex;
	}

	// First check our files
	for(int iFile=0;iFile<m_vsFilename.Size();iFile++)
	{
		if(iButtonIndex==iClickedIndex)
		{
			sFolder=sThisFolder;
			sFile=m_vsFilename[iFile];
			return 1000000;
		}
		iButtonIndex++;
		if(iButtonIndex>iClickedIndex) return iButtonIndex;
	}

	// Now set each subfolder
	for(int iFolder=0;iFolder<m_vfhSubFolders.Size();iFolder++)
	{
		// See if the folder is expanded or not
		if(m_vfhSubFolders[iFolder].m_bExpanded)
		{
			if(iButtonIndex==iClickedIndex)
			{
				m_vfhSubFolders[iFolder].m_bExpanded=FALSE;
				sFolder="!FOLDER";
				return 1000000;
			}
			iButtonIndex++;
			if(iButtonIndex>iClickedIndex) return iButtonIndex;

			iButtonIndex=
				m_vfhSubFolders[iFolder].ButtonClicked(
					iDepth+1,
					iButtonIndex,
					iClickedIndex,
					sFolder,sFile,
					sThisFolder+m_vfhSubFolders[iFolder].m_sName);
			if(iButtonIndex>iClickedIndex) return iButtonIndex;
		}
		else
		{
			if(iButtonIndex==iClickedIndex)
			{
				m_vfhSubFolders[iFolder].m_bExpanded=TRUE;
				sFolder="!FOLDER";
				return iButtonIndex;
			}
			iButtonIndex++;
			if(iButtonIndex>iClickedIndex) return iButtonIndex;
		}
	}

	return iButtonIndex;
}

// Returns total number of (visible) file + folder lines
int CFileHierarchy::GetHeight(int iDepth)
{
	int iTotal=m_vsFilename.Size();
	if(iDepth==0) iTotal++;

	for(int iFolder=0;iFolder<m_vfhSubFolders.Size();iFolder++)
	{
		if(m_vfhSubFolders[iFolder].m_bExpanded)
			iTotal+=1+m_vfhSubFolders[iFolder].GetHeight(iDepth+1);
		else
			iTotal++;
	}

	return iTotal;
}

void CFileHierarchy::PaintFile(CGraphic& gTarget,int iButton,const char* pcText,
	int iActive,BOOL bOver,BOOL bPressed,BOOL bSelected)
{
	// Work out button state
	enum{NORMAL,OVER,PRESSED,SELECTED} state;
	if(bSelected) 
		state=SELECTED;
	else if(iActive==iButton)
		state=(bPressed & bOver) ? PRESSED : (bOver ? OVER : NORMAL);
	else
		state=NORMAL;

	// Pick colours based on state
	COLORREF clrFill,iFillAlpha,clrText;
	const COLORREF WHITE=RGB(255,255,255),BLACK=RGB(0,0,0);
	const int SOLID=256;
	switch(state)
	{
	case NORMAL:
		clrFill=WHITE; iFillAlpha=(60*256)/100; clrText=BLACK; break;
	case OVER:
		clrFill=WHITE; iFillAlpha=SOLID; clrText=BLACK; break;
	case PRESSED:
		clrFill=BLACK; iFillAlpha=(20*256)/100; clrText=BLACK; break;
	case SELECTED:
		clrFill=BLACK; iFillAlpha=(20*256)/100; clrText=BLACK; break;
	}

	// Draw background
	gTarget.FillRect(0,17*iButton,136,15,clrFill,iFillAlpha);

	// Trim text
	CString sText=pcText;
	if(sText.Right(4).CompareNoCase(".wav")==0) sText=sText.Left(sText.GetLength()-4);
	if(sText.Right(4).CompareNoCase(".ogg")==0) sText=sText.Left(sText.GetLength()-4);

	// Draw text
	m_gfFont.Draw(&gTarget,4,17*iButton+1,
		sText,clrText);
}

void CFileHierarchy::PaintFolder(CGraphic& gTarget,int iButton,const char* pcText,
	int iDepth,CGraphicAlpha* pgaPlus,CGraphicAlpha* pgaMinus,
	BOOL bExpanded,int iActive,BOOL bOver,BOOL bPressed)
{
	// Work out button state
	enum{NORMAL,OVER,PRESSED} state;
	if(iActive==iButton)
		state=(bPressed & bOver) ? PRESSED : (bOver ? OVER : NORMAL);
	else
		state=NORMAL;

	// Pick colours based on state
	COLORREF clrFill,iFillAlpha,clrText;
	const COLORREF WHITE=RGB(255,255,255),BLACK=RGB(0,0,0);
	const int SOLID=256;
	switch(state)
	{
	case NORMAL:
		clrFill=BLACK; iFillAlpha=(60*256)/100; clrText=WHITE; break;
	case OVER:
		clrFill=BLACK; iFillAlpha=SOLID; clrText=WHITE; break;
	case PRESSED:
		clrFill=RGB(128,128,128); iFillAlpha=SOLID; clrText=WHITE; break;
	}

	// Draw background
	gTarget.FillRect(0,17*iButton,136,15,clrFill,iFillAlpha);

	// Draw text
	m_gfFont.Draw(&gTarget,17+13*iDepth,17*iButton+1,
		pcText,clrText);

	// Draw icon
	CGraphicAlpha* pgaIcon=(bExpanded ? pgaMinus : pgaPlus);
	pgaIcon->Copy(&gTarget,4+13*iDepth,17*iButton+3);
}

// Expand so that the selected file is visible
BOOL CFileHierarchy::ExpandToShow(
	const CString &sSelectedFolder,const CString &sSelectedFile,
	CString sThisFolder)
{
	// Normalise folder
	CFileSystem::NormaliseInternalFolder(sThisFolder);

	// First check our files
	if(sSelectedFolder.CompareNoCase(sThisFolder)==0)
	{
		for(int iFile=0;iFile<m_vsFilename.Size();iFile++)
		{
			if(sSelectedFile.CompareNoCase(m_vsFilename[iFile])==0)
			{
				m_bExpanded=TRUE;
				return TRUE;
			}
		}
	}

	// Now check each subfolder
	for(int iFolder=0;iFolder<m_vfhSubFolders.Size();iFolder++)
	{
		if(m_vfhSubFolders[iFolder].ExpandToShow(
			sSelectedFolder,sSelectedFile,
			sThisFolder+m_vfhSubFolders[iFolder].m_sName))
		{
			m_bExpanded=TRUE;
			return TRUE;
		}
	}

	return FALSE;
}

// Expand enough levels as can be expanded without running out of space
void CFileHierarchy::ExpandDefault(int iNumButtons)
{
	for(int i=0;i<5;i++)
	{
		ExpandLevels(i);
		if(GetHeight()>iNumButtons)
		{
			if(i>0)
				ExpandLevels(i-1);
			break;
		}
	}
}

// Expands all items up to given level, and un-expands the rest
void CFileHierarchy::ExpandLevels(int iLevel,int iDepth)
{
	m_bExpanded=(iDepth<=iLevel);

	for(int iFolder=0;iFolder<m_vfhSubFolders.Size();iFolder++)
	{
		m_vfhSubFolders[iFolder].ExpandLevels(iLevel,iDepth+1);
	}
}
