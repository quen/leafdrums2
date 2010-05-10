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
#include "Sample.h"
#include "WavFile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CSample::CSample()
{
	m_psData=NULL;
	m_iSamples=0;
}

CSample::~CSample()
{
	if(m_psData) delete m_psData;
}

void CSample::Load(CString strFilename)
{
	CWavFile::Return r;	
	if(strFilename.Right(4).CompareNoCase(".ogg")==0)
	{
		r=CWavFile::LoadVorbis(strFilename,m_iSamples,m_psData);
	}
	else
	{
		r=CWavFile::LoadACM(strFilename,m_iSamples,m_psData);
	}
	if(r!=CWavFile::OK) throw CWavFile::ErrorText(r);
}

void CSample::Create(int iSize)
{
	// Delete existing sample
	if(m_psData) delete m_psData;

	// Allocate new sample
	m_psData=new short[iSize*2];
	m_iSamples=iSize;
}

void CSample::Set(short* psData,int iSize)
{
	// Delete existing sample
	if(m_psData) delete m_psData;

	// Set this sample pointer
	m_psData=psData;
	m_iSamples=iSize;
}

void CSample::Detach()
{
	m_psData=NULL;
	m_iSamples=0;
}
