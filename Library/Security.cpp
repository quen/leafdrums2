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
#include "Security.h"

CRegistration* CRegistration::sm_pCurrent;

int rseed;

int random(int r)	// Between 0 and r-1 inclusive

  {
  int c;
  rseed=(int)(177*(long)(rseed)%30269);
  c=(int)(((float)(rseed)/32767)*r);
  return c;
  } 


CString CRegistration::RegisteredName()
{
	if(m_fRegistered) return m_strUserName;
	else return "";
}

BOOL CRegistration::Registered()
{
	return m_fRegistered;
}

int CRegistration::CheckRegistered(CString temp,
	int Number)
{
	long Answer;
	int q;
	if(temp.GetLength()<8) return 0;
	rseed=0;
	for(q=0;q<temp.GetLength();q++) 
		rseed+=temp[q]*m_iConstant;
	Answer=0;
	for(q=0;q<temp.GetLength();q++)
	{
		Answer+=temp[q]*(rseed%256);
		random(10);
	}
	Answer%=30000;
	if(Answer==Number) return TRUE;
		else return FALSE;
}

CRegistration::CRegistration(int iConstant,CWinApp* app)
{
	sm_pCurrent=this;
	m_iConstant=iConstant;

	m_fRegistered=FALSE;

	CFile f;

	// Check for leafdrums2.key in program directory
	char acName[1024];
	GetModuleFileName(NULL,acName,sizeof(acName));
	CString sName=acName;
	sName=sName.Left(sName.GetLength()-3)+"key";
	
	if(f.Open(sName,CFile::modeRead|CFile::shareDenyWrite))
	{
		char acData[1024];
		int iRead=f.Read(acData,sizeof(acData)-1);
		acData[iRead]=0;
		f.Close();
		CString sKey=acData;

		if(sKey.Find(':')!=-1)
		{
			m_strUserName=sKey.Left(sKey.Find(':'));
			if(CheckRegistered(m_strUserName,
				atoi(sKey.Mid(sKey.Find(':')+1))))
				m_fRegistered=TRUE;			
		}
	}
	else
	{
		m_strUserName=app->GetProfileString("Registration","Name",NULL);
		if(CheckRegistered(m_strUserName,
			app->GetProfileInt("Registration","Number",0)))
			m_fRegistered=TRUE;
	}

	DWORD dwDayCode=app->GetProfileInt("Misc","Cookie",1);
	DWORD dwTopPart=dwDayCode>>28;
	DWORD dwLowPart=dwDayCode&0x0fffffff;

	FILETIME ft;
	GetSystemTimeAsFileTime(&ft);

	if(dwDayCode==1)
	{
		DWORD dwHoursTime=ft.dwHighDateTime >> 4;
		// Now in somewhere around the hours range :)
		// 6871.95 seconds, 12.57 of those in a day

		dwLowPart=dwHoursTime;
		dwTopPart=((dwLowPart * 37) & 0xf);

		dwDayCode=dwLowPart | (dwTopPart << 28);
		app->WriteProfileInt("Misc","Cookie",dwDayCode);
	}

	if( ((dwLowPart * 37) & 0xf) != dwTopPart )
		m_iDaysUsed=99;
	else
	{
		DWORD dwHoursTime=ft.dwHighDateTime >> 4;
		// Now in somewhere around the hours range :)
		// 6871.95 seconds, 12.57 of those in a day

		m_iDaysUsed=(dwHoursTime-dwLowPart);
		m_iDaysUsed*=6782;		// Seconds
		m_iDaysUsed/=3600*24;	// Days
	}
}






