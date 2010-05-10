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
#ifndef CONTROLHANDLER_H
#define CONTROLHANDLER_H

#include "SectionHandler.h"

class CControlHandler : public CSectionHandler
{
private:
	CGraphic* m_pgBackground;
	CGraphic* m_pgLevelIndicator;

	// Count of how long to show clip indicator for
	int m_iClipCount;

	void UpdateLevel();

public:
	virtual int GetWidth() { return CLIENTWIDTH; }
	virtual int GetHeight() { return 39; }
	virtual int GetSplitX() { return 350; }

	virtual ~CControlHandler();

	// Create all the controls for this section
	virtual void Create(CLeafDrumsView* pView,int iX,int iY);

	// Draw this section onto the specified graphic
	virtual void Draw(CGraphicOnscreen& goTarget);

	// Command messages
	virtual void OnHScroll(int iID,int iValue);
	virtual void OnButton(int iID);
	virtual void OnPlayPositionChanged();
	virtual void OnPlayStopped();

	// Section should reset its controls etc.
	virtual void NewSong();
};

#include "LeafDrumsView.h"

#endif // CONTROLHANDLER_H
