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

#include "TextLineReader.h"

CTextLineReader::CTextLineReader(BYTE* pbData,int iSize)
{
	m_pbData=pbData;
	m_iSize=iSize;
	m_iPos=0;
}

BOOL CTextLineReader::NextLine(CString& strLine)
{
	// Skip any CRLF stuff
	while(m_iPos<m_iSize && m_pbData[m_iPos]=='\n' || m_pbData[m_iPos]=='\r') m_iPos++;
	if(m_iPos>=m_iSize) return FALSE;

	int iStart=m_iPos;

	// Read until another CRLF
	while(m_iPos<m_iSize && m_pbData[m_iPos]!='\n' && m_pbData[m_iPos]!='\r') m_iPos++;

	// Copy that data into a string
	char* pbData=strLine.GetBuffer(m_iPos-iStart+1);
	for(int i=0;i<m_iPos-iStart;i++)
		pbData[i]=m_pbData[iStart+i];
	pbData[i]=0;
	strLine.ReleaseBuffer();

	return TRUE;
}