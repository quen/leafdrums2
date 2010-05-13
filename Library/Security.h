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
#ifndef SECURITY_H
#define SECURITY_H

class CRegistration
{
private:
	BOOL m_fRegistered;
	CString m_strUserName;

	int m_iConstant;

	int m_iDaysUsed;

	int CheckRegistered(CString temp,int Number);

public:
	static CRegistration* sm_pCurrent;

	CRegistration(int constant,CWinApp* app);

	CString RegisteredName();
	BOOL Registered();

	int DaysUsed() { return m_iDaysUsed; }
};

#endif // SECURITY_H
