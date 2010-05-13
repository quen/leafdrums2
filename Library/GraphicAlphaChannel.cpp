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

#include "GraphicAlphaChannel.h"
#include "ResourceData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Constructors which manage memory
CGraphicAlphaChannel::CGraphicAlphaChannel(int iWidth,int iHeight)
{
	// Copy size data
	m_iWidth=iWidth; m_iHeight=iHeight;

	// Allocate memory
	m_pbData=new BYTE[m_iWidth*m_iHeight];
}

CGraphicAlphaChannel::~CGraphicAlphaChannel()
{
	// Free memory
	if(m_pbData!=NULL) delete m_pbData;
}

// Draw into a CGraphic
void CGraphicAlphaChannel::Draw(CGraphic* pgrTarget,int iDestinationX, int iDestinationY,
	int iSourceX,int iSourceY,int iSourceWidth,int iSourceHeight,const COLORREF clr,
	const COLORREF clrBG)
{
	// Calculate necessary clipping
	if(iDestinationY<0)
	{
		iSourceY+=-iDestinationY;
		iSourceHeight-=-iDestinationY;
		iDestinationY=0;
	}
	if(iDestinationX<0)
	{
		iSourceX+=-iDestinationX;
		iSourceWidth-=-iDestinationX;
		iDestinationX=0;
	}
	if(iSourceWidth+iDestinationX > pgrTarget->Width())
		iSourceWidth=pgrTarget->Width()-iDestinationX;
	if(iSourceHeight+iDestinationY > pgrTarget->Height())
		iSourceHeight=pgrTarget->Height()-iDestinationY;

	// Calculate starting memory position in target
	BYTE* pbTarget=pgrTarget->Data();
	int iTargetPitch=pgrTarget->ByteWidth();
	pbTarget=&pbTarget[iTargetPitch*iDestinationY + 3*iDestinationX];

	// Calculate starting memory position in source
	BYTE* pbSource=Data();
	int iSourcePitch=m_iWidth;
	pbSource=&pbSource[iSourcePitch*iSourceY + iSourceX];

	if(clrBG==0xffffffff && clr!=0xffffffff)
	{
		// Work out colour
		int iR=GetRValue(clr),iG=GetGValue(clr),iB=GetBValue(clr);

		// Render data
		for(int iY=0;iY<iSourceHeight;iY++)
		{
			for(int iX=0;iX<iSourceWidth;iX++)
			{
				// Obtain alpha component
				int iAlpha=pbSource[iX];

				// Handle colour components
				pbTarget[iX*3+0]=(iB*iAlpha+pbTarget[iX*3+0]*(256-iAlpha))>>8;
				pbTarget[iX*3+1]=(iG*iAlpha+pbTarget[iX*3+1]*(256-iAlpha))>>8;
				pbTarget[iX*3+2]=(iR*iAlpha+pbTarget[iX*3+2]*(256-iAlpha))>>8;
			}

			// Go onto next line
			pbTarget=&pbTarget[iTargetPitch];
			pbSource=&pbSource[iSourcePitch];
		}
	}
	else if(clrBG!=0xffffffff && clr==0xffffffff)
	{
		// Work out colour
		int iR=GetRValue(clrBG),iG=GetGValue(clrBG),iB=GetBValue(clrBG);

		// Render data
		for(int iY=0;iY<iSourceHeight;iY++)
		{
			for(int iX=0;iX<iSourceWidth;iX++)
			{
				// Obtain alpha component
				int iAlpha=255-pbSource[iX];

				// Handle colour components
				pbTarget[iX*3+0]=(iB*iAlpha+pbTarget[iX*3+0]*(256-iAlpha))>>8;
				pbTarget[iX*3+1]=(iG*iAlpha+pbTarget[iX*3+1]*(256-iAlpha))>>8;
				pbTarget[iX*3+2]=(iR*iAlpha+pbTarget[iX*3+2]*(256-iAlpha))>>8;
			}

			// Go onto next line
			pbTarget=&pbTarget[iTargetPitch];
			pbSource=&pbSource[iSourcePitch];
		}
	}
	else // Draw both foreground and background
	{
		// Work out colour
		int iR=GetRValue(clr),iG=GetGValue(clr),iB=GetBValue(clr);
		int iRBG=GetRValue(clrBG),iGBG=GetGValue(clrBG),iBBG=GetBValue(clrBG);

		// Render data
		for(int iY=0;iY<iSourceHeight;iY++)
		{
			for(int iX=0;iX<iSourceWidth;iX++)
			{
				// Obtain alpha component
				int iAlpha=pbSource[iX];
				int iInverseAlpha=256-iAlpha;

				// Handle colour components
				pbTarget[iX*3+0]=(iB*iAlpha+iBBG*iInverseAlpha)>>8;
				pbTarget[iX*3+1]=(iG*iAlpha+iGBG*iInverseAlpha)>>8;
				pbTarget[iX*3+2]=(iR*iAlpha+iRBG*iInverseAlpha)>>8;
			}

			// Go onto next line
			pbTarget=&pbTarget[iTargetPitch];
			pbSource=&pbSource[iSourcePitch];
		}
	}
}
		
