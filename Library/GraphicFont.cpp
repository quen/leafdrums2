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
#include "GraphicAlpha.h"
#include "GraphicFont.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Antialiasing info and balancing
#define FACTOR		3

#if FACTOR==2

#define ODDLINE1	64
#define ODDLINE2	64
#define EVENLINE1	64
#define EVENLINE2	63

#else

const c_iPixelAmount[3][3]=
{
	28,28,28,
	28,31,28,
	28,28,28
};

#endif

// Statics

int CGraphicFont::sm_iBufferXStart;
int CGraphicFont::sm_iBufferYStart;
CString CGraphicFont::sm_strBufferText;
COLORREF CGraphicFont::sm_clrBufferColour;
CGraphic* CGraphicFont::sm_pgrBufferDest;
CGraphicFont* CGraphicFont::sm_pgfThis;

CUnicodeDisplay* CGraphicFont::sm_pUnicodeDisplay;

// Obtain DLL for displaying Unicode
CUnicodeDisplay* CGraphicFont::GetUnicodeDisplay()
{
	if(sm_pUnicodeDisplay==NULL)
		sm_pUnicodeDisplay=new CUnicodeDisplay();

	return sm_pUnicodeDisplay;
}

// Constructor selects font
CGraphicFont::CGraphicFont(CString strFontName,int iPointSize,BOOL fAntiAliased,
	BOOL fItalic,BOOL fBold,BOOL fUnderline)
{
	// Not yet cached
	m_fCached=FALSE;

	// Set anti-aliased flag
	m_fAntiAliased=fAntiAliased;

	// Create font
	if(m_fAntiAliased) iPointSize*=FACTOR;
	VERIFY(m_Font.CreateFont(-iPointSize,0,0,0,
		fBold? FW_BOLD : FW_NORMAL,
		fItalic,
		fUnderline,
		0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,DEFAULT_PITCH|FF_DONTCARE,strFontName));

	// Obtain pixel height
	CDC dc;
	dc.CreateDC("DISPLAY",NULL,NULL,NULL);
	dc.SelectObject(m_Font);
	TEXTMETRIC tm;
	VERIFY(dc.GetTextMetrics(&tm));
	m_iHeight=tm.tmHeight+tm.tmExternalLeading;
	m_iBaseline=tm.tmAscent;
}

CGraphicFont::CGraphicFont(LOGFONT lfFont,BOOL fAntiAliased)
{
	// Not yet cached
	m_fCached=FALSE;

	// Set anti-aliased flag
	m_fAntiAliased=fAntiAliased;

	// Create font
	if(m_fAntiAliased) 
	{
		lfFont.lfHeight*=FACTOR;
		lfFont.lfWidth*=FACTOR;
	}

	VERIFY(m_Font.CreateFontIndirect(&lfFont));

	// Obtain pixel height
	CDC dc;
	dc.CreateDC("DISPLAY",NULL,NULL,NULL);
	dc.SelectObject(m_Font);
	TEXTMETRIC tm;
	VERIFY(dc.GetTextMetrics(&tm));
	m_iHeight=tm.tmHeight+tm.tmExternalLeading;
}

CGraphicFont::~CGraphicFont()
{
	if(m_fCached)
		for(int iC=32;iC<256;iC++) delete m_pgacCachedLetter[iC];
}

// Get width of a string
int CGraphicFont::Width(CString strText)
{
	if(strText=="") return 0;
	if(m_fCached) return WidthCached(strText);

	// Obtain width and height
	CDC dc;
	dc.CreateDC("DISPLAY",NULL,NULL,NULL);
	dc.SelectObject(m_Font);
	CRect rect(0,0,0,0);
	int iHeight=dc.DrawText(strText,rect,DT_CALCRECT|DT_LEFT|DT_SINGLELINE);
	return rect.right/ ( m_fAntiAliased ? FACTOR : 1);
}

// Get width of a character
int CGraphicFont::Width(char cLetter)
{
	return Width(CString(cLetter));
}

// Get width of a string
int CGraphicFont::Width(wchar_t* pwsText)
{
	if(pwsText[0]==0) return 0;

	int iHeight;
	return GetUnicodeDisplay()->GetTextWidth(&m_Font,pwsText,iHeight) /
		(m_fAntiAliased ? FACTOR : 1);
}

// Get width of a character
int CGraphicFont::Width(wchar_t wcLetter)
{
	wchar_t wsLetter[2];
	wsLetter[0]=wcLetter;
	wsLetter[1]=0;
	return Width(wsLetter);
}


