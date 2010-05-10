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

#include "Block.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CBlock* CBlock::Create(BOOL fStereo,int iSize)
{
	// Work out how much RAM to allocate
	int iBytes=iSize*2;
	if(fStereo) iBytes*=2;
	iBytes+=sizeof(CBlock);

	// Allocate the memory
	BYTE* pbData=new BYTE[iBytes];
	CBlock* pBlock=(CBlock*) pbData;
	pBlock->m_fStereo=fStereo;
	pBlock->m_iSize=iSize;

	return pBlock;
}

void CBlock::operator delete(void* pData)
{
	delete (BYTE*) pData;
}
