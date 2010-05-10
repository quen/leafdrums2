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
#ifndef SOUND_H
#define SOUND_H

// Class that includes the data relevant to either a drum or a loop

#include <afxmt.h>
#include "Sample.h"
#include "MixBuffer.h"
#include "FXcode/Effect.h"
#include "vec.h"

#define NUMEFFECTS 2

// Constant used for storing samples when you don't care about
// the tempo
#define CSound_TEMPOANY 0

class CSong;

class CSound : public CObject
{
	DECLARE_SERIAL(CSound)

private:
	// Sample: original, after resampling or fading, after FX
	CSample m_smpOrig;
	vec<CSample> m_vsmpMod,m_vsmpFX;
	vec<int> m_viSampleTempos;

	// Original filename and folder within the leafDrums filesystem
	CString m_strFilename;
	CString m_strFolder;

	// Fit settings
	BOOL m_bFit;
	int m_iFitLength,m_iFitTrimStart,m_iFitTrimLength;

	// Trim settings
	BOOL m_bTrimFade;
	int m_iTrimStart,m_iTrimLength,m_iFadeIn,m_iFadeOut;

	// Tune settings
	BOOL m_bTune;
	int m_iTune;

	// Reverse setting
	BOOL m_bReverse;

	// Effects
	CEffect* m_pEffect[NUMEFFECTS];

	// Generic controls (Level 0-256, Pan 0-256, Accent 0-512 [256 = 1*level])
	int m_iLevel,m_iAccent,m_iPan;

	// Mute
	BOOL m_bMute;

	// Functions
	void ApplyMods(int iTempo,BOOL fLockCS=TRUE);
	void ApplyFX(int iTempo,BOOL fLockCS=TRUE);

	// Clears the rendered samples for tempos etc
	void ClearRendered();

	CMutex m_cs;

	// Resample from part of source to all of target, using the specified
	// starting position and increment in the source sample, and wrap to
	// indicate whether it wraps at ends (only important in VERY minor case)
	static void Resample(CSample& sSource,CSample& sTarget,
		double dOrigPos,double dOrigPlus,BOOL bWrap);

	int GetDesiredTempo(int iTempo) const
	{
		return m_bFit ? iTempo : CSound_TEMPOANY;
	}

	// Get index of sample at desired tempo, creating it if needed. MUST be within lock
	int GetSampleIndex(int iTempo);

public:
	CSound();
	~CSound();

	// Clear to no sound
	void Clear();

	// Return true if ther'es a sound in there
	BOOL HasSample();

	// Retrieve input audio data (do not mess)
	CSample& GetOriginal() { return m_smpOrig; }

	// Retrive final output data (do not mess)
	CSample& GetFinal(int iTempo) 
	{ 
		CSingleLock sl(&m_cs,TRUE);
		return m_vsmpFX[GetSampleIndex(iTempo)]; 
	}

	// Mix the given portion of the sample into the given buffer, 
	// adjusting level as required
	void Play(CMixBuffer& mixTarget,int iTargetOffset,
		int iOffset,int iLength,
		int iLevelLStart,const int iLevelLIncrement, 
		int iLevelRStart,const int iLevelRIncrement,
		const BOOL fAccent,const int iTempo);

	// Returns length of sound's output version
	int GetFinalLength(int iTempo)
	{
		CSingleLock sl(&m_cs,TRUE);
		return m_vsmpFX[GetSampleIndex(iTempo)].Size();
	}

	int GetOriginalLength() const {	return m_smpOrig.Size(); }

	// Get basic details
	CString GetFile() const { return m_strFilename; }
	CString GetFolder() const { return m_strFolder; }
	CString GetName() const;

	// Set file [may throw char* or CString]
	void SetFile(CString strFilename,CString strFolder,BOOL bClearSettings=FALSE);

	// Data relating to fit options. All fit data is in quarter beats
	BOOL IsFit() const { return m_bFit; }
	int GetFitLength() const { return m_iFitLength; }
	int GetFitTrimStart() const { return m_iFitTrimStart; }
	int GetFitTrimLength() const { return m_iFitTrimLength; }
	void SetFit(int iFitLength,int iFitTrimStart,int iFitTrimLength);
	void InformTempos(const vec<int> viTempos);
	void ClearFit();

	// Data relating to tune options. Tune data is in 1/24 semitones (so octave= 24*12)
	BOOL IsTuned() const { return m_bTune; }
	int GetTuning() const { return m_iTune; }
	void SetTuned(int iTune);
	void ClearTuned();

	// Data relating to trim+fade options. (All in samples.)
	BOOL IsTrimFade() const { return m_bTrimFade; }
	int GetTrimStart() const { return m_iTrimStart; }
	int GetTrimLength() const { return m_iTrimLength; }
	int GetFadeIn() const { return m_iFadeIn; }
	int GetFadeOut() const { return m_iFadeOut; }
	void SetTrimFade(int iTrimStart,int iTrimLength,int iFadeIn,int iFadeOut);
	void ClearTrimFade();

	// Control for effects
	CEffect* Effect(int iEffect);
	void SetEffect(int iEffect,CRuntimeClass* pEffect,CEffect* pSource=NULL);
	void EffectChanged();

	// Control for generic data
	void SetLevel(int iLevel) {	m_iLevel=iLevel; }
	void SetAccent(int iAccent) { m_iAccent=iAccent; }
	void SetPan(int iPan) {	m_iPan=iPan; }
	void SetMute(BOOL bMute) { m_bMute=bMute; }
	void SetReverse(BOOL bReverse);
	int GetLevel() const { return m_iLevel; }
	int GetAccent() const { return m_iAccent; }
	int GetPan() const { return m_iPan; }
	BOOL GetMute() const  { return m_bMute; }
	BOOL GetReverse() const { return m_bReverse; }

	// Serialize to disk
	void Serialize(CArchive& archive,CSong& s,BOOL bSetFile=TRUE);
};

#include "Song.h"

#endif // SOUND_H
