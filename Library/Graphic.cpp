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

#include "Graphic.h"
#include "CompressedResourceData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Constructors which manage memory
CGraphic::CGraphic(int iWidth,int iHeight)
{
	// Copy size data
	m_iWidth=iWidth; m_iHeight=iHeight;

	// Default offset and clip
	CGraphicOffset* pgo=new CGraphicOffset();
	pgo->m_iOffsetX=pgo->m_iOffsetY=0;
	m_vgoOffsetStack.Add(pgo);

	// Default clip
	CGraphicClip* pgc=new CGraphicClip();
	pgc->m_iClipX=pgc->m_iClipY=0;
	pgc->m_iClipWidth=m_iWidth;
	pgc->m_iClipHeight=m_iHeight;
	m_vgcClipStack.Add(pgc);

	// Calculate width in bytes (DWORD-rounded)
	m_iByteWidth=m_iWidth*3;
	if((m_iByteWidth%4) != 0)
		m_iByteWidth=(m_iByteWidth/4+1)*4;

	// Allocate memory
	m_pbData=new BYTE[m_iByteWidth*m_iHeight];
}

CGraphic::~CGraphic()
{
	// Free memory
	delete m_pbData;
}

// Copy into another CGraphic
void CGraphic::Copy(CGraphic* pgrTarget,int iDestinationX, int iDestinationY,
	int iSourceX,int iSourceY,int iSourceWidth,int iSourceHeight)
{
	// Add offset
	iDestinationX+=pgrTarget->OffsetX();
	iDestinationY+=pgrTarget->OffsetY();
	iSourceX+=OffsetX();
	iSourceY+=OffsetY();

	// Calculate necessary clipping
	if(iDestinationY<pgrTarget->ClipY())
	{
		iSourceY+=pgrTarget->ClipY()-iDestinationY;
		iSourceHeight-=pgrTarget->ClipY()-iDestinationY;
		iDestinationY=pgrTarget->ClipY();
	}
	if(iDestinationX<pgrTarget->ClipX())
	{
		iSourceX+=pgrTarget->ClipX()-iDestinationX;
		iSourceWidth-=pgrTarget->ClipX()-iDestinationX;
		iDestinationX=pgrTarget->ClipX();
	}
	if(iSourceWidth+iDestinationX > pgrTarget->ClipX()+pgrTarget->ClipWidth())
		iSourceWidth=pgrTarget->ClipX()+pgrTarget->ClipWidth()-iDestinationX;
	if(iSourceHeight+iDestinationY > pgrTarget->ClipY()+pgrTarget->ClipHeight())
		iSourceHeight=pgrTarget->ClipY()+pgrTarget->ClipHeight()-iDestinationY;

	if(iSourceWidth<=0 || iSourceHeight<=0) return;

	// Calculate starting memory position in target
	BYTE* pbTarget=pgrTarget->Data();
	int iTargetPitch=pgrTarget->ByteWidth();
	pbTarget=&pbTarget[iTargetPitch*iDestinationY + 3*iDestinationX];

	// Calculate starting memory position in source
	BYTE* pbSource=Data();
	int iSourcePitch=ByteWidth();
	pbSource=&pbSource[iSourcePitch*iSourceY + 3*iSourceX];

	// Copy data (doesn't clip)
	for(int iY=0;iY<iSourceHeight;iY++)
	{
		CopyMemory(pbTarget,pbSource,iSourceWidth*3);
		pbTarget=&pbTarget[iTargetPitch];
		pbSource=&pbSource[iSourcePitch];
	}
}
		
