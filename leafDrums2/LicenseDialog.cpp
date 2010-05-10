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
#include "resource.h"
#include "LicenseDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLicenseDialog dialog


CLicenseDialog::CLicenseDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CLicenseDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLicenseDialog)
	m_strLicense = _T("");
	//}}AFX_DATA_INIT
}


void CLicenseDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLicenseDialog)
	DDX_Text(pDX, IDC_EDITLICENSE, m_strLicense);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLicenseDialog, CDialog)
	//{{AFX_MSG_MAP(CLicenseDialog)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLicenseDialog message handlers
