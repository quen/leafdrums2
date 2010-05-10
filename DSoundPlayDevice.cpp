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
#include "DSoundPlayDevice.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Buffer is 256k 
#define BUFFERSIZE 256*1024

CDSoundPlayDevice::CDSoundPlayDevice()
{
	m_pds=NULL;
	m_pdsb=NULL;
	m_dwBufferPos=0;
	m_dwPlayEndTime=0;

	CoInitialize(NULL);

	// Create DirectSound
	HRESULT hr;
	hr=CoCreateInstance(
		CLSID_DirectSound,NULL,CLSCTX_INPROC_SERVER,
		IID_IDirectSound,(LPVOID*)&m_pds);
	if(FAILED(hr))
	{
		CString strError=
			"Failed to obtain access to DirectSound. Please ensure that "
			"you have installed Microsoft Direct X on your PC; we "
			"recommend that you use the most recent version (8.0 at "
			"time of writing), available from http://www.microsoft.com/directx/";
		throw strError;
	}

	// Initialise
	if(FAILED(m_pds->Initialize(NULL)))
	{
		m_pds->Release(); m_pds=NULL;

		CString strError=
			"Failed to initialise DirectSound. There may be some problem with "
			"your system installation. (Please be sure you have DirectSound 8 "
			"(or better) correctly installed, and a sound driver that works.)";
		throw strError;
	}

	// Set priority
	if(FAILED(m_pds->SetCooperativeLevel(
		AfxGetMainWnd()->GetSafeHwnd(),DSSCL_PRIORITY)))
	{
		m_pds->Release(); m_pds=NULL;

		CString strError=
			"Failed to obtain priority access to DirectSound. Please check that "
			"you are not running any other applications which might demand full "
			"sound access (unlikely...)";
		throw strError;
	}

	// Sound format
	WAVEFORMATEX wfx;
	wfx.wFormatTag=WAVE_FORMAT_PCM;
	wfx.nChannels=2;
	wfx.nSamplesPerSec=44100;
	wfx.wBitsPerSample=16;
	wfx.nBlockAlign=(wfx.nChannels*wfx.wBitsPerSample)/8;
	wfx.nAvgBytesPerSec=wfx.nSamplesPerSec*wfx.nBlockAlign;
	wfx.cbSize=0;

	// Access primary buffer
	DSBUFFERDESC1 dsbdPrimary;
	dsbdPrimary.dwSize=sizeof(dsbdPrimary);
	dsbdPrimary.dwFlags=DSBCAPS_PRIMARYBUFFER;
	dsbdPrimary.dwBufferBytes=0;
	dsbdPrimary.dwReserved=0;
	dsbdPrimary.lpwfxFormat=NULL;

	IDirectSoundBuffer* pdsbPrimary;

	if(FAILED(m_pds->CreateSoundBuffer(
		(DSBUFFERDESC*)(&dsbdPrimary),&pdsbPrimary,NULL)))
	{
		m_pds->Release(); m_pds=NULL;

		CString strError=
			"Failed to access the DirectSound primary buffer. (Huh?!) "
			"Make sure you have DirectX 8 (or better) installed and "
			"you are not running any other software with advanced "
			"audio requirements.";
		throw strError;
	}

	// Set primary buffer format
	hr=pdsbPrimary->SetFormat(&wfx);
	if(FAILED(hr))
	{
		m_pdsb->Release(); m_pdsb=NULL;
		m_pds->Release(); m_pds=NULL;

		CString strError=
			"Failed to set the primary sound format to 16-bit 44.1 kHz stereo. "
			"Make sure your soundcard supports this format, you have current "
			"drivers, and DirectX 8 (or better) is installed.";
		throw strError;
	}

	// Release primary buffer
	pdsbPrimary->Release();

    // Create secondary buffer
	DSBUFFERDESC1 dsbd;
	dsbd.dwSize=sizeof(dsbd);
	dsbd.dwFlags=DSBCAPS_GETCURRENTPOSITION2|DSBCAPS_GLOBALFOCUS;
	dsbd.dwBufferBytes=BUFFERSIZE;
	dsbd.dwReserved=0;
	dsbd.lpwfxFormat=&wfx;

	if(FAILED(m_pds->CreateSoundBuffer(
		(DSBUFFERDESC*)(&dsbd),&m_pdsb,NULL)))
	{
		m_pds->Release(); m_pds=NULL;

		CString strError=
			"Failed to create a DirectSound buffer. (Huh?!) "
			"Make sure you have DirectX 8 (or better) installed.";
		throw strError;
	}

}

