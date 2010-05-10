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
#ifndef SONG_H
#define SONG_H

#include "vec.h"
#include "Synchronization.h"

#include "Sound.h"
#include "Track.h"
#include "Pattern.h"
#include "PlayLimits.h"

#define WM_PLAYPOSITIONCHANGED	WM_USER+2000
#define WM_PLAYSTOPPED			WM_USER+2001

#define CCoda_MAXMARKERS 3

#define CSong_WRITEWHOLE 0
#define CSong_WRITESOUNDS 1
#define CSong_WRITETRACKS 2

#define WRITEFORMAT_WAV 0 
#define WRITEFORMAT_OGGHIGH 1 // Quality 6 
#define WRITEFORMAT_OGGLOW 2  // Quality 2

class CCoda;
class CTimeChange;

// Represents location within a song
class SongLocation
{
public:
	int iIndex,iBar,iRepeat,iTempo,iTimeSignature;	
	SongLocation() { iIndex=0; iBar=0; iRepeat=0; }
	SongLocation(int iTargetBar) { iIndex=-1; iBar=iTargetBar; iRepeat=0; }
	BOOL Stop() { return iBar==-1; }
	BOOL operator==(const SongLocation& sl)
	{
		return sl.iIndex==iIndex && sl.iBar==iBar && sl.iRepeat==iRepeat;
	}
	BOOL operator!=(const SongLocation& sl) { return !(*this==sl); }
};

class CSong;

class CPatternBlock
{
public:
	CPatternBlock();
	void Set(CSong& s,int iFromBar,int iFromTrack,int iToBar,int iToTrack);
	void RemovePattern(CPattern* pp);
	void Paste(CSong& s,int iTargetBar,int iTargetTrack,BOOL bInsert);

	BOOL IsEmpty() { return m_iWidth==0 || m_iHeight==0; }
	int GetWidth() { return m_iWidth; }
	int GetHeight() { return m_iHeight; }

private:
	vec<CPattern*> m_vppData;
	int m_iWidth,m_iHeight;
};

// Class representing the entire song
class CSong : public CObject
{
private:
	// Sounds, tracks, and patterns
	vec<CSound> m_vsSounds;
	vec<CTrack> m_vtTracks;
	vec<CPattern> m_vpPatterns;

	// Coda markers
	vec<CCoda> m_vcCoda;

	// Current clipboard
	CPatternBlock m_pbClipboard;

	// Linearised song
	vec<SongLocation> m_vslLinear;

	// Tempo/timesignature changes
	vec<CTimeChange> m_vtcTime;

	// Level (0-256)
	int m_iLevel;

	// True if song loops
	BOOL m_bLoop;

	// True if currently playing
	BOOL m_bPlaying;

	// Maximum level reached in last playback segment
	int m_iCurrentMaxLevel;

	// Current playback position
	int m_iCurrentBar,m_iCurrentSixteenth;

	// Start/stop markers
	int m_iStartBar,m_iStopBar;

	// Current play limits
	CPlayLimits m_pl;

	// Map of sound rearrangement (if you dragged any lines)
	vec<int> m_viSoundMap;

	// Method CALLED FROM OTHER THREAD
	void SetCurrentPlayData(int iBar,int iSixteenth,int iMaxLevel);

	// Whether or not to embed all sounds
	BOOL m_bEmbedSounds;

	// Finds index for a location
	int FindLocationIndex(const SongLocation& sl);

	INTERNAL_CANSYNCH;

public:

	// Construct
	CSong();

	// Get basic info
	int GetLevel() const { return m_iLevel; }
	int GetBarCount();
	BOOL IsLoop() const { return m_bLoop; }
	void SetLoop(BOOL bLoop) { m_bLoop=bLoop; }

	// Get tempo at start of song 
	int GetStartTempo() const;
	int GetStartTimeSignature() const;

	// Returns time signature of bar, or -1 if there are multiple
	int GetUniqueTimeSignature(int iBar);

	// Set other data
	void SetLevel(int iLevel) { m_iLevel=iLevel; }

	// Calculated info
	static int GetBarLength(SongLocation sl) { return GetBarLength(sl.iTempo,sl.iTimeSignature); }
	static int GetBarLength(int iTempo,int iTimeSignature) { return (44100 * 60 * iTimeSignature) / iTempo; }

	// Get/set basic data
	int GetNumSounds() { return m_vsSounds.Size(); }
	CSound& GetSound(int iSound) const { return m_vsSounds[iSound]; }
	CSound& AddSound();
	void RemoveSound(CSound& s);
	int GetNumUsedSounds();

	int GetNumPatterns() { return m_vpPatterns.Size(); }
	CPattern& GetPattern(int iPattern) const { return m_vpPatterns[iPattern]; }
	CPattern& AddPattern();
	void RemovePattern(CPattern& p);

	int GetNumTracks() { return m_vtTracks.Size(); }
	CTrack& GetTrack(int iTrack) const { return m_vtTracks[iTrack]; }
	CTrack& AddTrack();
	void RemoveTrack(CTrack& t);
	int GetNumUsedTracks();

	CPatternBlock& GetClipboard() { return m_pbClipboard; }

