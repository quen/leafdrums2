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
#ifndef BLOCK_H
#define BLOCK_H

class CBlock
{
private:
	BOOL m_fStereo;
	int m_iSize;
	short m_sData[1];

public:
	static CBlock* Create(BOOL fStereo,int iSize);
	void operator delete(void* pData);

	BOOL Stereo() { return m_fStereo; }
	BOOL Size() { return m_iSize; }
	short* Data() { return m_sData; }
};

#endif // BLOCK_H