CDSoundPlayDevice::~CDSoundPlayDevice()
{
	// Free DirectSound interfaces
	if(m_pdsb)
	{
		m_pdsb->Release();
		m_pdsb=NULL;
	}
	if(m_pds)
	{
		m_pds->Release();
		m_pds=NULL;
	}
	CoUninitialize();
}

// Add a buffer to be output (convert
// to stereo if necessary) and start
// playback, if not already
void CDSoundPlayDevice::PlayBuffer(CBlock* pBlock)
{
	short
		*psData1,
		*psData2;
	DWORD dwBytes1,dwBytes2;

	// Lock buffer
	if(FAILED(m_pdsb->Lock(m_dwBufferPos,pBlock->Size()*4,(void**)&psData1,&dwBytes1,(void**)&psData2,&dwBytes2,
		0)))
	{
#ifdef _DEBUG
		CString strTrace;
		strTrace.Format("* DirectSound lock failure at buffer pos %i (%i bytes)",
			m_dwBufferPos,pBlock->Size()*4);
		TRACE(strTrace);
#endif // _DEBUG

		// We may have missed some data, and need to write later on instead. Get write cursor
		if(FAILED(m_pdsb->GetCurrentPosition(NULL,&m_dwBufferPos)))
		{
			CString strError=
				"Error obtaining position in DirectSound buffer.";
			throw strError;		
		}

#ifdef _DEBUG
		strTrace.Format("* Safe write position is buffer pos %i",
			m_dwBufferPos);
		TRACE(strTrace);
#endif // _DEBUG

		HRESULT hr=m_pdsb->Lock(m_dwBufferPos,pBlock->Size()*4,(void**)&psData1,&dwBytes1,(void**)&psData2,&dwBytes2,
			0);
		if(FAILED(hr))
		{
			TRACE("* Failed twice! HRESULT=%x",hr);
			CString strError=
				"Error locking DirectSound buffer.";
			throw strError;		
		}
		TRACE("* Succeeded, continuing");
	}

	// Write data
	if(pBlock->Stereo())
	{
		// Copy data block into our own storage
		CopyMemory(psData1,pBlock->Data(),dwBytes1);
		if(psData2!=NULL)
			CopyMemory(psData2,&((BYTE*)pBlock->Data())[dwBytes1],dwBytes2);
	}
	else // Mono...
	{
		// Unpack data block into stereo in our storage
		short* psData=psData1;
		DWORD dwPos=0;
		for(int i=0;i<pBlock->Size();i++)
		{
			psData[dwPos++]=pBlock->Data()[i];
			if(dwPos==dwBytes1 && psData==psData1)
			{
				psData=psData2;
				dwPos=0;
			}
			psData[dwPos++]=pBlock->Data()[i];
			if(dwPos==dwBytes1 && psData==psData1)
			{
				psData=psData2;
				dwPos=0;
			}
		}
	}

	// Unlock buffer
	if(FAILED(m_pdsb->Unlock(psData1,dwBytes1,psData2,dwBytes2)))
	{
		CString strError=
			"Error unlocking DirectSound buffer.";
		throw strError;		
	}

	// Update buffer position
	m_dwBufferPos+=pBlock->Size()*4;
	if(m_dwBufferPos>BUFFERSIZE) m_dwBufferPos-=BUFFERSIZE;

	// Delete the block
	delete pBlock;

	// Play buffer if it isn't already
	if(FAILED(m_pdsb->Play(0,0,DSBPLAY_LOOPING)))
	{
		CString strError=
			"Error playing DirectSound buffer.";
		throw strError;		
	}

	// Get current play position
	DWORD dwPlayCursor;
	if(FAILED(m_pdsb->GetCurrentPosition(&dwPlayCursor,NULL)))
	{
		CString strError=
			"Error obtaining position in DirectSound buffer.";
		throw strError;		
	}

	// Work out bytes left in buffer
	DWORD dwBytesLeft=
		(dwPlayCursor < m_dwBufferPos) ? 
		(m_dwBufferPos-dwPlayCursor) :
	    (BUFFERSIZE - (dwPlayCursor-m_dwBufferPos));

	// Estimate end time
	m_dwPlayEndTime=
		GetTickCount()+(dwBytesLeft*10)/(4*441);
}

