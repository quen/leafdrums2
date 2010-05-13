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
#ifndef GRAPHICONSCREEN_H
#define GRAPHICONSCREEN_H

#include "Graphic.h"

class CGraphicOnscreen : public CGraphic
{
private:
	// Bitmap info for 24-bit version
	BITMAPINFO m_BI24Bit;

	// Bitmap info structure and palette entries for 8-bit version
	BITMAPINFO m_BI8Bit;
	RGBQUAD m_temp[256];

	// 8-bit data and up-to-date flag
	BYTE* m_pb8BitData;
	int m_i8BitByteWidth;
	BOOL m_f8BitUpToDate;

public:
	// Constructor and destructor, handle memory allocation
	CGraphicOnscreen(int iWidth,int iHeight);
	~CGraphicOnscreen();

	// Mark changed
	void Changed();

	// Drawing functions
	void Draw(CDC* pDC,int iDestX,int iDestY)	// Draw whole bitmap
		{ Draw(pDC,iDestX,iDestY,0,0,Width(),Height()); }
	void Draw(CDC* pDC,int iDestX,int iDestY,	// Draw some of bitmap
		int iSourceX,int iSourceY,int iSourceWidth,int iSourceHeight);
};

#endif GRAPHICONSCREEN_H