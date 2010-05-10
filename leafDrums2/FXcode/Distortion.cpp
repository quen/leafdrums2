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
#include "Distortion.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_SERIAL(CDistortion,CEffect,1)

CDistortion::CDistortion()
{
	AddParam("Distort",256,5000);
	AddParam("Quieten",0,256);
}

void CDistortion::ProcessBlock(CBlock* &pBlock)
{
	int iWords=pBlock->Size();
	if(pBlock->Stereo()) iWords*=2;

	short* psData=pBlock->Data();

	int iDistort=GetParam(0);
	int iQuieten=GetParam(1);
	int iLevel=256-iQuieten;

	for(int i=0;i<iWords;i++)
	{
		int iEffect=psData[i];
		iEffect=(iEffect*iDistort)/256;
		if(iEffect>32767) iEffect=32767;
		else if(iEffect<-32768) iEffect=-32768;

		psData[i]=(iLevel*iEffect)/256;
	}
}

void CDistortion::Reset()
{
	int i=1;
}

CString CDistortion::Name()
{
	CString strTemp="Distortion";
	return strTemp;
}

void CDistortion::Serialize(CArchive& archive)
{
	CEffect::Serialize(archive);
}