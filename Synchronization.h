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
#ifndef SYNCHRONIZATION_H
#define SYNCHRONIZATION_H

#include <afxmt.h>

#define INTERNAL_CANSYNCH public: CRealSynchObject m_isoLock;

#define INTERNAL_DEFERREDSYNCH public: CDeferredSynchObject m_isoLock;
#define INTERNAL_DEFERREDSYNCHINIT(p) m_isoLock.DeferTo(&(p->m_isoLock));

#define INTERNAL_SYNCHRONIZE CInternalSynchLock islSynch(&m_isoLock);

class CInternalSynchObject
{
public:
	virtual void Lock()=0;
	virtual void Unlock()=0;
};

class CDeferredSynchObject : public CInternalSynchObject
{
private:
	CInternalSynchObject* m_pisoDefer;

public:
	CDeferredSynchObject() { m_pisoDefer=NULL; }
	void Lock()
	{
		ASSERT(m_pisoDefer!=NULL);
		m_pisoDefer->Lock();
	}
	void Unlock()
	{
		ASSERT(m_pisoDefer!=NULL);
		m_pisoDefer->Unlock();
	}
	void DeferTo(CInternalSynchObject* piso) { m_pisoDefer=piso; }
};

class CRealSynchObject : public CInternalSynchObject
{
private:
	// Actual lock we're using
	CCriticalSection m_cs;

	// Thread which owns lock
	DWORD m_dwLockedThread;

	// Count on lock
	int m_iLockCount;

public:
	CRealSynchObject()
	{
		m_dwLockedThread=0;
		m_iLockCount=0;
	}

	void Lock()
	{
		DWORD dwThread=GetCurrentThreadId();
		if(m_dwLockedThread==dwThread)
		{
			m_iLockCount++;
			return;
		}

		m_cs.Lock();
		m_iLockCount++;
		m_dwLockedThread=dwThread;
	}

	void Unlock()
	{
		DWORD dwThread=GetCurrentThreadId();
		ASSERT(m_dwLockedThread==dwThread);
		m_iLockCount--;
		if(m_iLockCount==0)
		{
			m_dwLockedThread=0;
			m_cs.Unlock();
		}
	}
};

class CInternalSynchLock
{
private:
	CInternalSynchObject* m_piso;
	BOOL m_bLock;

public:

	CInternalSynchLock(CInternalSynchObject* piso,BOOL bLock=TRUE)
	{
		m_piso=piso;
		m_bLock=FALSE;
		if(bLock)
			Lock();
	}

	void Lock()
	{
		ASSERT(!m_bLock);
		m_bLock=TRUE;
		m_piso->Lock();
	}

	void Unlock()
	{
		ASSERT(m_bLock);
		m_bLock=FALSE;
		m_piso->Unlock();
	}

	~CInternalSynchLock()
	{
		if(m_bLock)
			Unlock();
	}
};

#endif // SYNCHRONIZATION_H
