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
#include "ResourceData.h"
#include "CompressedResourceData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Constructors which manage memory
CGraphicAlpha::CGraphicAlpha(int iWidth,int iHeight)
{
	// Copy size data
	m_iWidth=iWidth; m_iHeight=iHeight;

	// Clear offset
	m_iOffsetX=m_iOffsetY=0;

	// Default clip
	m_iClipX=m_iClipY=0;
	m_iClipWidth=m_iWidth;
	m_iClipHeight=m_iHeight;

	// Allocate memory
	m_pbData=new BYTE[m_iWidth*4*m_iHeight];
}

CGraphicAlpha::CGraphicAlpha(CGraphicAlpha& src)
{
	m_pbData=NULL;

	*this=src;
}

CGraphicAlpha& CGraphicAlpha::operator=(CGraphicAlpha& src)
{
	if(m_pbData) delete m_pbData;

	// Copy basic stats
	m_iWidth=src.m_iWidth;
	m_iHeight=src.m_iHeight;
	m_pbData=new BYTE[m_iWidth*4*m_iHeight];
	CopyMemory(m_pbData,src.m_pbData,m_iWidth*4*m_iHeight);

	// Copy clip and offset
	m_iOffsetX=src.m_iOffsetX;
	m_iOffsetY=src.m_iOffsetY;
	m_iClipX=src.m_iClipX;
	m_iClipY=src.m_iClipY;
	m_iClipWidth=src.m_iClipWidth;
	m_iClipHeight=src.m_iClipHeight;

	return *this;
}

CGraphicAlpha::~CGraphicAlpha()
{
	// Free memory
	if(m_pbData!=NULL) delete m_pbData;
}

// Copy into a CGraphic
void CGraphicAlpha::Copy(CGraphic* pgrTarget,int iDestinationX, int iDestinationY,
	int iSourceX,int iSourceY,int iSourceWidth,int iSourceHeight)
{
	// Add offset
	iDestinationX+=pgrTarget->OffsetX();
	iDestinationY+=pgrTarget->OffsetY();
	iSourceX+=m_iOffsetX;
	iSourceY+=m_iOffsetY;

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
	int iSourcePitch=m_iWidth*4;
	pbSource=&pbSource[iSourcePitch*iSourceY + 4*iSourceX];

	// Copy data (doesn't clip)
	for(int iY=0;iY<iSourceHeight;iY++)
	{
		for(int iX=0;iX<iSourceWidth;iX++)
		{
			// Obtain alpha component
			int iAlpha=pbSource[iX*4];

			// Handle colour components
			for(int iC=0;iC<3;iC++)
				pbTarget[iX*3+iC]=
					(pbSource[iX*4+1+iC]*iAlpha
					+pbTarget[iX*3+iC]*(256-iAlpha))>>8;
		}

		// Go onto next line
		pbTarget=&pbTarget[iTargetPitch];
		pbSource=&pbSource[iSourcePitch];
	}
}
		
// Copy into a CGraphic using alpha channel backwards
void CGraphicAlpha::CopyAlphaInverted(CGraphic* pgrTarget,int iDestinationX, int iDestinationY,
	int iSourceX,int iSourceY,int iSourceWidth,int iSourceHeight)
{
	// Add offset
	iDestinationX+=pgrTarget->OffsetX();
	iDestinationY+=pgrTarget->OffsetY();
	iSourceX+=m_iOffsetX;
	iSourceY+=m_iOffsetY;

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
	int iSourcePitch=m_iWidth*4;
	pbSource=&pbSource[iSourcePitch*iSourceY + 4*iSourceX];

	// Copy data (doesn't clip)
	for(int iY=0;iY<iSourceHeight;iY++)
	{
		for(int iX=0;iX<iSourceWidth;iX++)
		{
			// Obtain alpha component
			int iAlpha=pbSource[iX*4];

			// Handle colour components
			for(int iC=0;iC<3;iC++)
				pbTarget[iX*3+iC]=
					(pbSource[iX*4+1+iC]*(256-iAlpha)
					+pbTarget[iX*3+iC]*iAlpha)>>8;
		}

		// Go onto next line
		pbTarget=&pbTarget[iTargetPitch];
		pbSource=&pbSource[iSourcePitch];
	}
}
		
