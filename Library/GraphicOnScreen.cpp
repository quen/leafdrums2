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

#include "GraphicOnscreen.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Constructor and destructor, handle memory allocation
CGraphicOnscreen::CGraphicOnscreen(int iWidth,int iHeight) : CGraphic(iWidth,iHeight)
{
	// See if we have an 8-bit display - assume if we can't get a DC that we don't
	// have one :)
	CDC dc;
	if(dc.CreateDC("DISPLAY",NULL,NULL,NULL) && dc.GetDeviceCaps(BITSPIXEL)==8)
	{
		// We do have one, calculate byte width
		m_i8BitByteWidth=Width();
		if((m_i8BitByteWidth%4) != 0)
			m_i8BitByteWidth=(m_i8BitByteWidth/4+1)*4;

		// Allocate RAM
		m_pb8BitData=new BYTE[m_i8BitByteWidth*Height()];

		// Mark changed
		m_f8BitUpToDate=FALSE;

		// Create bitmapinfo
		FillMemory(&m_BI8Bit,sizeof(m_BI8Bit),0);
		m_BI8Bit.bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
		m_BI8Bit.bmiHeader.biWidth=Width();
		m_BI8Bit.bmiHeader.biHeight=-Height();
		m_BI8Bit.bmiHeader.biPlanes=1;
		m_BI8Bit.bmiHeader.biBitCount=8;
		m_BI8Bit.bmiHeader.biCompression=BI_RGB;

		// Create 8-bit palette
		WORD* pwPalette=(WORD*)m_BI8Bit.bmiColors;
		for(int iColour=0;iColour<256;iColour++)
			pwPalette[iColour]=iColour;
	}
	else
		m_pb8BitData=NULL;

	// Create bitmapinfo for 24-bit version
	FillMemory(&m_BI24Bit,sizeof(m_BI24Bit),0);
	m_BI24Bit.bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
	m_BI24Bit.bmiHeader.biWidth=Width();
	m_BI24Bit.bmiHeader.biHeight=-Height();
	m_BI24Bit.bmiHeader.biPlanes=1;
	m_BI24Bit.bmiHeader.biBitCount=24;
	m_BI24Bit.bmiHeader.biCompression=BI_RGB;
}

CGraphicOnscreen::~CGraphicOnscreen()
{
	if(m_pb8BitData) delete m_pb8BitData;
}

// Mark changed
void CGraphicOnscreen::Changed()
{
	m_f8BitUpToDate=FALSE;
}

// Drawing function
void CGraphicOnscreen::Draw(CDC* pDC,int iDestX,int iDestY,	
	int iSourceX,int iSourceY,int iSourceWidth,int iSourceHeight)
{
	// If we are on an 8-bit display...
	if(m_pb8BitData)
	{
		if(!m_f8BitUpToDate)
		{
			// Consider whether to render the whole thing or just
			// part of it - if area less than half
			int i8bStartY,i8bStartX,i8bWidth,i8bHeight;

			if(iSourceWidth*iSourceHeight <
				Width()*Height()/2)
			{
				// Draw only part, do not mark up to date
				i8bStartY=iSourceY;
				i8bStartX=iSourceX;
				i8bWidth=iSourceWidth;
				i8bHeight=iSourceHeight;
			}
			else
			{
				// Draw the whole lot, mark up to date
				i8bStartY=0;
				i8bStartX=0;
				i8bWidth=Width();
				i8bHeight=Height();
				m_f8BitUpToDate=TRUE;
			}

			// Render to 8-bit in standard palette
			// to draw.
			// Palette: 4 shades red, 8 green, 8 blue
			// (low to high)
			BYTE* pbSourceLine,* pbDestLine;
			for(int iY=i8bStartY;iY<i8bStartY+i8bHeight;iY++)
			{
				pbSourceLine=&(Data()[iY*ByteWidth()]);
				pbDestLine=&m_pb8BitData[iY*m_i8BitByteWidth];
				for(int iX=i8bStartX;iX<i8bStartX+i8bWidth;iX++)
				{
					int iRed=pbSourceLine[iX*3+2];
					iRed+=32; if(iRed>255) iRed=255;
					int iGreen=pbSourceLine[iX*3+1];
					iGreen+=16; if(iGreen>255) iGreen=255;
					int iBlue=pbSourceLine[iX*3+0];
					iBlue+=16; if(iBlue>255) iBlue=255;

					pbDestLine[iX]=
						255-
						(((iRed)>>6) |				
						(((iGreen)>>5) << 2) |		
						(((iBlue)>>5) << 5));	
				}
			}
		}

		SetDIBitsToDevice(*pDC,
			iDestX,iDestY,iSourceWidth,iSourceHeight,
			iSourceX,iSourceY,iSourceY,iSourceHeight,
			&m_pb8BitData[m_i8BitByteWidth*iSourceY],&m_BI8Bit,DIB_PAL_COLORS);
	}
	else
		SetDIBitsToDevice(*pDC,
			iDestX,iDestY,iSourceWidth,iSourceHeight,
			iSourceX,iSourceY,iSourceY,iSourceHeight,
			&(Data()[ByteWidth()*iSourceY]),&m_BI24Bit,DIB_RGB_COLORS);
}
