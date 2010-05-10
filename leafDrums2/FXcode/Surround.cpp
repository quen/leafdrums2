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
#include "Surround.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_SERIAL(CSurround,CEffect,1)

CSurround::CSurround()
{
	AddParam("Effect level",0,256);
}

void CSurround::ProcessBlock(CBlock* &pBlock)
{
	int iWords=pBlock->Size();
	if(pBlock->Stereo()) iWords*=2;

	short* psData=pBlock->Data();

	int iWet=GetParam(0);
	int iDry=256-iWet;

	for(int i=0;i<iWords;i++)
	{
		// Skip L
		i++;

		// Process R
		int iEffect=-psData[i];

		psData[i]=((iWet*iEffect)+(iDry*psData[i]))/256;
	}
}

void CSurround::Reset()
{
	int i=1;
}

CString CSurround::Name()
{
	CString strTemp="Surround (invert R)";
	return strTemp;
}

void CSurround::Serialize(CArchive& archive)
{
	CEffect::Serialize(archive);
}