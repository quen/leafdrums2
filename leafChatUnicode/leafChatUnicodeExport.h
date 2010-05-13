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
#ifndef LEAFCHATUNICODEEXPORT_H
#define LEAFCHATUNICODEEXPORT_H

// Callback definition
typedef void* (*OwnerProc)(const void* pOwnerData,int iMessage,const void* pData);

// Exported functions
typedef HWND (*CreateEditProc)(HWND hwndParent,OwnerProc op,void* opData,UINT uAdditionalStyles);
typedef int (*GetTextLengthProc)(HWND hwndEdit);
typedef void (*GetTextProc)(HWND hwndEdit,wchar_t* pwcBuffer,int iBufferSize);
typedef void (*SetFontNameProc)(HWND hwndEdit,const char* pcFont);
typedef void (*SetEnabledProc)(HWND hwndEdit,BOOL bEnabled);
typedef void (*MoveProc)(HWND hwndEdit,RECT rLocation);
typedef int (*GetTextWidthProc)(CFont* pFont,wchar_t* pwsText,int& iHeight);
typedef void (*DrawTextProc)(CBitmap& bitmap,CFont& font,int iWidth,int iHeight,wchar_t* pwsText,RECT* pRect);
typedef BOOL (*PreTranslateMessageProc)(HWND hwndEdit,MSG* pMsg);

// Message definitions
#define IW_RETURN 1

#define IW_PAGEUP 10
#define IW_PAGEDOWN 11

#define IW_CTRLHOME 20
#define IW_CTRLEND 21

#define IW_CREATED 30

#define IW_COMPLETENICK 100

#define IW_GETFOREGROUND 110
#define IW_GETBACKGROUND 111

#endif // LEAFCHATUNICODEEXPORT_H