// Draw using font into a bitmap
int CGraphicFont::Draw(CGraphic* pgrDest,int iXStart,int iYStart,
	CString strText,COLORREF colour,COLORREF clrBG,BOOL fFlush)
{
	if(strText=="") return 0;
	if(m_fCached)
	{
		// Don't use cache if this will be affected by MBCS processing
		if(MultiByteToWideChar(CP_ACP,0,strText,strText.GetLength(),NULL,0)==strText.GetLength())
			return DrawCached(pgrDest,iXStart,iYStart,strText,colour,clrBG,fFlush);
	}

	// Obtain width and height
	CDC dc;
	dc.CreateDC("DISPLAY",NULL,NULL,NULL);
	dc.SelectObject(m_Font);
	CRect rect(0,0,0,0);
	int iHeight=dc.DrawText(strText,rect,DT_CALCRECT|DT_LEFT|DT_SINGLELINE);
	int iWidth=rect.right;
/*
	{
		CString sTest="‚Ð‚ç‚ª‚È";
		CDC dc2;
		dc2.CreateDC("DISPLAY",NULL,NULL,NULL);
		dc2.SelectObject(m_Font);
		CRect rect(0,0,0,0);
		dc2.DrawText(sTest,rect,DT_CALCRECT|DT_LEFT|DT_SINGLELINE);
		dc2.DrawText(sTest,rect,DT_LEFT|DT_SINGLELINE);
	}
*/
	// Create a black-and-white bitmap of appropriate size
	CBitmap bitmap;
	bitmap.CreateBitmap(iWidth+16,iHeight,1,1,NULL);

	// Create memory DC using bitmap
	CDC dcMemory;
	dcMemory.CreateCompatibleDC(NULL);
	dcMemory.SelectObject(&bitmap);
	dcMemory.SelectObject(&m_Font);

	// Clear bitmap
	dcMemory.SelectObject((HBRUSH)GetStockObject(BLACK_BRUSH));
	dcMemory.SelectObject((HPEN)GetStockObject(NULL_PEN));
	dcMemory.Rectangle(0,0,iWidth+16+1,iHeight+1);

	// Draw using font into bitmap
	dcMemory.SetTextColor(RGB(255,255,255));
	dcMemory.SetBkMode(TRANSPARENT);
	rect.SetRect(8,0,iWidth+8,iHeight);
	dcMemory.DrawText(strText,rect,DT_LEFT|DT_SINGLELINE);

	// Create alpha-graphic of appropriate size
	int iAlphaWidth=(iWidth%16 == 0) ? (iWidth) : (iWidth/16+1)*16,
		iAlphaHeight=iHeight;
	if(m_fAntiAliased) 
	{
		iAlphaWidth/=FACTOR; iAlphaWidth++;
		iAlphaHeight/=FACTOR; iAlphaHeight++;
	}
	CGraphicAlpha ga(iAlphaWidth,iAlphaHeight);

	// Fill alpha-graphic with selected colour and 0 alpha
	DWORD* pwTarget=(DWORD*)(ga.Data());
	DWORD dwColour;
	if(colour==0xffffffff)
		dwColour=
			((clrBG & 0x00ff0000) >> 8) |
			((clrBG & 0x0000ff00) << 8) |
			((clrBG & 0x000000ff) << 24);
	else
		dwColour=
			((colour & 0x00ff0000) >> 8) |
			((colour & 0x0000ff00) << 8) |
			((colour & 0x000000ff) << 24);
	for(int iPos=0;iPos<iAlphaWidth*iAlphaHeight;iPos++)
		pwTarget[iPos]=dwColour;

	// Get bitmap bits
	BITMAP bitmapInfo;
	bitmap.GetObject(sizeof(BITMAP),&bitmapInfo);

	ASSERT(iWidth+16==bitmapInfo.bmWidth);
	ASSERT(iHeight==bitmapInfo.bmHeight);
	int iWidthBytes=bitmapInfo.bmWidthBytes;

	BYTE* pbBits=new BYTE[iWidthBytes*iHeight];
	bitmap.GetBitmapBits(iWidthBytes*iHeight,pbBits);

	// Use bitmap bits to extract information and output into
	// alpha-component of alpha-graphic
	BYTE* pbTarget=ga.Data();
	BYTE* pbSource=pbBits;
	for(int iY=0;iY<iHeight;iY++)
	{
		pbTarget=&(ga.Data())[iAlphaWidth*4*
			(iY / (m_fAntiAliased? FACTOR : 1))];
		pbSource=&pbBits[iWidthBytes*iY+1];	// Skip the blank 8 pixels

		if(!m_fAntiAliased)
			for(int iX=1;iX<iWidthBytes-1;iX++)
			{
				pbTarget[0]=(pbSource[0] &  0x80) ? 255 : 0;
				pbTarget[4]=(pbSource[0] &  0x40) ? 255 : 0;
				pbTarget[8]=(pbSource[0] &  0x20) ? 255 : 0;
				pbTarget[12]=(pbSource[0] & 0x10) ? 255 : 0;
				pbTarget[16]=(pbSource[0] & 0x08) ? 255 : 0;
				pbTarget[20]=(pbSource[0] & 0x04) ? 255 : 0;
				pbTarget[24]=(pbSource[0] & 0x02) ? 255 : 0;
				pbTarget[28]=(pbSource[0] & 0x01) ? 255 : 0;
				
				pbTarget=&pbTarget[32];
				pbSource=&pbSource[1];
			}
		else
			
#if FACTOR==2
		{
			BYTE bOne,bTwo;
			if(iY&1)	{ bOne=ODDLINE1; bTwo=ODDLINE2; }
			else { bOne=EVENLINE1; bTwo=EVENLINE2; }

			for(int iX=1;iX<iWidthBytes-1;iX++)
			{
				pbTarget[0]+=(pbSource[0]  & 0x80) ? bOne : 0;
				pbTarget[0]+=(pbSource[0]  & 0x40) ? bTwo : 0;
				pbTarget[4]+=(pbSource[0]  & 0x20) ? bOne : 0;
				pbTarget[4]+=(pbSource[0]  & 0x10) ? bTwo : 0;
				pbTarget[8]+=(pbSource[0]  & 0x08) ? bOne : 0;
				pbTarget[8]+=(pbSource[0]  & 0x04) ? bTwo : 0;
				pbTarget[12]+=(pbSource[0] & 0x02) ? bOne : 0;
				pbTarget[12]+=(pbSource[0] & 0x01) ? bTwo : 0;
				
				pbTarget=&pbTarget[16];
				pbSource=&pbSource[1];
			}
		}
#else
		{
			BYTE bOne,bTwo,bThree;
			bOne=c_iPixelAmount[iY % FACTOR][0];
			bTwo=c_iPixelAmount[iY % FACTOR][1];
			bThree=c_iPixelAmount[iY % FACTOR][2];

			for(int iX=1;iX<iWidthBytes-1;)
			{
				pbTarget[0]+=(pbSource[0]  & 0x80) ? bOne : 0;
				pbTarget[0]+=(pbSource[0]  & 0x40) ? bTwo : 0;
				pbTarget[0]+=(pbSource[0]  & 0x20) ? bThree : 0;
				pbTarget[4]+=(pbSource[0]  & 0x10) ? bOne : 0;
				pbTarget[4]+=(pbSource[0]  & 0x08) ? bTwo : 0;
				pbTarget[4]+=(pbSource[0]  & 0x04) ? bThree : 0;
				pbTarget[8]+=(pbSource[0] & 0x02) ? bOne : 0;
				pbTarget[8]+=(pbSource[0] & 0x01) ? bTwo : 0;

				iX++; if(iX>=iWidthBytes-1) break;
				pbSource=&pbSource[1];

				pbTarget[8]+=(pbSource[0]  & 0x80) ? bThree : 0;
				pbTarget[12]+=(pbSource[0]  & 0x40) ? bOne : 0;
				pbTarget[12]+=(pbSource[0]  & 0x20) ? bTwo : 0;
				pbTarget[12]+=(pbSource[0]  & 0x10) ? bThree : 0;
				pbTarget[16]+=(pbSource[0]  & 0x08) ? bOne : 0;
				pbTarget[16]+=(pbSource[0]  & 0x04) ? bTwo : 0;
				pbTarget[16]+=(pbSource[0] & 0x02) ? bThree : 0;
				pbTarget[20]+=(pbSource[0] & 0x01) ? bOne : 0;

				iX++; if(iX>=iWidthBytes-1) break;
				pbSource=&pbSource[1];
				
				pbTarget[20]+=(pbSource[0]  & 0x80) ? bTwo : 0;
				pbTarget[20]+=(pbSource[0]  & 0x40) ? bThree : 0;
				pbTarget[24]+=(pbSource[0]  & 0x20) ? bOne : 0;
				pbTarget[24]+=(pbSource[0]  & 0x10) ? bTwo : 0;
				pbTarget[24]+=(pbSource[0]  & 0x08) ? bThree : 0;
				pbTarget[28]+=(pbSource[0]  & 0x04) ? bOne : 0;
				pbTarget[28]+=(pbSource[0] & 0x02) ? bTwo : 0;
				pbTarget[28]+=(pbSource[0] & 0x01) ? bThree : 0;

				iX++; if(iX>=iWidthBytes-1) break;
				pbSource=&pbSource[1];

				pbTarget=&pbTarget[32];
			}
		}
#endif
	}

	// Delete copy of bitmap bits
	delete pbBits;

	// Do background if necessary
	if(clrBG!=0xffffffff && colour!=0xffffffff)
	{
		int iRed=GetRValue(clrBG),
			iGreen=GetGValue(clrBG),
			iBlue=GetBValue(clrBG);

		BYTE *pbTarget=(BYTE*)ga.Data();
		for(int i=0;i<ga.Width()*ga.Height();i++)
		{
			// Get alpha
			int iAlpha=pbTarget[0],iInvAlpha=255-iAlpha;

			// Fill using alpha
			pbTarget[0]=255;
			pbTarget[1]=(pbTarget[1]*iAlpha+iBlue*iInvAlpha)/255;
			pbTarget[2]=(pbTarget[2]*iAlpha+iGreen*iInvAlpha)/255;
			pbTarget[3]=(pbTarget[3]*iAlpha+iRed*iInvAlpha)/255;

			// Next spot
			pbTarget=&pbTarget[4];
		}

	}

	// Output alpha-graphic onto dest graphic
	if(colour==0xffffffff)
		ga.CopyAlphaInverted(pgrDest,iXStart,iYStart,0,0,
			iWidth / (m_fAntiAliased ? FACTOR : 1),
			iHeight / (m_fAntiAliased ? FACTOR : 1));
	else
		ga.Copy(pgrDest,iXStart,iYStart,0,0,
			iWidth / (m_fAntiAliased ? FACTOR : 1),
			iHeight / (m_fAntiAliased ? FACTOR : 1));

	return iWidth / (m_fAntiAliased ? FACTOR : 1);
}

