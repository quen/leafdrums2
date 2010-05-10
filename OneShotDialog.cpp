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
#include "OneShotDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COneShotDialog dialog


COneShotDialog::COneShotDialog(DWORD dwMessageID,
							   CWnd* pParent /*=NULL*/)
	: CDialog(COneShotDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(COneShotDialog)
	m_strText = _T("");
	m_fStopShowing = FALSE;
	//}}AFX_DATA_INIT

	m_dwID=dwMessageID;
	m_strText.LoadString(dwMessageID);

	CString strNumber;
	strNumber.Format("%i",m_dwID);
	if(AfxGetApp()->GetProfileInt("One-shot information",
		strNumber,0)!=0)
		m_fStopShowing=TRUE;
}


void COneShotDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COneShotDialog)
	DDX_Text(pDX, IDC_TEXT, m_strText);
	DDX_Check(pDX, IDC_DONTSHOW, m_fStopShowing);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COneShotDialog, CDialog)
	//{{AFX_MSG_MAP(COneShotDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COneShotDialog message handlers

void COneShotDialog::OnOK() 
{
	UpdateData(TRUE);

	if(m_fStopShowing)
	{
		CString strNumber;
		strNumber.Format("%i",m_dwID);
		AfxGetApp()->WriteProfileInt("One-shot information",
			strNumber,1);
	}
	
	CDialog::OnOK();
}

int COneShotDialog::DoModal() 
{
	if(m_fStopShowing) return IDOK;

	// Do not show more than one at a time
	static BOOL fInOneShot;
	if(fInOneShot) return IDOK;

	fInOneShot=TRUE;
	int iRet=CDialog::DoModal();
	fInOneShot=FALSE;

	return iRet;
}
