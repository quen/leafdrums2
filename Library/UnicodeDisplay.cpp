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
#include "UnicodeDisplay.h"

CUnicodeDisplay::CUnicodeDisplay()
{
	// Load DLL
	m_hDLL=LoadLibrary("leafChatUnicode.dll");
	ASSERT(m_hDLL);

	m_dtp=(DrawTextProc)GetProcAddress(m_hDLL,"?DrawTextW@@YAXAAVCBitmap@@AAVCFont@@HHPAGPAUtagRECT@@@Z");
	ASSERT(m_dtp);
	m_gtwp=(GetTextWidthProc)GetProcAddress(m_hDLL,"?GetTextWidth@@YAHPAVCFont@@PAGAAH@Z");
	ASSERT(m_gtwp);
}

CUnicodeDisplay::~CUnicodeDisplay()
{
	FreeLibrary(m_hDLL);
}

int CUnicodeDisplay::GetTextWidth(CFont* pFont,wchar_t* pwsText,int& iHeight)
{
	return (*m_gtwp)(pFont,pwsText,iHeight);
}

void CUnicodeDisplay::DrawText(CBitmap& bitmap,CFont& font,int iWidth,int iHeight,wchar_t* pwsText,RECT* pRect)
{
	(*m_dtp)(bitmap,font,iWidth,iHeight,pwsText,pRect);
}
