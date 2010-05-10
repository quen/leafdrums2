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
#include "FileSystem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Current instance
CFileSystem* CFileSystem::sm_pCurrent;

// Constructor (loads settings)
CFileSystem::CFileSystem()
{
	// This is the new static filesystem
	sm_pCurrent=this;

	// Load folder settings
	int iCount=AfxGetApp()->GetProfileInt("Root folders","Folder count",0);
	for(int iFolder=0;iFolder<iCount;iFolder++)
	{
		// Find and add this folder
		CString strTemp;
		strTemp.Format("Folder %i",iFolder);
		CString strFolder=AfxGetApp()->GetProfileString("Root folders",strTemp);
		NormaliseFolder(strFolder);
		m_vsRootPaths.Add(strFolder);
	}

	// If count is zero, add one with our path + 'Sounds'
	if(m_vsRootPaths.Size()==0)
	{
		m_vsRootPaths.Add("Sounds/");
	}
}

// Destructor (frees memory, saves settings)
CFileSystem::~CFileSystem()
{
	// No filesystem any more
	sm_pCurrent=NULL;

	// Delete stored data
	Clear();

	// Save settings
	AfxGetApp()->WriteProfileInt("Root folders","Folder count",m_vsRootPaths.Size());
	for(int iFolder=0;iFolder<m_vsRootPaths.Size();iFolder++)
	{
		CString strTemp;
		strTemp.Format("Folder %i",iFolder);
		AfxGetApp()->WriteProfileString("Root folders",strTemp,
			m_vsRootPaths[iFolder]);
	}
}

// Clear stored data (at start of load)
void CFileSystem::Clear()
{
	for(int i=0;i<m_vpsStoredData.Size();i++)
		delete m_vpsStoredData[i];

	m_vpsStoredData.Clear();
	m_viStoredDataLength.Clear();
	m_vsStoredFolder.Clear();
	m_vsStoredFilename.Clear();
}

// Add stored data (during load)
void CFileSystem::AddData(const char* pcFolder,const char* pcFilename,
	short* psData,int iDataLength)
{
	// Check it doesn't exist
	if(IsInMemory(pcFolder,pcFilename)) return;

	m_vpsStoredData.Add(psData);
	m_viStoredDataLength.Add(iDataLength);
	CString* pstrFolder=new CString(pcFolder);
	NormaliseInternalFolder(*pstrFolder);
	m_vsStoredFolder.Add(pstrFolder);
	m_vsStoredFilename.Add(new CString(pcFilename));
}

// Obtain information about data in memory
BOOL CFileSystem::IsInMemory(const char* pcFolder,const char* pcFilename)
{
	CString sFolder=pcFolder;
	NormaliseInternalFolder(sFolder);
	for(int iIndex=0;iIndex<m_vpsStoredData.Size();iIndex++)
	{
		if(m_vsStoredFolder[iIndex].CompareNoCase(sFolder)==0 && 
			m_vsStoredFilename[iIndex].CompareNoCase(pcFilename)==0)
			return TRUE;
	}

	return FALSE;
}

short* CFileSystem::GetData(const char* pcFolder,const char* pcFilename)
{
	CString sFolder=pcFolder;
	NormaliseInternalFolder(sFolder);
	for(int iIndex=0;iIndex<m_vpsStoredData.Size();iIndex++)
	{
		if(m_vsStoredFolder[iIndex].CompareNoCase(sFolder)==0 && 
			m_vsStoredFilename[iIndex].CompareNoCase(pcFilename)==0)
		{
			return m_vpsStoredData[iIndex];
		}
	}
	ASSERT(FALSE);
	return NULL;
}

int CFileSystem::GetDataLength(const char* pcFolder,const char* pcFilename)
{
	CString sFolder=pcFolder;
	NormaliseInternalFolder(sFolder);
	for(int iIndex=0;iIndex<m_vpsStoredData.Size();iIndex++)
	{
		if(m_vsStoredFolder[iIndex].CompareNoCase(sFolder)==0 && 
			m_vsStoredFilename[iIndex].CompareNoCase(pcFilename)==0)
		{
			return m_viStoredDataLength[iIndex];
		}
	}
	ASSERT(FALSE);
	return 0;
}

