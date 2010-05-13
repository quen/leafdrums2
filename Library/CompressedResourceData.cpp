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
#include "zlib.h"
#include "CompressedResourceData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CCompressedResourceData::CCompressedResourceData(const char* pstrName, const char* pstrType)
	: CResourceData(pstrName,pstrType)
{
	m_pbData=NULL;

	// Get the size from the first int
	ASSERT(CResourceData::Size()>sizeof(int));
	ASSERT(sizeof(DWORD)==sizeof(int));
	CopyMemory(&m_dwSize,CResourceData::Data(),sizeof(int));

	// Allocate memory from that
	m_pbData=new BYTE[m_dwSize];

	// Decompress data from resource into here
	z_stream zs;
	zs.zalloc = (alloc_func)0;
	zs.zfree = (free_func)0;
	zs.opaque = (voidpf)0;

	zs.next_in  = &CResourceData::Data()[sizeof(int)];
	zs.avail_in = CResourceData::Size()-sizeof(int);
	zs.next_out = m_pbData;
	zs.avail_out= m_dwSize;

	if(inflateInit(&zs)!=Z_OK)
		throw "Error initialising decompression for scheme file";

	if(inflate(&zs,Z_FINISH)!=Z_STREAM_END) throw "Error decompressing scheme file";

	inflateEnd(&zs);

	if(zs.avail_out!=0) throw "Scheme file corrupt";
}

CCompressedResourceData::~CCompressedResourceData()
{
	if(m_pbData) delete m_pbData;
}
