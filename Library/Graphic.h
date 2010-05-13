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
#ifndef GRAPHIC_H
#define GRAPHIC_H

#include "vec.h"

class CGraphicClip
{
public:
	int m_iClipX,m_iClipY,m_iClipWidth,m_iClipHeight;
};

class CGraphicOffset
{
public:
	int m_iOffsetX,m_iOffsetY;
};

class CGraphic
{
private:
	// 24-bit image and its vital statistics
	int m_iWidth,m_iHeight,m_iByteWidth;
	BYTE* m_pbData;

	// Clip/offset stacks (last is current)
	vec<CGraphicClip> m_vgcClipStack;
	vec<CGraphicOffset> m_vgoOffsetStack;

public:
	// Constructors which manage memory
	CGraphic(int iWidth,int iHeight);
	~CGraphic();

	// Clip data access
	int ClipX() const { return m_vgcClipStack[m_vgcClipStack.Size()-1].m_iClipX; }
	int ClipY() const { return m_vgcClipStack[m_vgcClipStack.Size()-1].m_iClipY; }
	int ClipWidth() const { return m_vgcClipStack[m_vgcClipStack.Size()-1].m_iClipWidth; }
	int ClipHeight() const { return m_vgcClipStack[m_vgcClipStack.Size()-1].m_iClipHeight; }
	int OffsetX() const { return m_vgoOffsetStack[m_vgoOffsetStack.Size()-1].m_iOffsetX; }
	int OffsetY() const { return m_vgoOffsetStack[m_vgoOffsetStack.Size()-1].m_iOffsetY; }

	// Access to the data
	BYTE* Data() { return m_pbData; }
	int Width()	 const { return m_iWidth; }
	int Height() const { return m_iHeight; }
	int ByteWidth() const { return m_iByteWidth; }

	// Clipping
	void Clip(int iX,int iY,int iWidth,int iHeight);
	void Offset(int iOffsetX,int iOffsetY);
	void UnClip();
	void UnOffset();

	// Copy into another CGraphic
	void Copy(CGraphic* pgrTarget)		// Copies entire graphic into 0,0
		{ Copy(pgrTarget,0,0); }
	void Copy(CGraphic* pgrTarget,		// Copies entire graphic to given
		int iDestinationX, int iDestinationY)
		{ Copy(pgrTarget,iDestinationX,iDestinationY,0,0,m_iWidth,m_iHeight); }
	void Copy(CGraphic* pgrTarget,		// Copies some of graphic
		int iDestinationX, int iDestinationY,
		int iSourceX,int iSourceY,int iSourceWidth,int iSourceHeight);

	// Assume that we're going "up to but not equal" the BR point and see if rect is clipped
	BOOL EntirelyClipped(int iTLX,int iTLY,int iBRX,int iBRY) const;
		
	// Fill a rectangle with colour
	void FillRect(int iX,int iY,int iWidth,int iHeight,COLORREF clr);

	// Fill including alpha
	void FillRect(int iX,int iY,int iWidth,int iHeight,COLORREF clr,int iAlpha);

	// Draw a line including alpha
	void DrawLine(int iX1,int iY1,int iX2,int iY2,COLORREF clr,int iAlpha=256);

	// Set a single point (with alpha)
	void SetPixel(int iX,int iY,COLORREF clr,int iAlpha=256);
	COLORREF GetPixel(int iX,int iY);

	// Invert
	void Invert();

	// Resample
	void Resample(int iWidth,int iHeight);

	// Load the data from disk or resource
	BOOL LoadFile(const char* pstrPath);
	BOOL LoadResource(const char* pstrResource);
	BOOL LoadCompressedResource(const char* pstrResource);
};

#endif // GRAPHIC_H