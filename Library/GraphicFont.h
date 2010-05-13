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
#ifndef GRAPHICFONT_H
#define GRAPHICFONT_H

#include "GraphicAlphaChannel.h"
#include "UnicodeDisplay.h"

class CGraphicFont
{
private:
	CFont m_Font;
	BOOL m_fAntiAliased;

	int m_iHeight,m_iBaseline;

	BOOL m_fCached;
	CGraphicAlphaChannel* m_pgacCachedLetter[256]; // Cached letter data
	int m_iCachedAdvance[256];	// Amount to advance x
	int m_iCachedStart[256]; // Starting point to draw bitmap (relative to requested x)
	int m_iCachedWidth[256]; // Actual width of bitmap

	// Draw using cache (return x advance)
	int DrawCached(CGraphic* pgrDest,int iXStart,int iYStart,
		CString strText,COLORREF colour,COLORREF clrBG=0xffffffff,BOOL fFlush=TRUE);
	int WidthCached(const char* strText);

	static int sm_iBufferXStart;
	static int sm_iBufferYStart;
	static CString sm_strBufferText;
	static COLORREF sm_clrBufferColour;
	static CGraphic* sm_pgrBufferDest;
	static CGraphicFont* sm_pgfThis;

	static CUnicodeDisplay* sm_pUnicodeDisplay;

	CUnicodeDisplay* GetUnicodeDisplay();

public:
	// Constructor selects font
	CGraphicFont(CString strFontName,int iPointSize,
		BOOL fAntiAliased=TRUE,BOOL fItalic=FALSE,BOOL fBold=FALSE,BOOL fUnderline=FALSE);
	CGraphicFont(LOGFONT lfFont,BOOL fAntiAliased=FALSE);
	~CGraphicFont();

	// Obtain pixel-height etc.
	int Height();

	// Obtains baseline offset
	int Baseline();

	// Draw using font into a bitmap (return x advance)
	int Draw(CGraphic* pgrDest,int iXStart,int iYStart,
		CString strText,COLORREF colour,COLORREF clrBG=0xffffffff,BOOL fFlush=TRUE);
	int Draw(CGraphic* pgrDest,int iXStart,int iYStart,
		wchar_t* pwsText,COLORREF colour,COLORREF clrBG=0xffffffff,BOOL fFlush=TRUE);
	static void Flush();

	// Get width of a string
	int Width(CString strText);
	int Width(wchar_t* pwsText);
	int Width(char cChar);
	int Width(wchar_t wcChar);

	// Cache the font into a series of bitmaps for faster drawing
	void Cache();
};

#endif // GRAPHICFONT_H