// Return the time we're going to run out of sound at
DWORD CDSoundPlayDevice::GetPlayEndTime()
{
	return m_dwPlayEndTime;
}

// Return the length of sound (in samples) that can be
// added to the buffer now
int CDSoundPlayDevice::GetBufferRoom()
{
	DWORD dwNow=GetTickCount();
	if(dwNow>m_dwPlayEndTime) return (BUFFERSIZE-4096)/4;

	// Cheat by getting expected end time, 
	// subtracting from now, and returning the difference
	// from buffer size, using a safety factor of 4 KB...
	DWORD dwBytesUsed=m_dwPlayEndTime-dwNow;
	return (BUFFERSIZE-dwBytesUsed-4096)/4;
}

// Stop all sound playback immediately
// [May throw CString]
void CDSoundPlayDevice::StopNow()
{
	if(FAILED(m_pdsb->Stop()))
	{
		CString strError=
			"Error stopping DirectSound playback.";
		throw strError;		
	}

	m_dwBufferPos=0;

	if(FAILED(m_pdsb->SetCurrentPosition(0)))
	{
		CString strError=
			"Error resetting buffer position.";
		throw strError;		
	}
}

// Stop all sound playback after currently-added buffers
// are complete (will block until that's the case)
// [May throw CString]
void CDSoundPlayDevice::StopAfterBuffers()
{
	// Add 32 KB of blank space
	short
		*psData1,
		*psData2;
	DWORD dwBytes1,dwBytes2;

	// Lock buffer
	if(FAILED(m_pdsb->Lock(m_dwBufferPos,32*1024,(void**)&psData1,&dwBytes1,(void**)&psData2,&dwBytes2,
		0)))
	{
		CString strError=
			"Error locking DirectSound buffer.";
		throw strError;		
	}

	// Write data
	ZeroMemory(psData1,dwBytes1);
	if(psData2==NULL)
		ZeroMemory(psData2,dwBytes2);

	// Unlock buffer
	if(FAILED(m_pdsb->Unlock(psData1,dwBytes1,psData2,dwBytes2)))
	{
		CString strError=
			"Error unlocking DirectSound buffer.";
		throw strError;		
	}

	// Calculate range of blank space
	DWORD dwStart1,dwEnd1,dwStart2,dwEnd2;
	dwStart1=m_dwBufferPos;
	dwEnd1=m_dwBufferPos+32*1024;
	if(dwEnd1>BUFFERSIZE)
	{
		dwStart2=0;
		dwEnd2=dwEnd1-BUFFERSIZE;
		dwEnd1=BUFFERSIZE;
	}
	else
		dwStart2=BUFFERSIZE+1;

	// Okay, now wait until play reaches this range
	while(TRUE)
	{
		// Get current play position
		DWORD dwPlayCursor;
		if(FAILED(m_pdsb->GetCurrentPosition(&dwPlayCursor,NULL)))
		{
			CString strError=
				"Error obtaining position in DirectSound buffer.";
			throw strError;		
		}

		// See if it's in our area
		if( (dwPlayCursor>=dwStart1 && dwPlayCursor<dwEnd1) ||
			(dwPlayCursor>=dwStart2 && dwPlayCursor<dwEnd2))
		{
			break;
		}

		// Wait a bit more
		Sleep(50); // 50 ms = about 900 samples, or about 3 KB of buffer
	}

	// Actually halt playback
	StopNow();
}