// Load the data from disk (.raw format)
BOOL CGraphic::LoadFile(const char* pstrPath)
{
	ASSERT(m_iWidth<=1024);
	// Attempt to open the file
	FILE* fp=fopen(pstrPath,"rb");
	if(fp==NULL) return FALSE;

	// Attempt to read the required amount of data
	for(int iY=0;iY<m_iHeight;iY++)
	{
		// Read a line of data
		BYTE bTemp[1024*3];
		int iRead=fread(bTemp,1,m_iWidth*3,fp);
		if(iRead!=m_iWidth*3)
		{
			fclose(fp);
			return FALSE;
		}

		// Reverse data from RGB to BGR format
		for(int iX=0;iX<m_iWidth;iX++)
		{
			m_pbData[iY*m_iByteWidth+iX*3 +0]=
				bTemp[iX*3 +2];
			m_pbData[iY*m_iByteWidth+iX*3 +1]=
				bTemp[iX*3 +1];
			m_pbData[iY*m_iByteWidth+iX*3 +2]=
				bTemp[iX*3 +0];
		}
	}

	// Close the file
	fclose(fp);

	// Successful
	return TRUE;
}


// Load the data from a resource (.raw format)
BOOL CGraphic::LoadResource(const char* pstrResource)
{
	try
	{
		// Attempt to load resource
		CResourceData res(pstrResource);

		// Check size is sufficient
		if(int(res.Size()) < m_iWidth * 3 * m_iHeight) return FALSE;

		// Copy data, flipping channel ordering [RGB -> BGR]
		BYTE* pbSource=res.Data();
		for(int iY=0;iY<m_iHeight;iY++)
			for(int iX=0;iX<m_iWidth;iX++)
		{
			m_pbData[iY*m_iByteWidth+iX*3]=
				pbSource[(iY*m_iWidth+iX)*3+2];
			m_pbData[iY*m_iByteWidth+iX*3 +1]=
				pbSource[(iY*m_iWidth+iX)*3+1];
			m_pbData[iY*m_iByteWidth+iX*3 +2]=
				pbSource[(iY*m_iWidth+iX)*3];
		}
	}
	catch(CException* e)
	{
		e->Delete();

		// If we couldn't load that, look for a compressed one
		return LoadCompressedResource(pstrResource);
	}

	return TRUE;
}

// Load the data from a resource (.raw or .bmp format)
BOOL CGraphic::LoadCompressedResource(const char* pstrResource)
{
	try
	{
		if(CResourceData::Exists(pstrResource,"RAW_C"))
		{
			// Attempt to load raw compressed resource
			CCompressedResourceData res(pstrResource,"RAW_C");

			// Check size is sufficient
			if(int(res.Size()) < m_iWidth * 3 * m_iHeight) return FALSE;

			// Copy data, flipping channel ordering [RGB -> BGR]
			BYTE* pbSource=res.Data();
			for(int iY=0;iY<m_iHeight;iY++)
				for(int iX=0;iX<m_iWidth;iX++)
			{
				m_pbData[iY*m_iByteWidth+iX*3]=
					pbSource[(iY*m_iWidth+iX)*3+2];
				m_pbData[iY*m_iByteWidth+iX*3 +1]=
					pbSource[(iY*m_iWidth+iX)*3+1];
				m_pbData[iY*m_iByteWidth+iX*3 +2]=
					pbSource[(iY*m_iWidth+iX)*3];
			}
		}
		else if(CResourceData::Exists(pstrResource,"BMP_C"))
		{
			// Attempt to load raw compressed resource
			CCompressedResourceData res(pstrResource,"BMP_C");

			BYTE* pbFile=res.Data();

			// Read headers
			BITMAPFILEHEADER* pbfh=(BITMAPFILEHEADER*)pbFile;
			BITMAPINFOHEADER* pbih=(BITMAPINFOHEADER*)(pbFile+sizeof(BITMAPFILEHEADER));

			// Check file information
			if(pbih->biBitCount!=24 || pbih->biCompression!=BI_RGB)
				return FALSE;

			// Obtain file width and byte width
			int iWidth=pbih->biWidth;
			int iWidthBytes= (((pbih->biWidth*3)&3)==0) ? pbih->biWidth*3 : ((((pbih->biWidth*3)/4)+1)*4);
			int iHeight=pbih->biHeight;
			if(iWidth!=m_iWidth || iHeight!=m_iHeight) return FALSE;

			// Seek to start of data
			BYTE* pbSource=&pbFile[pbfh->bfOffBits];

			// Copy into image
			BYTE* pbData;
			for(int iY=iHeight-1;iY>=0;iY--)
			{
				// Get address in target bitmap
				pbData=Data();
				pbData=&pbData[m_iByteWidth*iY];

				// Convert line
				for(int iX=0;iX<iWidth;iX++)
				{
					pbData[0]=pbSource[iX*3];
					pbData[1]=pbSource[iX*3+1];
					pbData[2]=pbSource[iX*3+2];
					pbData=&pbData[3];
				}

				pbSource=&pbSource[iWidthBytes];
			}
		}
	}
	catch(CException* e)
	{
		e->Delete();
		return FALSE;
	}

	return TRUE;
}