// Load the data from disk (.raw format)
BOOL CGraphicAlphaChannel::LoadFile(const char* pstrPath)
{
	ASSERT(m_iWidth<=1024);
	// Attempt to open the file
	FILE* fp=fopen(pstrPath,"rb");
	if(fp==NULL) return FALSE;

	// Attempt to read the required amount of data
	for(int iY=0;iY<m_iHeight;iY++)
	{
		// Read a line of data
		BYTE bTemp[1024];
		int iRead=fread(bTemp,1,m_iWidth,fp);
		if(iRead!=m_iWidth)
		{
			fclose(fp);
			return FALSE;
		}

		// Invert channel
		for(int iX=0;iX<m_iWidth;iX++)
		{
			m_pbData[(iY*m_iWidth+iX)+0]=
				255-bTemp[iX];
		}
	}

	// Close the file
	fclose(fp);

	// Successful
	return TRUE;
}


// Load the data from a resource (.raw format)
BOOL CGraphicAlphaChannel::LoadResource(const char* pstrResource)
{
	try
	{
		// Attempt to load resource
		CResourceData res(pstrResource);

		// Check size is sufficient
		if(int(res.Size()) < m_iWidth * m_iHeight) return FALSE;

		// Copy data, inverting
		BYTE* pbSource=res.Data();
		for(int iY=0;iY<m_iHeight;iY++)
			for(int iX=0;iX<m_iWidth;iX++)
		{
			m_pbData[(iY*m_iWidth+iX)]=
				255-pbSource[(iY*m_iWidth+iX)];
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
void CGraphicAlphaChannel::Rotate()
{
	// Allocate new memory
	BYTE* pbSource=(BYTE*)m_pbData;
	BYTE* pbTarget=new BYTE[m_iWidth*m_iHeight];

	// Copy graphic, rotated
	for(int iY=0;iY<m_iHeight;iY++) for(int iX=0;iX<m_iWidth;iX++)
		pbTarget[iX*m_iHeight+iY]=pbSource[iY*m_iWidth+iX];

	// Switch buffers and delete old one
	m_pbData=(BYTE*) pbTarget;
	delete pbSource;

	// Switch width and height
	int iTemp=m_iWidth;
	m_iWidth=m_iHeight;
	m_iHeight=iTemp;
}

void CGraphicAlphaChannel::FlipHorizontal()
{
	BYTE* pbTarget=(BYTE*)m_pbData;

	for(int iY=0;iY<m_iHeight;iY++) 
		for(int iX=0;iX<m_iWidth/2;iX++)
	{
		BYTE bTemp=pbTarget[iY*m_iWidth+iX];
		pbTarget[iY*m_iWidth+iX]=pbTarget[iY*m_iWidth+(m_iWidth-1-iX)];
		pbTarget[iY*m_iWidth+(m_iWidth-1-iX)]=bTemp;
	}
}

void CGraphicAlphaChannel::Resample(int iWidth,int iHeight)
{
	// Use ResampleToNew...
	CGraphicAlphaChannel* pgaTemp=ResampleToNew(iWidth,iHeight);

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

CGraphicAlphaChannel* CGraphicAlphaChannel::ResampleToNew(int iWidth,int iHeight)
{
	// Allocate new memory
	CGraphicAlphaChannel* pgaTarget=new CGraphicAlphaChannel(iWidth,iHeight);
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
			fA+=float(m_pbData[(iRealY*m_iWidth+iRealX)])*fProportion;

			// Add to total pixel count
			fPixels+=fProportion;
		}

		// Set target pixel
		pbTarget[(iWidth*iY+iX)]=(BYTE)(fA/fPixels);
	}

	return pgaTarget;
}

void CGraphicAlphaChannel::Copy(CGraphicAlphaChannel* pgacTarget,		
	int iDestinationX, int iDestinationY,
	int iSourceX,int iSourceY,int iSourceWidth,int iSourceHeight)
{
	// Calculate necessary clipping
	if(iDestinationY<0)
	{
		iSourceY+=-iDestinationY;
		iSourceHeight-=-iDestinationY;
		iDestinationY=0;
	}
	if(iDestinationX<0)
	{
		iSourceX+=-iDestinationX;
		iSourceWidth-=-iDestinationX;
		iDestinationX=0;
	}
	if(iSourceWidth+iDestinationX > pgacTarget->Width())
		iSourceWidth=pgacTarget->Width()-iDestinationX;
	if(iSourceHeight+iDestinationY > pgacTarget->Height())
		iSourceHeight=pgacTarget->Height()-iDestinationY;

	// Calculate starting memory position in target
	BYTE* pbTarget=pgacTarget->Data();
	int iTargetPitch=pgacTarget->Width();
	pbTarget=&pbTarget[iTargetPitch*iDestinationY + iDestinationX];

	// Calculate starting memory position in source
	BYTE* pbSource=Data();
	int iSourcePitch=m_iWidth;
	pbSource=&pbSource[iSourcePitch*iSourceY + iSourceX];

	// Copy the lines
	for(int iY=0;iY<iSourceHeight;iY++)
	{
		CopyMemory(pbTarget,pbSource,iSourceWidth);
		pbTarget=&pbTarget[iTargetPitch];
		pbSource=&pbSource[iSourcePitch];
	}
}
