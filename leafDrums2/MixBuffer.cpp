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
#include "MixBuffer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CMixBuffer::CMixBuffer()
{
	m_plData=NULL;
	m_iSamples=0;
}

CMixBuffer::~CMixBuffer()
{
	if(m_plData) delete m_plData;
}

void CMixBuffer::Create(int iSize)
{
	// Delete existing MixBuffer
	if(m_plData) delete m_plData;

	// Allocate new MixBuffer
	m_plData=new long[iSize*2];
	m_iSamples=iSize;
	FillMemory(m_plData,m_iSamples*2*sizeof(long),0);
}


