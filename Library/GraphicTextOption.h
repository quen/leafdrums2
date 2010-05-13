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
#if !defined(AFX_GRAPHICTEXTOPTION_H__EBA3AF04_6B59_11D2_B7F2_9D51758AD573__INCLUDED_)
#define AFX_GRAPHICTEXTOPTION_H__EBA3AF04_6B59_11D2_B7F2_9D51758AD573__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "vec.h"

#include "Graphic.h"
#include "GraphicAlpha.h"
#include "GraphicFont.h"

#include "GraphicOption.h"

class CGraphicTextOption : public CGraphicOption
{
// Construction
public:
	CGraphicTextOption(const BOOL fAutoDelete=FALSE)
		: CGraphicOption(fAutoDelete) 
	{  }

// Attributes
public:

// Operations
public:
	// Add an item to the selection, check count
	void AddItem(const char* strText);
	int Count() { return m_vstrText.Size(); }

	// Return the index of an item
	int GetIndex(const char* strText);
	
	// Delete an item or all of them
	void DelItem(const int iIndex);
	void Clear();

	virtual int GetMin() { return 0; }
	virtual int GetMax() { return m_vstrText.Size()-1; }
	virtual CString GetText(int iIndex) { return m_vstrText[iIndex]; }

protected:

	// Strings
	vec<CString> m_vstrText;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRAPHICTEXTOPTION_H__EBA3AF04_6B59_11D2_B7F2_9D51758AD573__INCLUDED_)