// Obtain information about real disk files
BOOL CFileSystem::IsFile(const char* pcFolder,const char* pcFilename)
{
	// Check the folder path ends with /
	CString sFolder=pcFolder;
	NormaliseInternalFolder(sFolder);

	// Look in each root
	for(int iRoot=0;iRoot<m_vsRootPaths.Size();iRoot++)
	{
		// Get this root path
		CString sRoot=m_vsRootPaths[iRoot];
		NormaliseFolder(sRoot);

		// If it's a relative path, add the software path
		if(!(sRoot.Find(':')!=-1 || sRoot.Left(1).FindOneOf("\\/")!=-1))
		{
			sRoot=GetProgramFolder()+"/"+sRoot;
		}

		// Add the folder and filename and see if it exists
		CFileStatus fs;
		if(CFile::GetStatus(sRoot+sFolder+pcFilename,fs))
			return TRUE;
	}
	return FALSE;
}

CString CFileSystem::GetFile(const char* pcFolder,const char* pcFilename)
{
	// Check the folder path ends with /
	CString sFolder=pcFolder;
	NormaliseInternalFolder(sFolder);

	// Look in each root
	for(int iRoot=0;iRoot<m_vsRootPaths.Size();iRoot++)
	{
		// Get this root path
		CString sRoot=m_vsRootPaths[iRoot];
		NormaliseFolder(sRoot);

		// If it's a relative path, add the software path
		if(!(sRoot.Find(':')!=-1 || sRoot.Left(1).FindOneOf("\\/")!=-1))
		{
			sRoot=GetProgramFolder()+"/"+sRoot;
		}

		// Add the folder and filename and see if it exists
		CFileStatus fs;
		if(CFile::GetStatus(sRoot+sFolder+pcFilename,fs))
			return sRoot+sFolder+pcFilename;
	}
	ASSERT(FALSE);
	return "";
}

// Get list of all files
void CFileSystem::ListFiles(vec<CString>& vsFolder,vec<CString>& vsFile,
	vec<BOOL>& vbFromMemory)
{
	// 1. List memory files
	for(int iMemory=0;iMemory<m_vpsStoredData.Size();iMemory++)
	{
		vsFolder.Add(m_vsStoredFolder[iMemory]);
		vsFile.Add(m_vsStoredFilename[iMemory]);
		vbFromMemory.Add(TRUE);
	}

	// 2. Scan for disk files
	for(int iRoot=0;iRoot<m_vsRootPaths.Size();iRoot++)
	{
		// Get this root path
		CString sRoot=m_vsRootPaths[iRoot];
		NormaliseFolder(sRoot);

		// If it's a relative path, add the software path
		if(!(sRoot.Find(':')!=-1 || sRoot.Left(1).FindOneOf("\\/")!=-1))
		{
			sRoot=GetProgramFolder()+"/"+sRoot;
		}

		// Scan for *.wav,ogg
		ListFiles(sRoot,"/",vsFolder,vsFile,vbFromMemory);
	}
}

void CFileSystem::ListFiles(CString sRoot,CString sFolder,
	vec<CString>& vsFolder,vec<CString>& vsFile,vec<BOOL>& vbFromMemory)
{
	CFileFind ffWav;
	BOOL bContinue=ffWav.FindFile(sRoot+sFolder+"*");
	while(bContinue)
	{
		bContinue=ffWav.FindNextFile();

		if(ffWav.GetFileName()==".." || ffWav.GetFileName()==".") continue;
		CString sFound=ffWav.GetFilePath();		

		// Check for directories
		if(ffWav.IsDirectory())
		{
			ListFiles(sRoot,sFolder+ffWav.GetFileName()+"/",
				vsFolder,vsFile,vbFromMemory);
		}
		else if(sFound.Right(4).CompareNoCase(".wav")==0 || sFound.Right(4).CompareNoCase(".ogg")==0 )
		{
			vsFolder.Add(sFolder);
			vsFile.Add(ffWav.GetFileName());
			vbFromMemory.Add(FALSE);
		}
	}
}

