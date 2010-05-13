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
#ifndef GraphicAlphaChannel_H
#define GraphicAlphaChannel_H

#include "Graphic.h"

class CGraphicAlphaChannel
{
private:
	// 8-bit channel and its vital statistics
	int m_iWidth,m_iHeight;
	BYTE* m_pbData;

public:
	// Constructors which manage memory
	CGraphicAlphaChannel(int iWidth,int iHeight);
	~CGraphicAlphaChannel();

	// Access to the data
	BYTE* Data() { return m_pbData; }
	int Width()	 { return m_iWidth; }
	int Height() { return m_iHeight; }

	// Draw into a CGraphic using transparency
	void Draw(CGraphic* pgrTarget,		// Copies entire graphic to given
		int iDestinationX, int iDestinationY,
		COLORREF clr,COLORREF clrBG=0xffffffff)
		{ Draw(pgrTarget,iDestinationX,iDestinationY,0,0,m_iWidth,m_iHeight,clr,clrBG); }
	void Draw(CGraphic* pgrTarget,		// Copies some of graphic
		int iDestinationX, int iDestinationY,
		int iSourceX,int iSourceY,int iSourceWidth,int iSourceHeight,
		COLORREF clr,COLORREF clrBG=0xffffffff);

	// Copy to another alpha channel
	void Copy(CGraphicAlphaChannel* pgacTarget,		
		int iDestinationX, int iDestinationY,
		int iSourceX,int iSourceY,int iSourceWidth,int iSourceHeight);
		
	// Load the data from disk or resource
	BOOL LoadFile(const char* pstrPath);
	BOOL LoadResource(const char* pstrResource);

	// Resample
	void Resample(int iWidth,int iHeight);
	CGraphicAlphaChannel* ResampleToNew(int iWidth,int iHeight); // Creates a new image

	// Rotate (switches x with y)
	void Rotate();

	// Flips
	void FlipHorizontal();
};

#endif // GraphicAlphaChannel_H