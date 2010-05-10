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
#include "LeafDrums.h"
#include "WriteDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWriteDlg dialog


CWriteDlg::CWriteDlg(CSong& song,CWnd* pParent /*=NULL*/)
: CDialog(CWriteDlg::IDD, pParent), m_song(song)
{
	//{{AFX_DATA_INIT(CWriteDlg)
	m_iFormat = -1;
	//}}AFX_DATA_INIT
	m_iOutputStyle = 0;
	m_iCount = 1;
}



void CWriteDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWriteDlg)
	DDX_Text(pDX, IDC_COUNT, m_iCount);
	DDV_MinMaxInt(pDX, m_iCount, 1, 99);
	DDX_Radio(pDX, IDC_OUTPUTSTYLE0, m_iOutputStyle);
	DDX_CBIndex(pDX, IDC_FORMAT, m_iFormat);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWriteDlg, CDialog)
	//{{AFX_MSG_MAP(CWriteDlg)
	ON_EN_CHANGE(IDC_COUNT, OnChangeCount)
	ON_BN_CLICKED(IDC_OUTPUTSTYLE0, OnChangeCount)
	ON_BN_CLICKED(IDC_OUTPUTSTYLE1, OnChangeCount)
	ON_BN_CLICKED(IDC_OUTPUTSTYLE2, OnChangeCount)
	ON_CBN_SELCHANGE(IDC_FORMAT, OnChangeCount)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWriteDlg message handlers

void CWriteDlg::OnChangeCount() 
{
	if(!UpdateData(TRUE)) return;

	double dSize=(double)(m_iCount*m_song.GetWriteSamples()*4);
	dSize/=(1024*1024);

	int iFiles=1;
	switch(m_iOutputStyle)
	{
	case CSong_WRITEWHOLE:
		break;
	case CSong_WRITESOUNDS:
		iFiles=m_song.GetNumUsedSounds();
		break;
	case CSong_WRITETRACKS:
		iFiles=m_song.GetNumUsedTracks();
		break;
	}

	dSize*=iFiles;

	switch(m_iFormat)
	{
	case WRITEFORMAT_WAV: 
		break;
	case WRITEFORMAT_OGGHIGH: 
		dSize/=7.35; 
		break;
	case WRITEFORMAT_OGGLOW: 
		dSize/=14.7; 
		break;
	}

	CString strSize;
	if(dSize>1.0)
		strSize.Format("(Size of file%s %.2lf MB)",((iFiles>1) ? "s" : ""),dSize);
	else
		strSize.Format("(Size of file%s %.0lf KB)",((iFiles>1) ? "s" : ""),dSize*1024.0);

	GetDlgItem(IDC_FILESIZE)->SetWindowText(strSize);	
}

BOOL CWriteDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CComboBox* pCombo=(CComboBox*)GetDlgItem(IDC_FORMAT);
	pCombo->AddString(".wav (uncompressed, 1400kb/s)");
	pCombo->AddString(".ogg (high quality, ~192kb/s)");
	pCombo->AddString(".ogg (Web quality, ~96kb/s)");
	pCombo->SetCurSel(0);
	
	OnChangeCount();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

