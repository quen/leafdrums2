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
#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include "vec.h"
#include "Song.h"

class CFileSystem
{
private:
	// Data about stored files
	vec<short*> m_vpsStoredData;
	vec<int> m_viStoredDataLength;
	vec<CString> m_vsStoredFolder;
	vec<CString> m_vsStoredFilename;

	// Root paths
	vec<CString> m_vsRootPaths;

	// List files in a particular folder (and subfolders)
	void ListFiles(CString sRoot,CString sFolder,
		vec<CString>& vsFolder,vec<CString>& vsFile,vec<BOOL>& vbFromMemory);

	// Clear stored data (at start of load)
	void Clear();

	// Add stored data (during load)
	void AddData(const char* pcFolder,const char* pcFilename,
		short* psData,int iDataLength);

public:
	// Current instance
	static CFileSystem* sm_pCurrent;

	// Constructor (loads settings)
	CFileSystem();

	// Destructor (frees memory, saves settings)
	~CFileSystem();

	// Serialize sounds
	void LoadSounds(CArchive& a);
	void SaveSounds(CArchive& a,CSong& s,BOOL bEmbedAll);

	// Obtain information about data in memory
	BOOL IsInMemory(const char* pcFolder,const char* pcFilename);
	short* GetData(const char* pcFolder,const char* pcFilename);
	int GetDataLength(const char* pcFolder,const char* pcFilename);

	// Obtain information about real disk files
	BOOL IsFile(const char* pcFolder,const char* pcFilename);
	CString GetFile(const char* pcFolder,const char* pcFilename);

	// Get list of all files
	void ListFiles(vec<CString>& vstrFolder,vec<CString>& vstrFile,vec<BOOL>& vbFromMemory);

	// Return the program's install folder
	static CString GetProgramFolder();

	// Normalise a folder (make sure it ends in /)
	static void NormaliseFolder(CString& strFolder);

	// Normalise internal folder (same as NormaliseFolder, but trims initial /)
	static void NormaliseInternalFolder(CString& strFolder);

	// Get data about current folders
	int GetNumRoots() { return m_vsRootPaths.Size(); }
	CString& GetRoot(int iIndex) { return m_vsRootPaths[iIndex]; }
	void DeleteRoot(int iIndex) { m_vsRootPaths.RemoveAt(iIndex); }
	void AddRoot(CString sRoot) { m_vsRootPaths.Add(sRoot); }
	void SetDefaultRoot(int iIndex) 
	{
		m_vsRootPaths.InsertAt(0,GetRoot(iIndex));
		DeleteRoot(iIndex+1);	
	}
};

#endif // FILESYSTEM_H
