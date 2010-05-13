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
#ifndef GRAPHICNUMBEROPTION_H
#define GRAPHICNUMBEROPTION_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "Graphic.h"
#include "GraphicAlpha.h"
#include "GraphicFont.h"

#include "GraphicOption.h"

class CGraphicNumberOption : public CGraphicOption
{
// Construction
public:
	CGraphicNumberOption(int iMin,int iMax,const BOOL fAutoDelete=FALSE)
		: CGraphicOption(fAutoDelete) ,m_iMin(iMin),m_iMax(iMax)
	{}

// Operations
public:

	virtual int GetMin() { return m_iMin; }
	virtual int GetMax() { return m_iMax; }
	virtual CString GetText(int iIndex) 
	{
		CString strText;
		strText.Format("%i",iIndex);
		return strText;
	}

protected:
	int m_iMin,m_iMax;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // GRAPHICNUMBEROPTION_H
