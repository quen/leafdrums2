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
#ifndef TEXTLINEREADER_H
#define TEXTLINEREADER_H

class CTextLineReader
{
private:
	BYTE* m_pbData;
	int m_iSize;
	int m_iPos;

public:
	CTextLineReader(BYTE* pbData,int iSize);

	BOOL NextLine(CString &strLine);
};

#endif // TEXTLINEREADER_H