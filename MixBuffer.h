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
#ifndef MIXBUFFER_H
#define MIXBUFFER_H

// Class that holds generic MixBuffer data - data and length

class CMixBuffer
{
private:
	long* m_plData;
	int m_iSamples;		// 2* this much data, because stereo

public:
	CMixBuffer();
	~CMixBuffer();

	long* Data() { return m_plData; }
	int Size() { return m_iSamples; }

	void Create(int iSize);
};

#endif // MIXBUFFER_H
