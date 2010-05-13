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

#include "GraphicTextOption.h"

#include "PaletteDC.h"
#include "GraphicOnscreen.h"
#include "ResourceData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGraphicTextOption

// Add an item to the selection, check count
void CGraphicTextOption::AddItem(const char* strText)
{
	m_vstrText.Add(strText);
}

// Return the index of an item
int CGraphicTextOption::GetIndex(const char* strText)
{
	CString strTemp=strText;
	return m_vstrText.Find(strTemp);
}

// Delete an item
void CGraphicTextOption::DelItem(const int iIndex)
{
	ASSERT(iIndex>=0 && iIndex<m_vstrText.Size());

	m_vstrText.RemoveAt(iIndex);
}

// Clear all items
void CGraphicTextOption::Clear()
{
	m_vstrText.Clear();
}

