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
#ifndef GRAPHICALPHA_H
#define GRAPHICALPHA_H

#include "Graphic.h"

class CGraphicAlpha
{
private:
	// 32-bit image and its vital statistics
	int m_iWidth,m_iHeight;
	BYTE* m_pbData;

	// Clipping rectangle and position offset
	int m_iClipX,m_iClipY,m_iClipWidth,m_iClipHeight;
	int m_iOffsetX,m_iOffsetY;

public:
	// Constructors which manage memory
	CGraphicAlpha(int iWidth,int iHeight);
	CGraphicAlpha(CGraphicAlpha& src);
	CGraphicAlpha& operator=(CGraphicAlpha& src);
	~CGraphicAlpha();

	// Clip data access
	int ClipX() const { return m_iClipX; }
	int ClipY() const { return m_iClipY; }
	int ClipWidth() const { return m_iClipWidth; }
	int ClipHeight() const { return m_iClipHeight; }
	int OffsetX() const { return m_iOffsetX; }
	int OffsetY() const { return m_iOffsetY; }

	// Access to the data
	BYTE* Data() { return m_pbData; }
	int Width()	 { return m_iWidth; }
	int Height() { return m_iHeight; }

	// Clipping
	void SetClip(int iX,int iY,int iWidth,int iHeight);
	void SetOffset(int iOffsetX,int iOffsetY);

	// Copy into a CGraphic using transparency
	void Copy(CGraphic* pgrTarget)		// Copies entire graphic into 0,0
		{ Copy(pgrTarget,0,0); }
	void Copy(CGraphic* pgrTarget,		// Copies entire graphic to given
		int iDestinationX, int iDestinationY)
		{ Copy(pgrTarget,iDestinationX,iDestinationY,0,0,m_iWidth,m_iHeight); }
	void Copy(CGraphic* pgrTarget,		// Copies some of graphic
		int iDestinationX, int iDestinationY,
		int iSourceX,int iSourceY,int iSourceWidth,int iSourceHeight);
	void CopyAlphaInverted(CGraphic* pgrTarget)		// Copies entire graphic into 0,0
		{ CopyAlphaInverted(pgrTarget,0,0); }
	void CopyAlphaInverted(CGraphic* pgrTarget,		// Copies entire graphic to given
		int iDestinationX, int iDestinationY)
		{ CopyAlphaInverted(pgrTarget,iDestinationX,iDestinationY,0,0,m_iWidth,m_iHeight); }
	void CopyAlphaInverted(CGraphic* pgrTarget,		// Copies some of graphic
		int iDestinationX, int iDestinationY,
		int iSourceX,int iSourceY,int iSourceWidth,int iSourceHeight);
		
	// Load the data from disk or resource
	BOOL LoadFile(const char* pstrPath);
	BOOL LoadResource(const char* pstrResource);
	BOOL LoadCompressedResource(const char* pstrResource);

	// Resample
	void Resample(int iWidth,int iHeight);
	CGraphicAlpha* ResampleToNew(int iWidth,int iHeight); // Creates a new image

	// Rotate (switches x with y)
	void Rotate();

	// Flips
	void FlipHorizontal();
	void FlipVertical();
};

#endif // GRAPHICALPHA_H