void CGraphic::Resample(int iWidth,int iHeight)
{
	// Calculate width of graphic
	int iWidthBytes=iWidth*3;
	if(iWidthBytes % 4 !=0) iWidthBytes=(iWidthBytes/4+1)*4;

	// Allocate new memory
	BYTE* pbTarget=new BYTE[iWidthBytes*iHeight];

	// Resample
	for(int iY=0;iY<iHeight;iY++)
		for(int iX=0;iX<iWidth;iX++)
	{
		// Obtain four corners of this point in source graphic
		float fXPos1=float(iX*m_iWidth)/float(iWidth);
		float fYPos1=float(iY*m_iHeight)/float(iHeight);
		float fXPos2=float((iX+1)*m_iWidth)/float(iWidth);
		float fYPos2=float((iY+1)*m_iHeight)/float(iHeight);

		// Accumulated colour values, and total count of pixels used
		float fB,fG,fR,fPixels;
		fB=fG=fR=fPixels=(float)0;
		for(int iSourceY=(int)fYPos1;iSourceY<fYPos2;iSourceY++)
			for(int iSourceX=(int)fXPos1;iSourceX<fXPos2;iSourceX++)
		{
			float fY=(float)iSourceY,fX=(float)iSourceX;

			// Work out the proportion of this pixel that's actually
			// within the required area
			float fProportion=(float)1;
			if(fY<fYPos1)
				fProportion*=(1-(fYPos1-fY));
			if(fX<fXPos1)
				fProportion*=(1-(fXPos1-fX));
			if(fXPos2-fX<1)
				fProportion*=(fXPos2-fX);
			if(fYPos2-fY<1)
				fProportion*=(fYPos2-fY);

			// If we've run off the edge, limit the coordinates
			int iRealY,iRealX;
			
			if(iSourceY<m_iHeight) iRealY=iSourceY;
			else iRealY=m_iHeight-1;

			if(iSourceX<m_iWidth) iRealX=iSourceX;
			else iRealX=m_iWidth-1;

			// Accumulate appropriate amount of this pixel's colour
			fB+=float(m_pbData[iRealY*m_iByteWidth+iRealX*3])*fProportion;
			fG+=float(m_pbData[iRealY*m_iByteWidth+iRealX*3+1])*fProportion;
			fR+=float(m_pbData[iRealY*m_iByteWidth+iRealX*3+2])*fProportion;

			// Add to total pixel count
			fPixels+=fProportion;
		}

		// Set target pixel
		pbTarget[iWidthBytes*iY+iX*3]=(BYTE)(fB/fPixels);
		pbTarget[iWidthBytes*iY+iX*3+1]=(BYTE)(fG/fPixels);
		pbTarget[iWidthBytes*iY+iX*3+2]=(BYTE)(fR/fPixels);
	}

	// Delete old data and set to new
	delete m_pbData;
	m_pbData=pbTarget;

	// Set data
	m_iWidth=iWidth;
	m_iHeight=iHeight;
	m_iByteWidth=iWidthBytes;
}

