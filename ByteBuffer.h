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
#ifndef BYTEBUFFER_H
#define BYTEBUFFER_H

/** Byte buffer that frees itself */
class CByteBuffer
{
private:
	BYTE* m_pbBuffer;
	int m_iSize;
	int m_iAllocated;

public:
	CByteBuffer(int iSize)
	{
		m_iAllocated=0;
		Create(iSize);
	}
	CByteBuffer()
	{
		m_iSize=0;
		m_iAllocated=0;
	}
	~CByteBuffer()
	{
		Free();
	}
	void Create(int iSize)
	{
		if(m_iAllocated!=0) delete m_pbBuffer;
		m_iSize=iSize;
	
		m_pbBuffer=new BYTE[m_iSize];
		m_iAllocated=m_iSize;
	}
	void Free()
	{
		if(m_iAllocated!=0)
			delete m_pbBuffer;
		m_iAllocated=0;
		m_iSize=0;
		m_pbBuffer=NULL;
	}
	void Resize(int iNewSize)
	{
		if(m_iSize==0) 
			Create(iNewSize);
		else
		{
			// If you shrink the buffer, we really shrink it
			if(iNewSize<m_iSize)
			{
				m_iAllocated=iNewSize;
				m_pbBuffer=(BYTE*)realloc(m_pbBuffer,m_iAllocated);
			}
			// If you make it bigger, we allocate in 64KB chunks
			else if(iNewSize > m_iAllocated)
			{
				// Pick next 64 KB chunk
				m_iAllocated = ((m_iAllocated+65536)>>16)<<16;
				m_pbBuffer=(BYTE*)realloc(m_pbBuffer,m_iAllocated);
			}
			// If neither of the above was true then it fits in 
			// already-allocated buffer so don't do anything
			m_iSize=iNewSize;
		}
	}

	operator BYTE* () { return m_pbBuffer; }
	operator char* () { return (char*)m_pbBuffer; }

	int Size() { return m_iSize; }

	void Transfer(CByteBuffer& bbTarget)
	{
		bbTarget.Free();
		bbTarget.m_iSize=m_iSize;
		bbTarget.m_iAllocated=m_iAllocated;
		bbTarget.m_pbBuffer=m_pbBuffer;
		m_iSize=0;
		m_iAllocated=0;
		m_pbBuffer=NULL;
	}

	BYTE* Detach()
	{
		if(m_iAllocated!=m_iSize)
		{
			m_pbBuffer=(BYTE*)realloc(m_pbBuffer,m_iSize);
		}
		BYTE* pbReturn=m_pbBuffer;
		m_pbBuffer=NULL;
		m_iSize=0;
		m_iAllocated=0;
		return pbReturn;
	}
};

#endif // BYTEBUFFER_H