// Load the data from disk (.raw format)
BOOL CGraphicAlpha::LoadFile(const char* pstrPath)
{
	ASSERT(m_iWidth<=1024);
	// Attempt to open the file
	FILE* fp=fopen(pstrPath,"rb");
	if(fp==NULL) return FALSE;

	// Attempt to read the required amount of data
	for(int iY=0;iY<m_iHeight;iY++)
	{
		// Read a line of data
		BYTE bTemp[1024*4];
		int iRead=fread(bTemp,1,m_iWidth*4,fp);
		if(iRead!=m_iWidth*4)
		{
			fclose(fp);
			return FALSE;
		}

		// Reverse data from RGBA to ABGR format
		for(int iX=0;iX<m_iWidth;iX++)
		{
			m_pbData[(iY*m_iWidth+iX)*4 +0]=
				255-bTemp[iX*4 +3];
			m_pbData[(iY*m_iWidth+iX)*4 +1]=
				bTemp[iX*4 +2];
			m_pbData[(iY*m_iWidth+iX)*4 +2]=
				bTemp[iX*4 +1];
			m_pbData[(iY*m_iWidth+iX)*4 +3]=
				bTemp[iX*4 +0];
		}
	}

	// Close the file
	fclose(fp);

	// Successful
	return TRUE;
}


