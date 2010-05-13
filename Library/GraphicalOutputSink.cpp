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
#include "GraphicalProducer.h"
#include "GraphicalOutputSink.h"

CGraphicalOutputSink::CGraphicalOutputSink()
{
}

void CGraphicalOutputSink::ScreenInvalidate(CRect* pRect)
{
	InvalidateRect(pRect,FALSE);
}

BOOL CALLBACK GOSWindowEnumProc(HWND hwnd,LPARAM lpThis)
{
	CGraphicalProducer* pgp=dynamic_cast<CGraphicalProducer*>
		(CWnd::FromHandle(hwnd));

	if(pgp && pgp->IsWindowVisible())
	{
		CGraphic* pg=(CGraphic*)lpThis;
		pgp->PaintTo(pg);
	}

	return TRUE;
}

void CGraphicalOutputSink::PaintChildren(CGraphic* pg)
{
	if(!EnumChildWindows(GetSafeHwnd(),
		&GOSWindowEnumProc,(LPARAM)pg))
	{
		DWORD dwError=GetLastError();
	}
}
