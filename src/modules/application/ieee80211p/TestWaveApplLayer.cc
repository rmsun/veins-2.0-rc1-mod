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

#include "TestWaveApplLayer.h"

Define_Module(TestWaveApplLayer);

void TestWaveApplLayer::initialize(int stage) {
	BaseWaveApplLayer::initialize(stage);
	receivedBeacons = 0;
	receivedWaveData = 0;
	waveDelayTime = 0;
	waveHop = 0;
}

void TestWaveApplLayer::onBeacon(WaveShortMessage* wsm) {
	receivedBeacons++;

	DBG << "Received beacon priority  " << wsm->getPriority() << " at " << simTime() << std::endl;
	int senderId = wsm->getSenderAddress();

	if (sendData) {
		t_channel channel = dataOnSch ? type_SCH : type_CCH;
		sendWSM(prepareWSM("data", dataLengthBits, channel, dataPriority, senderId,2));
	}
}

void TestWaveApplLayer::onData(WaveShortMessage* wsm) {
	EV << "received wave data from 1609.3" << endl;
	//int recipientId = wsm->getRecipientAddress();
	//int recipientId = wsm->getDesIp();
	//if (recipientId == myId) {
		//DBG  << "Received data priority  " << wsm->getPriority() << " at " << simTime() << std::endl;
		receivedWaveData++;
		waveDelayTime += (wsm->getArrivalTime() - wsm->getTimestamp());
		waveHop += wsm->getHop();
		EV << "arrival time: " << wsm->getArrivalTime() << endl;
		//EV << "send time: " << wsm->getSendTime() << endl;
	//}
}

void TestWaveApplLayer::onIpData(Ipv6Pkt* ipkt) {
    //EV << "received ip data from 1609.3" << endl;
    //int recipientId = wsm->getRecipientAddress();
    //int recipientId = wsm->getDesIp();
    //if (recipientId == myId) {
        //DBG  << "Received data priority  " << wsm->getPriority() << " at " << simTime() << std::endl;
       // receivedIpData++;
       // delayTime += (ipkt->getArrivalTime() - ipkt->getTimestamp());
      //  hop += ipkt->getHop();
       // EV << "arrival time: " << ipkt->getArrivalTime() << endl;
        //EV << "send time: " << wsm->getSendTime() << endl;
    //}
}
TestWaveApplLayer::~TestWaveApplLayer() {

}

void TestWaveApplLayer::finish(){

	recordScalar("statsReceivedwaveData", receivedWaveData);
	recordScalar("statswaveDelayTime", waveDelayTime);
	recordScalar("statswaveHop", waveHop);



	std::ofstream f1("rmc/output",std::ios::app);
	f1<<"MyIPAddress "<<"statsReceivedwaveData "<<"statswaveDelayTime "<<"statswaveHop "<<endl;
	f1<<wave1609_3->myIpAddress<<" "<< receivedWaveData<<" "<< waveDelayTime <<" "<<waveHop<<endl;
	f1.close();
	BaseWaveApplLayer::finish();
}

