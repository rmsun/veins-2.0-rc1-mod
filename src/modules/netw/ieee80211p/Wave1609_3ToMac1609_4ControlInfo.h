//
// Copyright (C) 2011 David Eckhoff <eckhoff@cs.fau.de>
//
// Documentation for these modules is at http://veins.car2x.org/
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//

/**
 * @brief Interface between BaseWaveApplLayer and Wave1609_3.
 *
 * @author rmc
 */



#ifndef ___WAVE1609_3TOMAC1609_4CONTROLINFO_H_
#define ___WAVE1609_3TOMAC1609_4CONTROLINFO_H_


class Wave1609_3ToMac1609_4ControlInfo:public cObject
{
	public:
        Wave1609_3ToMac1609_4ControlInfo(int protocolId)
        :protocolId(protocolId)
        {

		};

        virtual ~Wave1609_3ToMac1609_4ControlInfo()
		{

		};

		int protocolId;
};

#endif /* WAVE1609_3TOMAC1609_4CONTROLINFO_H_*/
