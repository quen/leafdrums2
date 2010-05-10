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
#ifndef PLAYLIMITS_H
#define PLAYLIMITS_H

class CPlayLimits
{
public:
	// Vector listing track numbers to play (no items = play all tracks)
	vec<int> m_viTracks; 

	// Vector listing lines to play (no items = play all lines)
	vec<int> m_viLines;

	// Return true if track should be played
	BOOL PlayTrack(int iTrack)
	{
		if(m_viTracks.Size()==0) return TRUE;
		return m_viTracks.Find(iTrack)!=-1;
	}

	// Return true if line should be played
	BOOL PlayLine(int iLine)
	{
		if(m_viLines.Size()==0) return TRUE;
		return m_viLines.Find(iLine)!=-1;
	}
};

#endif // PLAYLIMITS_H
