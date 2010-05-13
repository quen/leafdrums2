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
#include "ResourceData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CResourceData::CResourceData(const char* strName, const char* strType)
{
	// Account for default type
	if(strType==NULL) strType="Raw";

	// Find and load resource
	HRSRC hrsrc=FindResource(AfxGetResourceHandle(),strName,strType);
	if(hrsrc==NULL) AfxThrowResourceException();

	m_dwSize=SizeofResource(AfxGetResourceHandle(),hrsrc);

	HGLOBAL hglb=LoadResource(AfxGetResourceHandle(),hrsrc);
	if(hglb==NULL) AfxThrowResourceException();

	m_pbData=(BYTE*)LockResource(hglb);
	if(m_pbData==NULL) AfxThrowResourceException();
}

BOOL CResourceData::Exists(const char* strName, const char* strType)
{
	// Account for default type
	if(strType==NULL) strType="Raw";

	// Find and load resource
	HRSRC hrsrc=FindResource(AfxGetResourceHandle(),strName,strType);
	return hrsrc!=NULL;
}
