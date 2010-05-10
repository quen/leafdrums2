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
#ifndef AUDIOHANDLER_H
#define AUDIOHANDLER_H

#include "SectionHandler.h"

#define MAXEFFECTS 14
#include "FXCode\Effect.h"

class CAudioHandler : public CSectionHandler
{
private:
	CGraphic* m_pgBackground;
	CSound* m_psSound;

	CGraphicFont m_gfFont;

	int m_iNumEffects;
	CRuntimeClass* m_prcEffect[MAXEFFECTS];

	CString 
		m_strScrollParamName[NUMEFFECTS][5],
		m_strSpinParamName[NUMEFFECTS][3];
	int
		m_iScrollParamNumber[NUMEFFECTS][5],
		m_iSpinParamNumber[NUMEFFECTS][3];

	void UpdateItemVisibility();
	void Play(CSample& smp);

	void InitTrimFadeControls(int iExcept=0);
	void InitFitControls(int iExcept=0);
	void InitTunedControls();
	void InitEffectControls(int iEffect=-1);
	void FillEffectsSpinner(CGraphicTextOption* pgto);
	void NewEffectValue(int iEffect,int iParam,int iValue);

	void OpenPreset();
	void SavePreset();

public:
	virtual int GetWidth() { return 613; }
	virtual int GetHeight() { return 478+8; }
	virtual int GetSplitY() { return 479; }
	virtual int GetSplitX() { return 612; }

	CAudioHandler() : m_gfFont("Verdana",11,FALSE) {}
	virtual ~CAudioHandler();

	// Create all the controls for this section
	virtual void Create(CLeafDrumsView* pView,int iX,int iY);

	// Draw this section onto the specified graphic
	virtual void Draw(CGraphicOnscreen& goTarget);

	// Command messages
	virtual void OnHScroll(int iID,int iValue);
	virtual void OnSelchange(int iID,int iValue);
	virtual void OnButton(int iID);

	// From other places
	void SetSound(CSound* psSound);

	// Clear on new song
	virtual void NewSong() { SetSound(&GetSong()->GetSound(0)); }
};

#endif // AUDIOHANDLER_H