BOOL CGraphic::EntirelyClipped(int iTLX,int iTLY,int iBRX,int iBRY) const
{
	// Add offset
	iTLX+=OffsetX();
	iTLY+=OffsetY();
	iBRX+=OffsetX();
	iBRY+=OffsetY();

	return (
		iTLX>=ClipX()+ClipWidth() || 
		iTLY>=ClipY()+ClipHeight() || 
		iBRX<=ClipX() || 
		iBRY<=ClipY());
}

// Fill a rectangle with colour
void CGraphic::FillRect(int iX,int iY,int iWidth,int iHeight,COLORREF clr)
{
	// Offset
	iX+=OffsetX();
	iY+=OffsetY();

	// Clip
	if(iX<ClipX())
	{
		iWidth-=(ClipX()-iX);
		iX=ClipX();
	}
	if(iY<ClipY())
	{
		iHeight-=(ClipY()-iY);
		iY=ClipY();
	}
	if(iX+iWidth>ClipX()+ClipWidth())
	{
		iWidth=ClipX()+ClipWidth()-iX;
	}
	if(iY+iHeight>ClipY()+ClipHeight())
	{
		iHeight=ClipY()+ClipHeight()-iY;
	}
	if(iHeight<=0 || iWidth<=0) return;

	// First fill first line:
	BYTE bBG[3];
	bBG[0]=GetBValue(clr);
	bBG[1]=GetGValue(clr);
	bBG[2]=GetRValue(clr);
	BYTE* pbTarget=&m_pbData[iY*ByteWidth()+(iX*3)];
	for(int i=0;i<iWidth;i++)
	{
		CopyMemory(pbTarget,bBG,3);
		pbTarget=&pbTarget[3];
	}

	// Now copy that line to all others (this will be dword-aligned, so much nicer)
	BYTE* pbSource=&m_pbData[iY*ByteWidth()+(iX*3)];
	int iWidthBytes=ByteWidth();
	pbTarget=&pbSource[iWidthBytes];

	for(i=1;i<iHeight;i++)
	{
		CopyMemory(pbTarget,pbSource,iWidth*3);
		pbTarget=&pbTarget[iWidthBytes];
	}
}

// Fill a rectangle with colour
void CGraphic::FillRect(int iX,int iY,int iWidth,int iHeight,COLORREF clr,int iAlpha)
{
	// Offset
	iX+=OffsetX();
	iY+=OffsetY();

	// Clip
	if(iX<ClipX())
	{
		iWidth-=(ClipX()-iX);
		iX=ClipX();
	}
	if(iY<ClipY())
	{
		iHeight-=(ClipY()-iY);
		iY=ClipY();
	}
	if(iX+iWidth>ClipX()+ClipWidth())
	{
		iWidth=ClipX()+ClipWidth()-iX;
	}
	if(iY+iHeight>ClipY()+ClipHeight())
	{
		iHeight=ClipY()+ClipHeight()-iY;
	}
	if(iHeight<=0 || iWidth<=0) return;

	// Get bitmap source
	BYTE* pbTarget=&m_pbData[iY*ByteWidth()+(iX*3)];

	// Get premultiplied colour data
	int 
		iR=GetRValue(clr)*iAlpha,
		iG=GetGValue(clr)*iAlpha,
		iB=GetBValue(clr)*iAlpha;

	// Invert alpha
	int iInvAlpha=256-iAlpha;

	for(int iLine=0;iLine<iHeight;iLine++)
	{
		_asm
		{
			push esi
			push edi

			mov esi,iInvAlpha
			mov eax,0
			mov edi,pbTarget
			mov ecx,iWidth

		pixelLoop:
			mov ebx,[iB]
			mov al,[edi]
			mul esi
			add eax,ebx
			shr eax,8
			mov [edi],al

			mov ebx,[iG]
			mov al,[edi+1]
			mul esi
			add eax,ebx
			shr eax,8
			mov [edi+1],al

			mov ebx,[iR]
			mov al,[edi+2]
			mul esi
			add eax,ebx
			shr eax,8
			mov [edi+2],al

			add edi,3

			dec ecx
			jnz pixelLoop

			pop edi
			pop esi
		}
		pbTarget+=ByteWidth();
	}
}

