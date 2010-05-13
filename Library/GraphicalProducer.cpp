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

/*
void CGraphicalProducer::DrawCopy(CGraphicOnscreen* pgo,CDC* pdc,int iX,int iY)
{
	CGraphicalOutputSink* pgos=dynamic_cast<CGraphicalOutputSink*>(GetParent());
	if(!pgos)
		pgo->Draw(pdc,iX,iY);
	else
	{
		// Get this window's co-ordinates relative to parent
		CRect rectWindow;
		GetWindowRect(&rectWindow);
		pgos->ScreenToClient(&rectWindow);

		// Copy the graphic to the parent's screen
		pgo->Copy(pgos->Screen(),rectWindow.left,rectWindow.top);

		// Invalidate that bit of the parent
		pgos->ScreenInvalidate(&rectWindow);
		m_fInPaint=TRUE;
	}
}
*/

BOOL CGraphicalProducer::HaveOutputSink()
{
	CGraphicalOutputSink* pgos=dynamic_cast<CGraphicalOutputSink*>(GetParent());
	return (pgos!=NULL);
}

void CGraphicalProducer::GraphicRepaint()
{
	CGraphicalOutputSink* pgos=dynamic_cast<CGraphicalOutputSink*>(GetParent());
	if(pgos)
	{
		// Ask parent to repaint this
		CRect rectThis;
		GetWindowRect(&rectThis);
		pgos->ScreenToClient(&rectThis);
		pgos->InvalidateRect(&rectThis,FALSE);
	}
	else
		Invalidate();
}

void CGraphicalProducer::PaintTo(CGraphic* pgTarget)
{
	CGraphicalOutputSink* pgos=dynamic_cast<CGraphicalOutputSink*>(GetParent());
	ASSERT(pgos);

	CRect rectThis;
	GetWindowRect(&rectThis);
	pgos->ScreenToClient(&rectThis);

	PaintTo(pgTarget,rectThis.left,rectThis.top);
}
