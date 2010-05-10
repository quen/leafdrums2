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
#ifndef SAMPLE_H
#define SAMPLE_H

// Class that holds generic sample data - data and length

class CSample
{
private:
	short* m_psData;
	int m_iSamples;		// 2* this much data, because stereo

public:
	CSample();
	~CSample();

	short* Data() { return m_psData; }
	int Size() const { return m_iSamples; }

	// Load throws a CString error if there's a problem
	void Load(CString strFilename);
	void Create(int iSize);
	void Set(short* psData,int iSize);

	// Detach (set to null without deleting memory)
	void Detach();
};

#endif // SAMPLE_H