// Return the program's install folder
CString CFileSystem::GetProgramFolder()
{
	char strCD[512];
	::GetModuleFileName(NULL,strCD,sizeof(strCD));

	// Remove all after last (back)slash
	for(int iPos=lstrlen(strCD);strCD[iPos]!='\\' &&
		strCD[iPos]!='/' && iPos>=0;iPos--);
	strCD[iPos]=0;

	return CString(strCD);
}

// Normalise a folder (make sure it ends in /)
void CFileSystem::NormaliseFolder(CString& strFolder)
{
	if(strFolder.Right(1).FindOneOf("\\/")==-1)
		strFolder+="/";
	else 
	{
		while(strFolder.Right(2).Left(1).FindOneOf("\\/")!=-1)
		{
			strFolder=strFolder.Left(strFolder.GetLength()-1);
		}
	}
}

// Normalise internal folder (same as NormaliseFolder, but trims initial /)
void CFileSystem::NormaliseInternalFolder(CString& strFolder)
{
	NormaliseFolder(strFolder);
	while(strFolder.Left(1).FindOneOf("\\/")!=-1)
		strFolder=strFolder.Mid(1);
}

// Serialize sounds
void CFileSystem::SaveSounds(CArchive& a,CSong& s,BOOL bEmbedAll)
{
	// Store version
	int iVersion=1;
	a << iVersion;

	// Work out data that needs saving
	vec<CString> vstrEmbed;
	vec<CSound*> vpsEmbedSounds;
	for(int iSound=0;iSound<s.GetNumSounds();iSound++)
	{
		// Get basic data
		CString 
			strFile=s.GetSound(iSound).GetFile(),
			strFolder=s.GetSound(iSound).GetFolder();

		// Ignore blank files
		if(strFile=="") continue;

		NormaliseFolder(strFolder);

		// If embed all, we definitely need to embed it, otherwise
		// check if it can be found as a file
		if(bEmbedAll || !IsFile(strFolder,strFile))
		{
			// Ok, add to list if it isn't already there
			if(vstrEmbed.Find(strFolder+"*"+strFile)==-1)
			{
				vstrEmbed.Add(strFolder+"*"+strFile);
				vpsEmbedSounds.Add(&s.GetSound(iSound));
			}
		}
	}

	// Get count
	int iCount=vstrEmbed.Size();
	a << iCount;

	// Store files
	for(int iEmbed=0;iEmbed<vstrEmbed.Size();iEmbed++)
	{
		// Get basic details
		CString 
			sFolder=vstrEmbed[iEmbed].Left(vstrEmbed[iEmbed].Find('*')),
			sFilename=vstrEmbed[iEmbed].Mid(vstrEmbed[iEmbed].Find('*')+1);

		// Store basic details
		a << sFolder;
		a << sFilename;

		int iLength=vpsEmbedSounds[iEmbed]->GetOriginal().Size();
		a << iLength;

		short *psData=vpsEmbedSounds[iEmbed]->GetOriginal().Data();
		for(int iData=0;iData<iLength*2;iData++)
			a << psData[iData];
	}
}

void CFileSystem::LoadSounds(CArchive& a)
{
	// Check version
	int iVersion;
	a >> iVersion;
	if(iVersion!=1)
		throw "File format not supported. Make sure this is a valid file and you are "
		  "using the latest version of leafDrums.";

	// Clear data
	Clear();

	// Get count
	int iCount;
	a >> iCount;

	// Read files
	for(int i=0;i<iCount;i++)
	{
		CString sFolder,sFilename;
		a >> sFolder;
		a >> sFilename;

		int iLength;
		a >> iLength;

		short* psData=new short[iLength*2];
		for(int iData=0;iData<iLength*2;iData++)
			a >> psData[iData];

		AddData(sFolder,sFilename,psData,iLength);
	}
}
