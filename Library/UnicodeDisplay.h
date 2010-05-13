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
#ifndef UNICODEDISPLAY_H
#define UNICODEDISPLAY_H

#include "leafChatUnicodeExport.h"

// Manages the Unicode DLL functions
class CUnicodeDisplay
{
public:
	CUnicodeDisplay();
	~CUnicodeDisplay();

	// Functions exported by DLL (wrappers)
	int GetTextWidth(CFont* pFont,wchar_t* pwsText,int& iHeight);
	void DrawText(CBitmap& bitmap,CFont& font,int iWidth,int iHeight,wchar_t* pwsText,RECT* pRect);

private:
	// DLL handle
	HINSTANCE m_hDLL;

	// Function handles
	GetTextWidthProc m_gtwp;
    DrawTextProc m_dtp;
};

#endif // UNICODEDISPLAY_H