void CGraphic::Invert()
{
	// I forgot what this function was supposed
	// to do exactly, so we ignore the iColour
	// parameter.
	for(int i=0;i<ByteWidth()*Height();i++)
		m_pbData[i]=255-m_pbData[i];
}

void CGraphic::Clip(int iX,int iY,int iWidth,int iHeight)
{
	// Apply offset
	iX+=OffsetX();
	iY+=OffsetY();

	// Intersect with current clip
	if(iX<ClipX()) 
	{
		iWidth-=ClipX()-iX;
		iX=ClipX();
	}
	if(iY<ClipY()) 
	{
		iHeight-=ClipY()-iY;
		iY=ClipY();
	}
	if(iX+iWidth > ClipX()+ClipWidth()) iWidth=ClipX()+ClipWidth()-iX;
	if(iY+iHeight > ClipY()+ClipHeight()) iHeight=ClipY()+ClipHeight()-iY;

	CGraphicClip* pgc=new CGraphicClip();
	pgc->m_iClipX=iX;
	pgc->m_iClipY=iY;
	pgc->m_iClipWidth=iWidth;
	pgc->m_iClipHeight=iHeight;
	m_vgcClipStack.Add(pgc);
}

void CGraphic::Offset(int iOffsetX,int iOffsetY)
{
	iOffsetX+=OffsetX();
	iOffsetY+=OffsetY();

	CGraphicOffset* pgo=new CGraphicOffset();
	pgo->m_iOffsetX=iOffsetX;
	pgo->m_iOffsetY=iOffsetY;
	m_vgoOffsetStack.Add(pgo);
}

void CGraphic::UnClip()
{
	m_vgcClipStack.RemoveAt(m_vgcClipStack.Size()-1);	
}

void CGraphic::UnOffset()
{
	m_vgoOffsetStack.RemoveAt(m_vgoOffsetStack.Size()-1);	
}

// Set a single point (with alpha)
void CGraphic::SetPixel(int iX,int iY,COLORREF clr,int iAlpha)
{
	// Offset
	iX+=OffsetX();
	iY+=OffsetY();

	// Clip
	if(iX<ClipX() || iY<ClipY() || 
		iX>=ClipX()+ClipWidth() || iY>=ClipY()+ClipHeight())
		return;

	// Get colour
	int iR=GetRValue(clr),iG=GetGValue(clr),iB=GetBValue(clr);

	// Do each colour
	int iSrcB=m_pbData[iY*ByteWidth()+iX*3];
	m_pbData[iY*ByteWidth()+iX*3]=
		(iB*iAlpha + iSrcB*(256-iAlpha))>>8;

	int iSrcG=m_pbData[iY*ByteWidth()+iX*3+1];
	m_pbData[iY*ByteWidth()+iX*3+1]=
		(iG*iAlpha + iSrcG*(256-iAlpha))>>8;

	int iSrcR=m_pbData[iY*ByteWidth()+iX*3+2];
	m_pbData[iY*ByteWidth()+iX*3+2]=
		(iR*iAlpha + iSrcR*(256-iAlpha))>>8;
}