// Load the data from a resource (.raw format)
BOOL CGraphicAlpha::LoadResource(const char* pstrResource)
{
	try
	{
		// Attempt to load resource
		CResourceData res(pstrResource);

		// Check size is sufficient
		if(int(res.Size()) < m_iWidth * 4 * m_iHeight) return FALSE;

		// Copy data, flipping channel ordering [RGBA -> ABGR]
		BYTE* pbSource=res.Data();
		for(int iY=0;iY<m_iHeight;iY++)
			for(int iX=0;iX<m_iWidth;iX++)
		{
			m_pbData[(iY*m_iWidth+iX)*4 +0]=
				255-pbSource[(iY*m_iWidth+iX)*4 +3];
			m_pbData[(iY*m_iWidth+iX)*4 +1]=
				pbSource[(iY*m_iWidth+iX)*4 +2];
			m_pbData[(iY*m_iWidth+iX)*4 +2]=
				pbSource[(iY*m_iWidth+iX)*4 +1];
			m_pbData[(iY*m_iWidth+iX)*4 +3]=
				pbSource[(iY*m_iWidth+iX)*4 +0];
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

// Load the data from a resource (.raw format or paired .bmp format)
BOOL CGraphicAlpha::LoadCompressedResource(const char* pstrResource)
{
	try
	{
		if(CResourceData::Exists(pstrResource,"RAW_C"))
		{
			// Attempt to load resource
			CCompressedResourceData res(pstrResource,"RAW_C");

			// Check size is sufficient
			if(int(res.Size()) < m_iWidth * 4 * m_iHeight) return FALSE;

			// Copy data, flipping channel ordering [RGBA -> ABGR]
			BYTE* pbSource=res.Data();
			for(int iY=0;iY<m_iHeight;iY++)
				for(int iX=0;iX<m_iWidth;iX++)
			{
				m_pbData[(iY*m_iWidth+iX)*4 +0]=
					255-pbSource[(iY*m_iWidth+iX)*4 +3];
				m_pbData[(iY*m_iWidth+iX)*4 +1]=
					pbSource[(iY*m_iWidth+iX)*4 +2];
				m_pbData[(iY*m_iWidth+iX)*4 +2]=
					pbSource[(iY*m_iWidth+iX)*4 +1];
				m_pbData[(iY*m_iWidth+iX)*4 +3]=
					pbSource[(iY*m_iWidth+iX)*4 +0];
			}
		}
		else if(
			CResourceData::Exists(pstrResource,"RGB_BMP_C") && 
			CResourceData::Exists(pstrResource,"A_BMP_C"))
		{
			// Attempt to load compressed 24-bit BMP resource
			CCompressedResourceData res(pstrResource,"RGB_BMP_C");

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
			for(int iY=iHeight-1;iY>=0;iY--)
			{
				// Get address in target bitmap
				BYTE* pbData=Data();
				pbData=&pbData[(iWidth*4)*iY];

				// Convert line
				for(int iX=0;iX<iWidth;iX++)
				{
					pbData[1]=pbSource[iX*3];
					pbData[2]=pbSource[iX*3+1];
					pbData[3]=pbSource[iX*3+2];
					pbData=&pbData[4];
				}

				pbSource=&pbSource[iWidthBytes];
			}

			// Attempt to load compressed 8-bit alpha channel BMP resource
			CCompressedResourceData crdAlpha(pstrResource,"A_BMP_C");

			pbFile=crdAlpha.Data();

			// Read headers
			pbfh=(BITMAPFILEHEADER*)pbFile;
			pbih=(BITMAPINFOHEADER*)(pbFile+sizeof(BITMAPFILEHEADER));

			// Check file information
			if(pbih->biBitCount!=8 || pbih->biCompression!=BI_RGB)
				return FALSE;

			// Obtain file width and byte width
			iWidth=pbih->biWidth;
			iWidthBytes=(((pbih->biWidth)&3)==0) ? pbih->biWidth : ((((pbih->biWidth)/4)+1)*4);
			iHeight=pbih->biHeight;
			if(iWidth!=m_iWidth || iHeight!=m_iHeight) return FALSE;

			// Seek to start of data
			pbSource=&pbFile[pbfh->bfOffBits];

			// Copy into image
			for(iY=iHeight-1;iY>=0;iY--)
			{
				// Get address in target bitmap
				BYTE* pbData=Data();
				pbData=&pbData[(iWidth*4)*iY];

				// Convert line
				for(int iX=0;iX<iWidth;iX++)
				{
					// Get alpha
					pbData[0]=255-pbSource[iX];

					// Unpremultiply (against white) the other channels
					if(pbData[0]!=0)
					{
						pbData[1]=255*(pbData[1] - (255-pbData[0]))/pbData[0];
						pbData[2]=255*(pbData[2] - (255-pbData[0]))/pbData[0];
						pbData[3]=255*(pbData[3] - (255-pbData[0]))/pbData[0];
					}

					pbData=&pbData[4];
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

// Rotate (switches x with y)
void CGraphicAlpha::Rotate()
{
	// Allocate new memory
	DWORD* pdwSource=(DWORD*)m_pbData;
	DWORD* pdwTarget=new DWORD[m_iWidth*m_iHeight];

	// Copy graphic, rotated
	for(int iY=0;iY<m_iHeight;iY++) for(int iX=0;iX<m_iWidth;iX++)
		pdwTarget[iX*m_iHeight+iY]=pdwSource[iY*m_iWidth+iX];

	// Switch buffers and delete old one
	m_pbData=(BYTE*) pdwTarget;
	delete pdwSource;

	// Switch width and height
	int iTemp=m_iWidth;
	m_iWidth=m_iHeight;
	m_iHeight=iTemp;
}

void CGraphicAlpha::FlipHorizontal()
{
	DWORD* pdwTarget=(DWORD*)m_pbData;

	for(int iY=0;iY<m_iHeight;iY++) 
		for(int iX=0;iX<m_iWidth/2;iX++)
	{
		DWORD dwTemp=pdwTarget[iY*m_iWidth+iX];
		pdwTarget[iY*m_iWidth+iX]=pdwTarget[iY*m_iWidth+(m_iWidth-1-iX)];
		pdwTarget[iY*m_iWidth+(m_iWidth-1-iX)]=dwTemp;
	}
}

void CGraphicAlpha::FlipVertical()
{
	DWORD* pdwTarget=(DWORD*)m_pbData;

	for(int iX=0;iX<m_iWidth;iX++) 
		for(int iY=0;iY<m_iHeight/2;iY++)
	{
		DWORD dwTemp=pdwTarget[iY*m_iWidth+iX];
		pdwTarget[iY*m_iWidth+iX]=pdwTarget[(m_iHeight-1-iY)*m_iWidth+iX];
		pdwTarget[(m_iHeight-1-iY)*m_iWidth+iX]=dwTemp;
	}
}

void CGraphicAlpha::Resample(int iWidth,int iHeight)
{
	// Use ResampleToNew...
	CGraphicAlpha* pgaTemp=ResampleToNew(iWidth,iHeight);

	// Delete old data and set to new
	delete m_pbData;
	m_pbData=pgaTemp->Data();

	// Delete created graphic
	pgaTemp->m_pbData=NULL;
    delete pgaTemp;

	// Set data
	m_iWidth=iWidth;
	m_iHeight=iHeight;
}

CGraphicAlpha* CGraphicAlpha::ResampleToNew(int iWidth,int iHeight)
{
	// Allocate new memory
	CGraphicAlpha* pgaTarget=new CGraphicAlpha(iWidth,iHeight);
	BYTE* pbTarget=pgaTarget->Data();

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
		float fA,fB,fG,fR,fPixels;
		fA=fB=fG=fR=fPixels=(float)0;
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
			fA+=float(m_pbData[(iRealY*m_iWidth+iRealX)*4+0])*fProportion;
			fB+=float(m_pbData[(iRealY*m_iWidth+iRealX)*4+1])*fProportion;
			fG+=float(m_pbData[(iRealY*m_iWidth+iRealX)*4+2])*fProportion;
			fR+=float(m_pbData[(iRealY*m_iWidth+iRealX)*4+3])*fProportion;

			// Add to total pixel count
			fPixels+=fProportion;
		}

		// Set target pixel
		pbTarget[(iWidth*iY+iX)*4+0]=(BYTE)(fA/fPixels);
		pbTarget[(iWidth*iY+iX)*4+1]=(BYTE)(fB/fPixels);
		pbTarget[(iWidth*iY+iX)*4+2]=(BYTE)(fG/fPixels);
		pbTarget[(iWidth*iY+iX)*4+3]=(BYTE)(fR/fPixels);
	}

	return pgaTarget;
}