// Draw using font into a bitmap
int CGraphicFont::Draw(CGraphic* pgrDest,int iXStart,int iYStart,
	wchar_t* pwsText,COLORREF colour,COLORREF clrBG,BOOL fFlush)
{
	if(pwsText[0]==0) return 0;

	// Obtain width and height
	int iHeight;
	int iWidth=GetUnicodeDisplay()->GetTextWidth(&m_Font,pwsText,iHeight);

	// Create a black-and-white bitmap of appropriate size
	CBitmap bitmap;
	bitmap.CreateBitmap(iWidth+16,iHeight,1,1,NULL);

	CRect rect;
	rect.SetRect(8,0,iWidth+8,iHeight);
	GetUnicodeDisplay()->DrawText(bitmap,m_Font,iWidth,iHeight,pwsText,rect);
/*
	// Create memory DC using bitmap
	CDC dcMemory;
	dcMemory.CreateCompatibleDC(NULL);
	dcMemory.SelectObject(&bitmap);
	dcMemory.SelectObject(&m_Font);

	// Clear bitmap
	dcMemory.SelectObject((HBRUSH)GetStockObject(BLACK_BRUSH));
	dcMemory.SelectObject((HPEN)GetStockObject(NULL_PEN));
	dcMemory.Rectangle(0,0,iWidth+16+1,iHeight+1);

	// Draw using font into bitmap
	dcMemory.SetTextColor(RGB(255,255,255));
	dcMemory.SetBkMode(TRANSPARENT);
*/
	// Create alpha-graphic of appropriate size
	int iAlphaWidth=(iWidth%16 == 0) ? (iWidth) : (iWidth/16+1)*16,
		iAlphaHeight=iHeight;
	if(m_fAntiAliased) 
	{
		iAlphaWidth/=FACTOR; iAlphaWidth++;
		iAlphaHeight/=FACTOR; iAlphaHeight++;
	}
	CGraphicAlpha ga(iAlphaWidth,iAlphaHeight);

	// Fill alpha-graphic with selected colour and 0 alpha
	DWORD* pwTarget=(DWORD*)(ga.Data());
	DWORD dwColour;
	if(colour==0xffffffff)
		dwColour=
			((clrBG & 0x00ff0000) >> 8) |
			((clrBG & 0x0000ff00) << 8) |
			((clrBG & 0x000000ff) << 24);
	else
		dwColour=
			((colour & 0x00ff0000) >> 8) |
			((colour & 0x0000ff00) << 8) |
			((colour & 0x000000ff) << 24);
	for(int iPos=0;iPos<iAlphaWidth*iAlphaHeight;iPos++)
		pwTarget[iPos]=dwColour;

	// Get bitmap bits
	BITMAP bitmapInfo;
	bitmap.GetObject(sizeof(BITMAP),&bitmapInfo);

	ASSERT(iWidth+16==bitmapInfo.bmWidth);
	ASSERT(iHeight==bitmapInfo.bmHeight);
	int iWidthBytes=bitmapInfo.bmWidthBytes;

	BYTE* pbBits=new BYTE[iWidthBytes*iHeight];
	bitmap.GetBitmapBits(iWidthBytes*iHeight,pbBits);

	// Use bitmap bits to extract information and output into
	// alpha-component of alpha-graphic
	BYTE* pbTarget=ga.Data();
	BYTE* pbSource=pbBits;
	for(int iY=0;iY<iHeight;iY++)
	{
		pbTarget=&(ga.Data())[iAlphaWidth*4*
			(iY / (m_fAntiAliased? FACTOR : 1))];
		pbSource=&pbBits[iWidthBytes*iY+1];	// Skip the blank 8 pixels

		if(!m_fAntiAliased)
			for(int iX=1;iX<iWidthBytes-1;iX++)
			{
				pbTarget[0]=(pbSource[0] &  0x80) ? 255 : 0;
				pbTarget[4]=(pbSource[0] &  0x40) ? 255 : 0;
				pbTarget[8]=(pbSource[0] &  0x20) ? 255 : 0;
				pbTarget[12]=(pbSource[0] & 0x10) ? 255 : 0;
				pbTarget[16]=(pbSource[0] & 0x08) ? 255 : 0;
				pbTarget[20]=(pbSource[0] & 0x04) ? 255 : 0;
				pbTarget[24]=(pbSource[0] & 0x02) ? 255 : 0;
				pbTarget[28]=(pbSource[0] & 0x01) ? 255 : 0;
				
				pbTarget=&pbTarget[32];
				pbSource=&pbSource[1];
			}
		else
			
#if FACTOR==2
		{
			BYTE bOne,bTwo;
			if(iY&1)	{ bOne=ODDLINE1; bTwo=ODDLINE2; }
			else { bOne=EVENLINE1; bTwo=EVENLINE2; }

			for(int iX=1;iX<iWidthBytes-1;iX++)
			{
				pbTarget[0]+=(pbSource[0]  & 0x80) ? bOne : 0;
				pbTarget[0]+=(pbSource[0]  & 0x40) ? bTwo : 0;
				pbTarget[4]+=(pbSource[0]  & 0x20) ? bOne : 0;
				pbTarget[4]+=(pbSource[0]  & 0x10) ? bTwo : 0;
				pbTarget[8]+=(pbSource[0]  & 0x08) ? bOne : 0;
				pbTarget[8]+=(pbSource[0]  & 0x04) ? bTwo : 0;
				pbTarget[12]+=(pbSource[0] & 0x02) ? bOne : 0;
				pbTarget[12]+=(pbSource[0] & 0x01) ? bTwo : 0;
				
				pbTarget=&pbTarget[16];
				pbSource=&pbSource[1];
			}
		}
#else
		{
			BYTE bOne,bTwo,bThree;
			bOne=c_iPixelAmount[iY % FACTOR][0];
			bTwo=c_iPixelAmount[iY % FACTOR][1];
			bThree=c_iPixelAmount[iY % FACTOR][2];

			for(int iX=1;iX<iWidthBytes-1;)
			{
				pbTarget[0]+=(pbSource[0]  & 0x80) ? bOne : 0;
				pbTarget[0]+=(pbSource[0]  & 0x40) ? bTwo : 0;
				pbTarget[0]+=(pbSource[0]  & 0x20) ? bThree : 0;
				pbTarget[4]+=(pbSource[0]  & 0x10) ? bOne : 0;
				pbTarget[4]+=(pbSource[0]  & 0x08) ? bTwo : 0;
				pbTarget[4]+=(pbSource[0]  & 0x04) ? bThree : 0;
				pbTarget[8]+=(pbSource[0] & 0x02) ? bOne : 0;
				pbTarget[8]+=(pbSource[0] & 0x01) ? bTwo : 0;

				iX++; if(iX>=iWidthBytes-1) break;
				pbSource=&pbSource[1];

				pbTarget[8]+=(pbSource[0]  & 0x80) ? bThree : 0;
				pbTarget[12]+=(pbSource[0]  & 0x40) ? bOne : 0;
				pbTarget[12]+=(pbSource[0]  & 0x20) ? bTwo : 0;
				pbTarget[12]+=(pbSource[0]  & 0x10) ? bThree : 0;
				pbTarget[16]+=(pbSource[0]  & 0x08) ? bOne : 0;
				pbTarget[16]+=(pbSource[0]  & 0x04) ? bTwo : 0;
				pbTarget[16]+=(pbSource[0] & 0x02) ? bThree : 0;
				pbTarget[20]+=(pbSource[0] & 0x01) ? bOne : 0;

				iX++; if(iX>=iWidthBytes-1) break;
				pbSource=&pbSource[1];
				
				pbTarget[20]+=(pbSource[0]  & 0x80) ? bTwo : 0;
				pbTarget[20]+=(pbSource[0]  & 0x40) ? bThree : 0;
				pbTarget[24]+=(pbSource[0]  & 0x20) ? bOne : 0;
				pbTarget[24]+=(pbSource[0]  & 0x10) ? bTwo : 0;
				pbTarget[24]+=(pbSource[0]  & 0x08) ? bThree : 0;
				pbTarget[28]+=(pbSource[0]  & 0x04) ? bOne : 0;
				pbTarget[28]+=(pbSource[0] & 0x02) ? bTwo : 0;
				pbTarget[28]+=(pbSource[0] & 0x01) ? bThree : 0;

				iX++; if(iX>=iWidthBytes-1) break;
				pbSource=&pbSource[1];

				pbTarget=&pbTarget[32];
			}
		}
#endif
	}

	// Delete copy of bitmap bits
	delete pbBits;

	// Do background if necessary
	if(clrBG!=0xffffffff && colour!=0xffffffff)
	{
		int iRed=GetRValue(clrBG),
			iGreen=GetGValue(clrBG),
			iBlue=GetBValue(clrBG);

		BYTE *pbTarget=(BYTE*)ga.Data();
		for(int i=0;i<ga.Width()*ga.Height();i++)
		{
			// Get alpha
			int iAlpha=pbTarget[0],iInvAlpha=255-iAlpha;

			// Fill using alpha
			pbTarget[0]=255;
			pbTarget[1]=(pbTarget[1]*iAlpha+iBlue*iInvAlpha)/255;
			pbTarget[2]=(pbTarget[2]*iAlpha+iGreen*iInvAlpha)/255;
			pbTarget[3]=(pbTarget[3]*iAlpha+iRed*iInvAlpha)/255;

			// Next spot
			pbTarget=&pbTarget[4];
		}

	}

	// Output alpha-graphic onto dest graphic
	if(colour==0xffffffff)
		ga.CopyAlphaInverted(pgrDest,iXStart,iYStart,0,0,
			iWidth / (m_fAntiAliased ? FACTOR : 1),
			iHeight / (m_fAntiAliased ? FACTOR : 1));
	else
		ga.Copy(pgrDest,iXStart,iYStart,0,0,
			iWidth / (m_fAntiAliased ? FACTOR : 1),
			iHeight / (m_fAntiAliased ? FACTOR : 1));

	return iWidth / (m_fAntiAliased ? FACTOR : 1);
}

