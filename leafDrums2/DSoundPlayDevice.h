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
#ifndef DSOUNDPLAYDEVICE_H
#define DSOUNDPLAYDEVICE_H

// This class abstracts out the soundcard device
// so as to make it nice and easy to use :)

#define DIRECTSOUND_VERSION 0x600

#include <mmsystem.h>
#include <dsound.h>
#include "Block.h"

class CDSoundPlayDevice
{
private:
	IDirectSound* m_pds;
	IDirectSoundBuffer* m_pdsb;

	DWORD m_dwBufferPos;
	DWORD m_dwPlayEndTime;

public:
	// Creates. 
	// [May throw CString]
	CDSoundPlayDevice();

	~CDSoundPlayDevice();

	// Add a buffer to be output (convert
	// to stereo if necessary) and start
	// playback, if not already. 
	// [May throw CString]
	void PlayBuffer(CBlock* pBlock);

	// Return the time we're going to run out of sound at
	DWORD GetPlayEndTime();

	// Return the length of sound (in samples) that can be
	// added to the buffer now
	int GetBufferRoom();

	// Stop all sound playback immediately
	// [May throw CString]
	void StopNow();

	// Stop all sound playback after currently-added buffers
	// are complete (will block until that's the case)
	// [May throw CString]
	void StopAfterBuffers();
};

#endif // DSOUNDPLAYDEVICE_H
