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
#ifndef FILEHIERARCHY_H
#define FILEHIERARCHY_H

#include "vec.h"
#include "GraphicAlpha.h"
#include "GraphicFont.h"

class CFileHierarchy
{
private:
	// Information about files
	vec<CString> m_vsFilename;
	vec<BOOL> m_vbFromMemory;

	// Subfolders
	vec<CFileHierarchy> m_vfhSubFolders;

	// Name of this folder (blank for root)
	CString m_sName;

	// Whether or not folder is expanded
	BOOL m_bExpanded;

	// Whether or not data is valid
	BOOL m_bValid;

	// Font used in painting
	CGraphicFont m_gfFont;

	// Adds a file to this or to subfolders (creating as necessary)
	void AddFile(CString sFolder,CString sFile,BOOL bFromMemory);

	// Adds a list of files
	void AddFiles(vec<CString> &vsFolder,vec<CString> &vsFile,vec<BOOL> &vbFromMemory);

	// Construct internal level
	CFileHierarchy(const char* pcName) : m_gfFont("Verdana",11,FALSE) { m_bExpanded=FALSE; m_bValid=TRUE; m_sName=pcName; }

	// Sort (and sort subfolders)
	void Sort();

	// Paints single button
	void PaintFile(CGraphic& gTarget,int iButton,const char* pcText,
		int iActive,BOOL bOver,BOOL bPressed,BOOL bSelected); 
	void PaintFolder(CGraphic& gTarget,int iButton,const char* pcText,
		int iDepth,CGraphicAlpha* pgaPlus,CGraphicAlpha* pgaMinus,
		BOOL bExpanded,int iActive,BOOL bOver,BOOL bPressed);

	// Expands all items up to given level, and un-expands the rest
	void ExpandLevels(int iLevel,int iDepth=0);

public:
	// Construct/destruct
	CFileHierarchy() : 	m_gfFont("Verdana",11,FALSE)
		{ m_bExpanded=TRUE; m_bValid=FALSE; }

	// Refreshes from filesystem
	void Refresh();

	// Marks invalid (needing refresh)
	void MarkInvalid() { m_bValid=FALSE; }

	// Paints all buttons as required (returns index of next unset button)
	// assuming that the graphic has already been offset and clipped
	int Paint(CGraphic& gTarget,int iNumButtons,
		CGraphicAlpha* pgaPlus,CGraphicAlpha* pgaMinus,
		int iDepth,int iButtonIndex,
		int iActive,BOOL bOver,BOOL bPressed, // -1 if no mouseover/pressed
		const CString &sSelectedFolder,const CString &sSelectedFile,
		CString sThisFolder=""); 

	// Responds to button request; sets folder,file if it's a file, 
	// sets folder to !FOLDER (and caller should re-do Paint) if 
	// it's a folder
	int ButtonClicked(
		int iDepth,
		int iButtonIndex,
		int iClickedIndex,
		CString& sFolder,CString& sFile,
		CString sThisFolder="");

	// Returns total number of (visible) files + folders
	int GetHeight(int iDepth=0);

	// Expand so that the selected file is visible
	BOOL ExpandToShow(const CString &sSelectedFolder,const CString &sSelectedFile,
		CString sThisFolder="");

	// Expand enough levels as can be expanded without running out of space
	void ExpandDefault(int iNumButtons);

	friend int __cdecl FolderSort(const CFileHierarchy** pv1,const CFileHierarchy** pv2);
};

#endif // FILEHIERARCHY_H 
