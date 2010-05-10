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
// SetTempoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "leafdrums.h"
#include "SetTempoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetTempoDlg dialog


CSetTempoDlg::CSetTempoDlg(CSongHandler* pshHandler,CSong* psSong,int iBar,CWnd* pwParent )
	: CDialog(CSetTempoDlg::IDD, pwParent)
{
	//{{AFX_DATA_INIT(CSetTempoDlg)
	//}}AFX_DATA_INIT
	m_pshHandler=pshHandler;
	m_psSong=psSong;
	m_iBar=iBar;

	CTimeChange& tc=psSong->GetTimeChange(m_iBar);
	if(!tc.IsEmpty())
	{
		m_iTempo=tc.GetTempo();
		m_iTimeSignature=tc.GetTimeSignature();
	}
	else
	{
		m_iTempo=psSong->GetStartTempo();
		m_iTimeSignature=psSong->GetStartTimeSignature();
	}
}


void CSetTempoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetTempoDlg)
	DDX_Text(pDX, IDC_TEMPO, m_iTempo);
	DDV_MinMaxInt(pDX, m_iTempo, 30, 300);
	DDX_Text(pDX, IDC_TIMESIGNATURE, m_iTimeSignature);
	DDV_MinMaxInt(pDX, m_iTimeSignature, 1, 12);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetTempoDlg, CDialog)
	//{{AFX_MSG_MAP(CSetTempoDlg)
	ON_BN_CLICKED(IDC_APPLY, OnApply)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetTempoDlg message handlers

void CSetTempoDlg::OnApply() 
{
	if(!UpdateData()) return;

	CTimeChange& tc=m_psSong->GetTimeChange(m_iBar);
	tc.SetTempo(m_iTempo);
	tc.SetTimeSignature(m_iTimeSignature);
	m_pshHandler->RepaintSong();
}

void CSetTempoDlg::OnOK() 
{
	if(!UpdateData()) return;
	OnApply();
	
	CDialog::OnOK();
}
