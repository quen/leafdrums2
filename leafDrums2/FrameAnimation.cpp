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
#include "FrameAnimation.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define NUMFRAMES 20

void CFrameAnimation::InvertHollowRect(CDC &dc,
	int iX1,int iY1,int iX2,int iY2)
{
	CRect rect;

	rect.SetRect(iX1,iY1,iX2,iY1+1);
	dc.InvertRect(&rect);

	rect.SetRect(iX1,iY2-1,iX2,iY2);
	dc.InvertRect(&rect);

	rect.SetRect(iX1,iY1,iX1+1,iY2);
	dc.InvertRect(&rect);

	rect.SetRect(iX2-1,iY1,iX2,iY2);
	dc.InvertRect(&rect);
}

int CFrameAnimation::Interpolate(int i1,int i2,int iPos)
{
	return i1+((i2-i1)*iPos)/NUMFRAMES;
}

CFrameAnimation::CFrameAnimation(CWnd* pWnd,
	int iStartX1,int iStartY1,int iStartX2,int iStartY2,
	int iEndX1,int iEndY1,int iEndX2,int iEndY2)
{
	CClientDC dc(pWnd);

	for(int i=0;i<=NUMFRAMES+1;i++)
	{
		// Remove last frame
		if(i!=0)
			InvertHollowRect(dc,
				Interpolate(iStartX1,iEndX1,i-1),
				Interpolate(iStartY1,iEndY1,i-1),
				Interpolate(iStartX2,iEndX2,i-1),
				Interpolate(iStartY2,iEndY2,i-1));

		// If we've just undrawn final frame, break out
		if(i==NUMFRAMES+1) break;

		// Draw this frame
		InvertHollowRect(dc,
			Interpolate(iStartX1,iEndX1,i),
			Interpolate(iStartY1,iEndY1,i),
			Interpolate(iStartX2,iEndX2,i),
			Interpolate(iStartY2,iEndY2,i));

		// Pause some so that user gets to see it
		Sleep(150/NUMFRAMES);
	}
}
