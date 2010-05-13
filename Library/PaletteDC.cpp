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

#include "PaletteDC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Global display device
CDisplayDevice* CDisplayDevice::sm_pDisplayDevice;

/* DisplayDevice
 *
 * Handles display device setup, which for now means 
 *  just palette init
 */

void CDisplayDevice::InitDC(CDC* pdc)
{
	pdc->SelectPalette(&m_pal,FALSE);
}

void CDisplayDevice::Realize(CWnd* pwnd)
{
	CClientDC dc(pwnd);
	dc.SelectPalette(&m_pal,FALSE);
	dc.RealizePalette();
}

CDisplayDevice::CDisplayDevice()
{
	// Load the global palette
	HGLOBAL hg=LoadResource(NULL,
		FindResource(AfxGetInstanceHandle(),"GlobalPalette",
		"CutePalette"));

	LOGPALETTE *lp=(LOGPALETTE *)LockResource(hg);
	m_pal.CreatePalette(lp);

	// Set global display device pointer
	sm_pDisplayDevice=this;
}

CPaletteDC::CPaletteDC(CWnd* pwnd) : CPaintDC(pwnd)
{
	CDisplayDevice::sm_pDisplayDevice->Realize(pwnd);
	CDisplayDevice::sm_pDisplayDevice->InitDC(this);
}

CPaletteClientDC::CPaletteClientDC(CWnd* pwnd) : CClientDC(pwnd)
{
	CDisplayDevice::sm_pDisplayDevice->InitDC(this);
}

