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
#ifndef VEC_H
#define VEC_H

// This is a template class for dynamic arrays of
// different types of objects.

template <class O> class vec 
{
private:
	int m_iMemorySize;
	O** m_ppObj;

	int m_iSize;	// Could be less than memorysize
					// if RemoveAt is used.
public:
	// constructor sets up member
	vec()
		{ m_ppObj=NULL; m_iMemorySize=m_iSize=0; }

	vec(const vec& source)
	{ 
		m_ppObj=NULL; m_iMemorySize=m_iSize=0;
	    (*this)=source; 
	} 

	// destructor deletes members
	~vec()
	{
		for(int i=0;i<m_iSize;i++) 
			if(m_ppObj[i]) delete m_ppObj[i];
		if(m_ppObj) delete m_ppObj;
	}

	void Clear(const int iSize=-1)
	{
		int i;
		if(iSize==-1)
		{
			for(i=0;i<m_iSize;i++) 
				if(m_ppObj[i]) delete m_ppObj[i];
			if(m_ppObj) delete m_ppObj;

			m_ppObj=NULL; m_iMemorySize=m_iSize=0;
		}
		else
		{
			for(i=iSize;i<m_iSize;i++)
			{
				if(m_ppObj[i]) delete m_ppObj[i];
				m_ppObj[i]=NULL;
			}
			if(iSize<m_iSize) m_iSize=iSize;
		}
	}

	// Return size of array
	int Size() const
		{ return m_iSize; }

	// set function sets a particular element
	// to the given value. The latter one relies
	// on the object being deep-copyable (copy
	// constructor done properly)
	void Set(const int iPos,O* pNewObj)
	{
		if(iPos>=m_iMemorySize)
		{
			int iNewMemorySize = (m_iMemorySize * 4) / 3;
			if(iNewMemorySize < iPos+1) iNewMemorySize=iPos+1;

			m_ppObj=(O**)realloc(m_ppObj,
				(iNewMemorySize)*sizeof(O*));
			for(int i=m_iMemorySize;i<iNewMemorySize;i++)
				m_ppObj[i]=NULL;
			m_iMemorySize=iNewMemorySize;
		}
		if(iPos>=m_iSize) m_iSize=iPos+1;

		if(m_ppObj[iPos]) delete m_ppObj[iPos];
		m_ppObj[iPos]=pNewObj;
	}

	void Set(const int iPos,O newObj)
	{
		O* pNewObj=new O;
		*pNewObj=newObj;
		Set(iPos,pNewObj);
	}

	void Add(O* pNewObj)
		{ Set(m_iSize,pNewObj); }
	void Add(O newObj)
		{ Set(m_iSize,newObj); }

	void InsertAt(const int iPos,O* pNewObj)
	{
		ASSERT(iPos>=0);
		// Ensure array is large enough and
		// set item in okay.
		Set(m_iSize,pNewObj);

		// Shuffle array around (bear in mind m_iSize
		// just got incremented)
		O* pTemp=m_ppObj[m_iSize-1];
		MoveMemory(&m_ppObj[iPos+1],&m_ppObj[iPos],
			(m_iSize-iPos-1)*sizeof(O*));

		m_ppObj[iPos]=pTemp;
	}

	void InsertAt(const int iPos, const O newObj)
	{
		ASSERT(iPos>=0);
		// Ensure array is large enough and
		// set item in okay.
		Set(m_iSize,newObj);

		// Shuffle array around (bear in mind m_iSize
		// just got incremented)
		O* pTemp=m_ppObj[m_iSize-1];
		MoveMemory(&m_ppObj[iPos+1],&m_ppObj[iPos],
			(m_iSize-iPos-1)*sizeof(O*));

		m_ppObj[iPos]=pTemp;
	}

	void RemoveAt(const int iPos)
	{
		ASSERT(iPos<m_iSize && iPos>=0);
		ASSERT(m_ppObj[iPos]!=NULL);

		delete m_ppObj[iPos];
		MoveMemory(&m_ppObj[iPos],&m_ppObj[iPos+1],
			(m_iSize-iPos-1)*sizeof(O*));
		
		m_iSize--;
		m_ppObj[m_iSize]=NULL;
	}

	O& operator[](const int iPos) const
	{
		ASSERT(iPos<m_iSize && iPos>=0);
		ASSERT(m_ppObj[iPos]!=NULL);
		return *m_ppObj[iPos];
	}

	BOOL IsNonNull(const int iPos) const
	{
		return (m_ppObj[iPos]!=NULL);
	}

	int Find(const O &comparison) const
	{
		for(int i=0;i<m_iSize;i++)
			if( !((*m_ppObj[i])!=comparison)) return i;
		return -1;
	}

	void Remove(const O &comparison)
	{
		int iPos=Find(comparison);
		ASSERT(iPos!=-1);
		RemoveAt(iPos);
	}

	// Add the contents of an existing array
	// to the end of this one
	void AddVector(const vec<O> &src)
	{
		// Allocate pointers to appropriate space
		m_ppObj=(O**)realloc(m_ppObj,(m_iSize+src.m_iSize)
			*sizeof(O*));

		// Deep-copy structure
		for(int i=0;i<src.m_iSize;i++)
		{
			if(src.m_ppObj[i]==NULL)
				m_ppObj[i+m_iSize]=NULL;
			else
			{
				m_ppObj[i+m_iSize]=new O;
				*(m_ppObj[i+m_iSize])=*(src.m_ppObj[i]);
			}
		}

		// Update data size members
		m_iMemorySize=m_iSize+src.m_iSize;
		m_iSize=m_iMemorySize;
	}

	BOOL operator==(const vec<O> &src) const
	{
		for(int i=0;i<m_iMemorySize;i++) 
			if(m_ppObj[i]) 
		{
			// Check array overrun
			if(i>=src.m_iMemorySize) return FALSE;

			// Compare objects (objects must be
			// comparable, of course)
			if( (*(m_ppObj[i])) != (*(src.m_ppObj[i])) )
				return FALSE;
		}
		return TRUE;
	}

	BOOL operator!=(const vec<O> &src) const
	{ return ! ((*this) == src); }


	vec<O>& operator=(const vec<O>& src)
	{
		for(int i=0;i<m_iMemorySize;i++) 
			if(m_ppObj[i]) delete m_ppObj[i];
		if(m_ppObj) delete m_ppObj;

		// Full deep (i.e. slow ;) copy
		m_iMemorySize=src.m_iSize;
		m_iSize=src.m_iSize;

		if(m_iMemorySize>0)
		{
			m_ppObj=new O*[m_iMemorySize];
			ASSERT(m_ppObj);
		}
		else 
			m_ppObj=NULL;

		for(i=0;i<m_iSize;i++)
		{
			if(src.m_ppObj[i]==NULL)
				m_ppObj[i]=NULL;
			else
			{
				m_ppObj[i]=new O;
				ASSERT(m_ppObj[i]);
				*(m_ppObj[i])=*(src.m_ppObj[i]);
			}
		}
				
		return *this;
	}

};

typedef vec<CString> vecstring;

#endif // VEC_H