	// Get last max play level (0-32768)
	int GetCurrentMaxLevel() { return m_iCurrentMaxLevel; }
	// Is playing?
	BOOL IsPlaying() { return m_bPlaying; }
	// Get play position
	int GetCurrentBar() { return m_iCurrentBar; }
	int GetCurrentSixteenth() { return m_iCurrentSixteenth; }

	// Plays a section of the given bar, beginning at sample offset and 
	// for length given, into the target sample, overwriting anything that's
	// already there. Returns max play level
	int Play(CSample& sTarget,int iTargetOffset,
		SongLocation sl,int iOffset,int iLength,CPlayLimits& pl);

	// Called by save dialog to enable embedding of all files (not just
	// those that aren't on disc)
	void SetEmbedSounds(BOOL bEmbedSounds) { m_bEmbedSounds=bEmbedSounds; }

	// Save to disc
	void Serialize(CArchive& archive);

	// Start playing from given bar up until (but not including) given bar
	void Start();

	// Stop playing
	void Stop();

	// Callback when playing stops
	void PlayingStopped();

	// Write the song to a file (optionally looped)
	void Write(const char* pstrFilename,int iCount,int iWriteStyle,int iFormat);

	// Returns length of song (in samples) that would be written
	int GetWriteSamples();

	// Global method that does actual playback code
	friend UINT TrackPlayThread(LPVOID lpvParam);

	void SetPlayRange(int iStartBar,int iStopBar)
	{
		m_iStartBar=iStartBar;
		m_iStopBar=iStopBar;
	}
	int GetStartBar() { return m_iStartBar; }
	int GetStopBar() { return m_iStopBar; }

	// Insert a bar at given location, shifting others along
	void InsertBar(int iBar);

	// Delete a bar at given location, shifting others along
	void DeleteBar(int iBar);

	// Return the next bar in the song
	SongLocation GetNextBar(SongLocation sl,BOOL bAllowLoop=TRUE);

	// Return the previous bar in the song
	SongLocation GetPreviousBar(SongLocation sl,BOOL bAllowLoop=TRUE);

	// Returns the location at which to begin playback
	SongLocation GetStartLocation();

	// Returns the location at which to end playback
	SongLocation GetEndLocation();

	// Returns the location after which to end playback (the last bar to play)
	SongLocation GetBeforeEndLocation();

	// Obtain coda information for a bar
	CCoda& GetCoda(int iBar);

	// Get time change information for a bar
	CTimeChange& GetTimeChange(int iBar);

	// Called by coda when a coda changes
	void CodaChanged();

	// Called by timechange when a time changes
	void TimeChanged();

	// Called by a track when length changes
	void LengthChanged();

	// Call to swap two sound lines (has no effect in song, UI only)
	void SwapSounds(int iSound1,int iSound2);

	// Return number of a line after rearrangement
	int GetSoundNumber(int iOriginalSound);

	// Get the slowest tempo used
	int GetSlowestTempo();
};

// Coda 
class CCoda
{
private:
	// Marker so we notice overwrite
	int iSpecial;

	// If a flag is present at beginning of this bar
	BOOL m_bFlag[CCoda_MAXMARKERS];

	// Non-zero if a repat marker is present at beginning of this bar
	int m_iRepeat[CCoda_MAXMARKERS];

	// Owner
	CSong* m_psOwner;

	// Initialise
	void Init();

public:
	// Initialise
	CCoda() { Init(); }
	CCoda(CSong* psOwner) { Init(); m_psOwner=psOwner; }

	// Copy & copy construct
	CCoda(const CCoda& src) { Init(); *this=src; }
	CCoda& operator=(const CCoda& src);

	// Get data
	BOOL HasFlag(int iMarker) const { return m_bFlag[iMarker]; }
	BOOL HasRepeat(int iMarker) const { return m_iRepeat[iMarker]!=0; }
	int GetRepeat(int iMarker) const { return m_iRepeat[iMarker]; }

	// Set data
	void SetFlag(int iMarker,BOOL bExists); 
	void SetRepeat(int iMarker,int iCount); 

	// Save to disc
	void Serialize(CArchive& archive);

	// Check if empty
	BOOL IsEmpty() const;
};

// Tempo changes
class CTimeChange
{
private:
	// Changes
	int m_iTempo,m_iTimeSignature;

	// Owner
	CSong* m_psOwner;

	// Initialise
	void Init();

public:
	// Initialise
	CTimeChange() { Init(); }
	CTimeChange(CSong* psOwner) { Init(); m_psOwner=psOwner; }

	BOOL IsEmpty() const { return m_iTempo==0; }

	// Copy & copy construct
	CTimeChange(const CTimeChange& src) { Init(); *this=src; }
	CTimeChange& operator=(const CTimeChange& src);

	// Get data
	int GetTempo() const { return m_iTempo; }
	int GetTimeSignature() const { return m_iTimeSignature; }

	// Set data
	void SetTempo(int iTempo); 
	void SetTimeSignature(int iTimeSignature); 
	void SetEmpty();

	// Save to disc
	void Serialize(CArchive& archive);
};


#endif // SONG_H