COLORREF CGraphic::GetPixel(int iX,int iY)
{
	// Offset
	iX+=OffsetX();
	iY+=OffsetY();

	// Clip
	if(iX<0 || iY<0 || 
		iX>=Width() || iY>=Height())
		return RGB(0,0,0);

	// Do each colour
	int iSrcB=m_pbData[iY*ByteWidth()+iX*3];
	int iSrcG=m_pbData[iY*ByteWidth()+iX*3+1];
	int iSrcR=m_pbData[iY*ByteWidth()+iX*3+2];

	return RGB(iSrcR,iSrcG,iSrcB);
}


// Draw a line including alpha
void CGraphic::DrawLine(
	int iX1,int iY1,int iX2,int iY2,COLORREF clr,int iAlpha)
{
	// If it's a point, draw point
	if(iX1==iX2 && iY1==iY2)
	{
		SetPixel(iX1,iY1,clr,iAlpha);
		return;
	}

	// Offset
	iX1+=OffsetX();
	iY1+=OffsetY();
	iX2+=OffsetX();
	iY2+=OffsetY();


	// Decide on direction of line
	int iXDistance=iX2-iX1;
	if(iXDistance<0) iXDistance=-iXDistance;
	int iYDistance=iY2-iY1;
	if(iYDistance<0) iYDistance=-iYDistance;

	if(iXDistance>iYDistance) // Draw horizotnally
	{
		// Swap points if necessary
		if(iX1>iX2)
		{
			int iTemp;
			iTemp=iX1;
			iX1=iX2;
			iX2=iTemp;
			iTemp=iY1;
			iY1=iY2;
			iY2=iTemp;
		}

		// Work out gradient per point (*65536)
		int iGradient=((iY2-iY1)*65536)/iXDistance;
		int iYPos=65536*iY1;

		// Clip horizontal
		if(iX1<ClipX())
		{
			iYPos+=(ClipX()-iX1)*iGradient;
			iX1=ClipX();			
		}
		if(iX2>=ClipX()+ClipWidth())
		{
			iX2=ClipX()+ClipWidth()-1;
		}
		if(iX2<iX1) return;

		// Draw line
		for(int iX=iX1;iX<=iX2;iX++)
		{
			int iY=iYPos>>16;
			int iYRemainder=iYPos%65536;

			// Work out alpha for the two pixels
			int iAlpha0=((65536-iYRemainder)*iAlpha)>>16;
			int iAlpha1=iAlpha-iAlpha0;

			// Draw each one
			SetPixel(iX-OffsetX(),iY-OffsetY(),clr,iAlpha0);
			SetPixel(iX-OffsetX(),iY+1-OffsetY(),clr,iAlpha1);

			// Add on to Y position
			iYPos+=iGradient;
		}
	}
	else // Draw vertically
	{
		// Swap points if necessary
		if(iY1>iY2)
		{
			int iTemp;
			iTemp=iX1;
			iX1=iX2;
			iX2=iTemp;
			iTemp=iY1;
			iY1=iY2;
			iY2=iTemp;
		}

		// Work out gradient per point (*65536)
		int iGradient=((iX2-iX1)*65536)/iYDistance;
		int iXPos=65536*iX1;

		// Clip vertical
		if(iY1<ClipY())
		{
			iXPos+=(ClipY()-iY1)*iGradient;
			iY1=ClipY();			
		}
		if(iY2>=ClipY()+ClipHeight())
		{
			iY2=ClipY()+ClipHeight()-1;
		}
		if(iY2<iY1) return;

		// Draw line
		for(int iY=iY1;iY<=iY2;iY++)
		{
			int iX=iXPos>>16;
			int iXRemainder=iXPos%65536;

			// Work out alpha for the two pixels
			int iAlpha0=((65536-iXRemainder)*iAlpha)>>16;
			int iAlpha1=iAlpha-iAlpha0;

			// Draw each one
			SetPixel(iX-OffsetX(),iY-OffsetY(),clr,iAlpha0);
			SetPixel(iX+1-OffsetX(),iY-OffsetY(),clr,iAlpha1);

			// Add on to X position
			iXPos+=iGradient;
		}
	}

}
