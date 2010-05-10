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
#ifndef FRAMEANIMATION_H
#define FRAMEANIMATION_H

class CFrameAnimation
{
private:
	void InvertHollowRect(CDC &dc,int iX1,int iY1,int iX2,int iY2);
	int Interpolate(int i1,int i2,int iPos);

public:
	CFrameAnimation(CWnd* pWnd,
		int iStartX1,int iStartY1,int iStartX2,int iStartY2,
		int iEndX1,int iEndY1,int iEndX2,int iEndY2);
};

#endif //  FRAMEANIMATION_H
