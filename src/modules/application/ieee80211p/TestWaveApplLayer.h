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

#ifndef TESTWAVEAPPLLAYER_H_
#define TESTWAVEAPPLLAYER_H_

#include "BaseModule.h"
#include <BaseWaveApplLayer.h>
//#include <GlobalTable.h>

#ifndef DBG
#define DBG EV
#endif
//#define DBG std::cerr << "[" << simTime().raw() << "] " << getParentModule()->getFullPath()

class TestWaveApplLayer  : 	public BaseWaveApplLayer {
	public:

		virtual ~TestWaveApplLayer();

		virtual void initialize(int stage);
		virtual void finish();

	protected:
		virtual void onBeacon(WaveShortMessage* wsm);
		virtual void onData(WaveShortMessage* wsm);
		virtual void onIpData(Ipv6Pkt* ipkt);

	protected:
		uint32_t receivedBeacons;
		uint32_t receivedWaveData;
//		uint32_t receivedIpData;


		simtime_t waveDelayTime;
		int waveHop;
};

#endif /* TESTWAVEAPPLLAYER_H_ */