int CGraphicFont::Height()
{ 
	if(m_fAntiAliased) return m_iHeight/FACTOR; 
	else return m_iHeight;
}

int CGraphicFont::Baseline()
{ 
	if(m_fAntiAliased) return m_iBaseline/FACTOR; 
	else return m_iBaseline;
}

// Render the 256 letters into memory so they can be drawn quickly in future
void CGraphicFont::Cache()
{
	// Don't do it on MBCS systems
/*	CPINFO cpi;
	GetCPInfo(CP_ACP,&cpi);
	if(cpi.MaxCharSize>1) return;
*/
	// Don't do it twice...
	if(m_fCached) 
	{
		ASSERT(FALSE);
		return;
	}
	m_fCached=TRUE;

	for(int iC=32;iC<256;iC++)
	{
		// Skip MBCS characters
		if(_istleadbyte(iC)) continue;

		char strText[2];
		strText[1]=0;
		strText[0]=iC;

		// Obtain width and height
		CDC dc;
		dc.CreateDC("DISPLAY",NULL,NULL,NULL);
		dc.SelectObject(m_Font);
		CRect rect(0,0,0,0);
		dc.DrawText(strText,rect,DT_CALCRECT|DT_LEFT|DT_SINGLELINE|DT_NOPREFIX);
		int iWidth=rect.right;

		// The reported width is how much it should be advanced
		m_iCachedAdvance[iC]=iWidth / (m_fAntiAliased? FACTOR : 1);

		// Create a black-and-white bitmap of appropriate size
		int iWidthExtra=iWidth*3;
		if((iWidthExtra&31)!=0) iWidthExtra=((iWidthExtra/32)+1)*32;
		CBitmap bitmap;
		bitmap.CreateBitmap(iWidthExtra,m_iHeight,1,1,NULL);

		// Create memory DC using bitmap
		CDC dcMemory;
		dcMemory.CreateCompatibleDC(NULL);
		dcMemory.SelectObject(&bitmap);
		dcMemory.SelectObject(&m_Font);

		// Clear bitmap
		dcMemory.SelectObject((HBRUSH)GetStockObject(BLACK_BRUSH));
		dcMemory.SelectObject((HPEN)GetStockObject(NULL_PEN));
		dcMemory.Rectangle(0,0,iWidthExtra+1,m_iHeight+1);

		// Draw using font into bitmap
		dcMemory.SetTextColor(RGB(255,255,255));
		dcMemory.SetBkMode(TRANSPARENT);
		rect.SetRect(iWidth,0,iWidth*2,m_iHeight);
		dcMemory.DrawText(strText,rect,DT_LEFT|DT_SINGLELINE|DT_NOCLIP|DT_NOPREFIX);

		// Create temporary alpha-channel of appropriate size
		int iAlphaWidth=(iWidthExtra%16 == 0) ? (iWidthExtra) : (iWidthExtra/16+1)*16,
			iAlphaHeight=m_iHeight;
		if(m_fAntiAliased) 
		{
			iAlphaWidth/=FACTOR; iAlphaWidth++;
			iAlphaHeight/=FACTOR; iAlphaHeight++;
		}
		CGraphicAlphaChannel gac(iAlphaWidth,iAlphaHeight);

		BYTE* pbTarget=gac.Data();

		// Clear channel
		for(int iPos=0;iPos<iAlphaWidth*iAlphaHeight;iPos++)
			pbTarget[iPos]=0;

		// Get bitmap bits
		BITMAP bitmapInfo;
		bitmap.GetObject(sizeof(BITMAP),&bitmapInfo);

		ASSERT(iWidthExtra==bitmapInfo.bmWidth);
		ASSERT(m_iHeight==bitmapInfo.bmHeight);
		int iWidthBytes=bitmapInfo.bmWidthBytes;
		ASSERT(iWidthBytes==iWidthExtra/8);

		BYTE* pbBits=new BYTE[iWidthBytes*m_iHeight];
		bitmap.GetBitmapBits(iWidthBytes*m_iHeight,pbBits);

		// Use bitmap bits to extract information and output into
		// alpha-channel
		BYTE* pbSource=pbBits;
		for(int iY=0;iY<m_iHeight;iY++)
		{
			pbTarget=&(gac.Data())[iAlphaWidth*
				(iY / (m_fAntiAliased? FACTOR : 1))];
			pbSource=&pbBits[iWidthBytes*iY];

			if(!m_fAntiAliased)
				for(int iX=1;iX<iWidthBytes;iX++)
				{
					pbTarget[0]=(pbSource[0] &  0x80) ? 255 : 0;
					pbTarget[1]=(pbSource[0] &  0x40) ? 255 : 0;
					pbTarget[2]=(pbSource[0] &  0x20) ? 255 : 0;
					pbTarget[3]=(pbSource[0] & 0x10) ? 255 : 0;
					pbTarget[4]=(pbSource[0] & 0x08) ? 255 : 0;
					pbTarget[5]=(pbSource[0] & 0x04) ? 255 : 0;
					pbTarget[6]=(pbSource[0] & 0x02) ? 255 : 0;
					pbTarget[7]=(pbSource[0] & 0x01) ? 255 : 0;
					
					pbTarget=&pbTarget[8];
					pbSource=&pbSource[1];
				}
			else
				
	#if FACTOR==2
			{
				BYTE bOne,bTwo;
				if(iY&1)	{ bOne=ODDLINE1; bTwo=ODDLINE2; }
				else { bOne=EVENLINE1; bTwo=EVENLINE2; }

				for(int iX=1;iX<iWidthBytes;iX++)
				{
					pbTarget[0]+=(pbSource[0]  & 0x80) ? bOne : 0;
					pbTarget[0]+=(pbSource[0]  & 0x40) ? bTwo : 0;
					pbTarget[1]+=(pbSource[0]  & 0x20) ? bOne : 0;
					pbTarget[1]+=(pbSource[0]  & 0x10) ? bTwo : 0;
					pbTarget[2]+=(pbSource[0]  & 0x08) ? bOne : 0;
					pbTarget[2]+=(pbSource[0]  & 0x04) ? bTwo : 0;
					pbTarget[3]+=(pbSource[0] & 0x02) ? bOne : 0;
					pbTarget[3]+=(pbSource[0] & 0x01) ? bTwo : 0;
					
					pbTarget=&pbTarget[4];
					pbSource=&pbSource[1];
				}
			}
	#else
			{
				BYTE bOne,bTwo,bThree;
				bOne=c_iPixelAmount[iY % FACTOR][0];
				bTwo=c_iPixelAmount[iY % FACTOR][1];
				bThree=c_iPixelAmount[iY % FACTOR][2];

				for(int iX=1;iX<iWidthBytes;)
				{
					pbTarget[0]+=(pbSource[0]  & 0x80) ? bOne : 0;
					pbTarget[0]+=(pbSource[0]  & 0x40) ? bTwo : 0;
					pbTarget[0]+=(pbSource[0]  & 0x20) ? bThree : 0;
					pbTarget[1]+=(pbSource[0]  & 0x10) ? bOne : 0;
					pbTarget[1]+=(pbSource[0]  & 0x08) ? bTwo : 0;
					pbTarget[1]+=(pbSource[0]  & 0x04) ? bThree : 0;
					pbTarget[2]+=(pbSource[0] & 0x02) ? bOne : 0;
					pbTarget[2]+=(pbSource[0] & 0x01) ? bTwo : 0;

					iX++; if(iX>=iWidthBytes) break;
					pbSource=&pbSource[1];

					pbTarget[2]+=(pbSource[0]  & 0x80) ? bThree : 0;
					pbTarget[3]+=(pbSource[0]  & 0x40) ? bOne : 0;
					pbTarget[3]+=(pbSource[0]  & 0x20) ? bTwo : 0;
					pbTarget[3]+=(pbSource[0]  & 0x10) ? bThree : 0;
					pbTarget[4]+=(pbSource[0]  & 0x08) ? bOne : 0;
					pbTarget[4]+=(pbSource[0]  & 0x04) ? bTwo : 0;
					pbTarget[4]+=(pbSource[0] & 0x02) ? bThree : 0;
					pbTarget[5]+=(pbSource[0] & 0x01) ? bOne : 0;

					iX++; if(iX>=iWidthBytes) break;
					pbSource=&pbSource[1];
					
					pbTarget[5]+=(pbSource[0]  & 0x80) ? bTwo : 0;
					pbTarget[5]+=(pbSource[0]  & 0x40) ? bThree : 0;
					pbTarget[6]+=(pbSource[0]  & 0x20) ? bOne : 0;
					pbTarget[6]+=(pbSource[0]  & 0x10) ? bTwo : 0;
					pbTarget[6]+=(pbSource[0]  & 0x08) ? bThree : 0;
					pbTarget[7]+=(pbSource[0]  & 0x04) ? bOne : 0;
					pbTarget[7]+=(pbSource[0] & 0x02) ? bTwo : 0;
					pbTarget[7]+=(pbSource[0] & 0x01) ? bThree : 0;

					iX++; if(iX>=iWidthBytes) break;
					pbSource=&pbSource[1];

					pbTarget=&pbTarget[8];
				}
			}
	#endif
		}

		// Delete copy of bitmap bits
		delete pbBits;

		// Look in alpha-channel to see where the data actually starts compared
		// to where it's supposed to, and where it ends
		pbTarget=gac.Data();
		int iStart=-1,iEnd=-1;
		for(int iX=0;iX<iAlphaWidth;iX++)
			for(int iY=0;iY<iAlphaHeight;iY++)
				if(pbTarget[iY*iAlphaWidth+iX]!=0)
		{
			if(iStart==-1) iStart=iX;
			iEnd=iX;
		}

		// Fixes glitch with spaces
		if(iStart==-1 && iEnd==-1)
		{
			iStart=iEnd=0;
			m_iCachedStart[iC]=0;
			m_iCachedWidth[iC]=0;
		}
		else
		{
			// Work out starting point relative to expected start
			m_iCachedStart[iC]=iStart-iWidth/(m_fAntiAliased ? FACTOR : 1);

			// Total width of data
			m_iCachedWidth[iC]=iEnd-iStart+1;
		}

		// Create the actual data storage for this one & copy in the data
		m_pgacCachedLetter[iC]=new CGraphicAlphaChannel(
			m_iCachedWidth[iC],m_iHeight/(m_fAntiAliased ? FACTOR : 1));
		gac.Copy(m_pgacCachedLetter[iC],0,0,iStart,0,m_iCachedWidth[iC],
			m_iHeight/(m_fAntiAliased ? FACTOR : 1));
	}
}

