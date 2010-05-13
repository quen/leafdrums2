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
#ifndef COMPRESSEDRESOURCEDATA_H
#define COMPRESSEDRESOURCEDATA_H

#include "ResourceData.h"

class CCompressedResourceData : public CResourceData
{
private:
	BYTE* m_pbData;	// Uncompressed resource data
	DWORD m_dwSize;	// Size of data

public:
	// Open the resource
	CCompressedResourceData(const char* pstrName, const char* pstrType);
	~CCompressedResourceData();

	// Get data and size
	BYTE* Data() { return m_pbData; }
	DWORD Size() { return m_dwSize; }
};

#endif // COMPRESSEDRESOURCEDATA_H