int CGraphicFont::DrawCached(CGraphic* pgrDest,int iXStart,int iYStart,
	CString strText,COLORREF colour,COLORREF clrBG,BOOL fFlush)
{
	int iEndX=WidthCached(strText);

	// Fill in background if necessary (do this first)
	if(clrBG!=0xffffffff)
		pgrDest->FillRect(iXStart,iYStart,iEndX,Height(),clrBG);

	// Draw previous text if there is some
	Flush();

	// Set this text into the buffer
	sm_iBufferXStart=iXStart;
	sm_iBufferYStart=iYStart;
	sm_strBufferText=strText;
	sm_clrBufferColour=colour;
	sm_pgrBufferDest=pgrDest;
	sm_pgfThis=this;

	// Flush it if necessary
	if(fFlush) Flush();

	return iEndX;
}

void CGraphicFont::Flush()
{
	if(sm_pgrBufferDest==NULL) return;

	int iX=sm_iBufferXStart;

	for(int iC=0;iC<sm_strBufferText.GetLength();iC++)
	{
		BYTE c=(BYTE)sm_strBufferText[iC];
		if(c>=32)
		{
			// Draw letter
			sm_pgfThis->m_pgacCachedLetter[c]->Draw(sm_pgrBufferDest,
				iX+sm_pgfThis->m_iCachedStart[c],sm_iBufferYStart,sm_clrBufferColour);
			
			// Advance cursor
			iX+=sm_pgfThis->m_iCachedAdvance[c];
		}
	}

	sm_pgrBufferDest=NULL;
}

int CGraphicFont::WidthCached(const char* strText)
{
	int iX=0;
	for(int iC=0;iC<lstrlen(strText);iC++)
	{
		if(((BYTE)strText[iC])>=32)
		{
			iX+=m_iCachedAdvance[(BYTE)(strText[iC])];
		}
	}

	return iX;
}
