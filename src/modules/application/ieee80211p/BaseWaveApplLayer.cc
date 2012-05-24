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

#include "BaseWaveApplLayer.h"

const simsignalwrap_t BaseWaveApplLayer::mobilityStateChangedSignal = simsignalwrap_t(MIXIM_SIGNAL_MOBILITY_CHANGE_NAME);

void BaseWaveApplLayer::initialize(int stage) {
	BaseApplLayer::initialize(stage);

	srand(time(0));
	if (stage==0) {
		myMac = FindModule<WaveAppToMac1609_4Interface*>::findSubModule(
		            getParentModule()->getParentModule());
		assert(myMac);
		wave1609_3 = FindModule<Wave1609_3*>::findSubModule(
		            getParentModule());
		assert(wave1609_3);

//		mobility = mobilityaccess->get(findHost());
//		assert(mobility);

//		EV<<"the name!!!!!:"<<mobility->getExternalId()<<endl;

		myId = getId();
		findHost()->subscribe(mobilityStateChangedSignal, this);  //注册mobilityStateChangedSignal
		packetNum = 0;  //packetNum初始化为0
		TTL = 256;  //TTL初值为256


		sendWaveDataNum = 0;
		sendWaveFloodNum = 0;
		receiveWaveFloodNum = 0;
		sendIpDataNum = 0;
		sendIpFloodNum = 0;
		receiveIpFloodNum = 0;

		receivedIpData = 0;
		ipDelayTime = 0;
		ipHop = 0;

		//sendCoordinNum = 0;
		//findHost()->subscribe(mobilityStateChangedSignal, this);  //注册mobilityStateChangedSignal
		//EV << "findHost: " <<  findHost()->getFullName();
		headerLength = par("headerLength").longValue();

		sendBeacons = par("sendBeacons").boolValue();
		beaconLengthBits = par("beaconLengthBits").longValue();
		beaconPriority = par("beaconPriority").longValue();

		sendData = par("sendData").boolValue();
		dataLengthBits = par("dataLengthBits").longValue();
		dataOnSch = par("dataOnSch").boolValue();
		dataPriority = par("dataPriority").longValue();

		//sendBeaconEvt = new cMessage("beacon evt", SEND_BEACON_EVT);
		floodingWavePosEvt = new cMessage("wave flooding position evt", SEND_WAVE_POS_EVT);
//		neighborPosEvt = new cMessage("broadcast neighbor's position", SEND_NEIGHBOR_POS_EVT);
		sendWaveDataEvt = new cMessage("wave send data evt", SEND_WAVE_DATA_EVT);

		floodingIpPosEvt = new cMessage("ip flooding position evt", SEND_IP_POS_EVT);
		sendIpDataEvt = new cMessage("ip send data evt", SEND_IP_DATA_EVT);
		//simulate asynchronous channel access
		double offSet = dblrand() * (par("beaconInterval").doubleValue()/2);
		offSet = offSet + floor(offSet/0.050)*0.050;

		if (sendBeacons) {  //改为send position info
			//scheduleAt(simTime() + offSet, sendBeaconEvt);
			//scheduleAt(simTime() + offSet, floodingPosEvt);  //调度flooding position event
			//scheduleAt(simTime() + 0.1, neighborPosEvt);  //调度neighbor position event
			//scheduleAt(simTime(), sendWaveDataEvt);  //调度send data event
			scheduleAt(simTime(), sendIpDataEvt);
		}

	}
}

WaveShortMessage*  BaseWaveApplLayer::prepareWSM(std::string name, int lengthBits, t_channel channel, int priority, int rcvId, int serial) {

	WaveShortMessage* wsm =		new WaveShortMessage(name.c_str());
	wsm->addBitLength(headerLength);  //addBitLength:cPaket报文头的长度(bit)
	wsm->addBitLength(lengthBits);

	switch (channel) {
		case type_SCH: wsm->setChannelNumber(Channels::SCH1); break; //will be rewritten at Mac1609_4 to actual Service Channel. This is just so no controlInfo is needed
		case type_CCH: wsm->setChannelNumber(Channels::CCH); break;
	}
	wsm->setPsid(0);
	wsm->setPriority(priority);
	wsm->setWsmVersion(1);
	wsm->setTimestamp(simTime());
	wsm->setSenderAddress(myId);
	wsm->setRecipientAddress(rcvId);
	wsm->setSenderPos(curPosition);
	wsm->setSerial(serial);  //serial是什么

	if (name == "beacon") {
		DBG << "Creating Beacon with Priority " << priority << " at Applayer at " << wsm->getTimestamp() << std::endl;
	}
	if (name == "data") {
		DBG << "Creating Data with Priority " << priority << " at Applayer at " << wsm->getTimestamp() << std::endl;
	}

	return wsm;
}

WaveShortMessage*  BaseWaveApplLayer::prepareWSM(std::string name, int lengthBits, t_channel channel, int priority, int rcvId, int serial, int destAddress, char* data) {

//    BaseWaveApplLayerToWave1609_3ControlInfo* addInfo = new BaseWaveApplLayerToWave1609_3ControlInfo(0);

	WaveShortMessage* wsm =		new WaveShortMessage(name.c_str());
//	wsm->addBitLength(headerLength);  //addBitLength:cPaket报文头的长度(bit)默认256
//	wsm->addBitLength(lengthBits);  //.dataLength

	switch (channel) {
		case type_SCH: wsm->setChannelNumber(Channels::SCH1); break; //will be rewritten at Mac1609_4 to actual Service Channel. This is just so no controlInfo is needed
		case type_CCH: wsm->setChannelNumber(Channels::CCH); break;
	}
	wsm->setPsid(0);
	wsm->setPriority(priority);
	wsm->setWsmVersion(1);
	wsm->setTimestamp(simTime());
	//EV << "prepareWSM simTime: " << simTime() << endl;
//	time = simTime().dbl();
	//wsm->setSendTime(time);
	wsm->setSenderAddress(myId);
	wsm->setRecipientAddress(rcvId);
	wsm->setSenderPos(curPosition);
	wsm->setSerial(serial);  //serial是什么


	wsm->setHop(0);  //hop初始化为0

	if (name == "wave flooding position")  //位置信息
	{
		wsm->setPacketType(WPOSITION);
		wsm->setTtl(TTL);
		wsm->setPacketNum(packetNum++);  //设置包编号

	}
//	if (name == "broadcast neighbor's position")
//	{
//		EV << "prepareWSM: yes this is neighbor's position" << endl;
//		//wsm->setPacketType(NEIGHBOR);  //位置信息
//		wsm->setPacketType(NEIGHBOR);
//	}
	if (name == "wave coordinate")  //协调信息
		wsm->setPacketType(WCOORDINATE_INFO);
	if (name == "wave routing data")  //数据
		wsm->setPacketType(WDATA);

	wsm->setSourceIp(myId);  //源IP,到1609.3模块再修改
	wsm->setNextIp(myId);
	wsm->setDesIp(destAddress); //0为广播，
	wsm->setLastIp(-1);  //lastIp为-1为原始数据包，没有转发

	//wsm->setTtl(TTL);  //设置TTL
	//if (name == "broadcast neighbor's position")
		//wsm->setTtl(1);  //设置TTL为1
	//else
	wsm->setTtl(TTL);
	wsm->setDataLength(lengthBits);  //数据长度
	wsm->setData(data);  //数据
	wsm->setGpsFlag(1);//初始化时标为1代表既是位置又是邻居消息
	//if (name == "beacon") {
		//DBG << "Creating Beacon with Priority " << priority << " at Applayer at " << wsm->getTimestamp() << std::endl;
	//}
	if (name == "data") {
		DBG << "Creating Data with Priority " << priority << " at Applayer at " << wsm->getTimestamp() << std::endl;
	}

//	wsm->setControlInfo(addInfo);
//	wsm->addBitLength(headerLength);
//	std::cout<<"appwsm:"<<sizeof(*wsm)<<endl;
//	wsm->encapsulate(wsm);

	return wsm;
}

Ipv6Pkt* BaseWaveApplLayer::prepareIpv6Pkt(std::string name, int lengthBits,t_channel channel,int rcvId, int destAddress, char* data)
{

//    BaseWaveApplLayerToWave1609_3ControlInfo* addInfo = new BaseWaveApplLayerToWave1609_3ControlInfo(6);

    Ipv6Pkt* ipv6pkt =     new Ipv6Pkt(name.c_str());
    ipv6pkt->addBitLength(headerLength);  //addBitLength:cPaket报文头的长度(bit)默认256
    ipv6pkt->addBitLength(lengthBits);  //.dataLength

    switch (channel)
    {
        case type_SCH: ipv6pkt->setChannelNumber(Channels::SCH1); break; //will be rewritten at Mac1609_4 to actual Service Channel. This is just so no controlInfo is needed
        case type_CCH: ipv6pkt->setChannelNumber(Channels::CCH); break;
     }



    ipv6pkt->setTimestamp(simTime());
    //EV << "prepareWSM simTime: " << simTime() << endl;
    //  time = simTime().dbl();
    //wsm->setSendTime(time);
    ipv6pkt->setSenderAddress(myId);
    ipv6pkt->setRecipientAddress(rcvId);
    ipv6pkt->setSenderPos(curPosition);


    ipv6pkt->setHop(0);  //hop初始化为0

    if (name == "ip flooding position")  //位置信息
    {
        ipv6pkt->setIpv6PacketType(IPOSITION);
        ipv6pkt->setTtl(TTL);
        ipv6pkt->setPacketNum(packetNum++);  //设置包编号

    }
//  if (name == "broadcast neighbor's position")
//  {
//      EV << "prepareWSM: yes this is neighbor's position" << endl;
//      //wsm->setPacketType(NEIGHBOR);  //位置信息
//      wsm->setPacketType(NEIGHBOR);
//  }
    if (name == "ip coordinate")  //协调信息
    {
        ipv6pkt->setIpv6PacketType(ICOORDINATE_INFO);
    }
    if (name == "ip routing data")  //数据
    {
        ipv6pkt->setIpv6PacketType(IDATA);
    }

    ipv6pkt->setSourceIp(myId);  //源IP,到1609.3模块再修改
    ipv6pkt->setNextIp(myId);
    ipv6pkt->setDesIp(destAddress); //0为广播，
    ipv6pkt->setLastIp(-1);  //lastIp为-1为原始数据包，没有转发

    ipv6pkt->setTtl(TTL);
    ipv6pkt->setDataLength(lengthBits);  //数据长度
    ipv6pkt->setData(data);  //数据
    ipv6pkt->setGpsFlag(1);//初始化时标为1代表既是位置又是邻居消息

//    if (name == "data") {
//        DBG << "Creating Data with Priority " << priority << " at Applayer at " << ipv6pkt->getTimestamp() << std::endl;
//    }

//    ipv6pkt->setControlInfo(addInfo);

    return ipv6pkt;

}

void BaseWaveApplLayer::receiveSignal(cComponent* source, simsignal_t signalID, cObject* obj) {
	EV << "app: receiveSignal" << endl;
	Enter_Method_Silent();
	if (signalID == mobilityStateChangedSignal) {
		handlePositionUpdate(obj);
	}
}

void BaseWaveApplLayer::handlePositionUpdate(cObject* obj) {
	EV << "handlePositionUpdate ..." << endl;
	ChannelMobilityPtrType const mobility = check_and_cast<ChannelMobilityPtrType>(obj);
	curPosition = mobility->getCurrentPosition();
	//scheduleAt(simTime(), floodingPosEvt);  //位置更新后，重新广播自己的位置信息。。新增。。第一个消息是在这里产生。。。

	//scheduleAt(simTime(), floodingWavePosEvt);
	scheduleAt(simTime(), floodingIpPosEvt);
//	scheduleAt(simTime(), neighborPosEvt);  //同时向邻居广播位置信息
	//scheduleAt(simTime(), sendDataEvt);
	EV << "handlePositionUpdate down..." << endl;
}

void BaseWaveApplLayer::handleLowerMsg(cMessage* msg) {  //处理1609.3发来的消息
	EV << "BaseWaveApplLayer: handleLowerMsg, received msg from 1609.3" << endl;
	//Mac80211Pkt* mac = dynamic_cast<Mac80211Pkt*>(msg);
	//ASSERT(mac);

	//WaveShortMessage*  wsm =  dynamic_cast<WaveShortMessage*>(mac->decapsulate());
	//EV << "wsm->getName() == " << wsm->getName() << endl;

//	BaseWaveApplLayerToWave1609_3ControlInfo* addInfo = dynamic_cast<BaseWaveApplLayerToWave1609_3ControlInfo*>(msg->getControlInfo());
//	assert(addInfo);
//	int tmprotocolId = addInfo->protocolId;


	//判断收到的报文协议类型,0代表wsmp，6代表ipv6
	int tmprotocolId = -1;
	WaveShortMessage* wsm = dynamic_cast<WaveShortMessage*>(msg);
	Ipv6Pkt* ipkt = dynamic_cast<Ipv6Pkt*>(msg);
	 if(wsm!=NULL)
	 {
	      tmprotocolId = 0;
	 }
	 else if(ipkt != NULL)
	 {
	      tmprotocolId = 6;
	 }
	 else
	 {
	      tmprotocolId = -1;
	 }


	//收到的是wave
	if(tmprotocolId == 0)
	{
//	    WaveShortMessage* wsm = dynamic_cast<WaveShortMessage*>(msg);
	    if (wsm != NULL)
	    {

	        if (std::string(wsm->getName()) == "beacon")
	        {
	            onBeacon(wsm);
	        }
	        //else if (std::string(wsm->getName()) == "data") {
	        //else if (std::string(wsm->getName()) == "routing data") {
			//onData(wsm);
			//;
	        //}
	        //else {
			//ASSERT(false);
	        //}
	        if (std::string(wsm->getName()) == "wave flooding position")  //更新nodePosition
	        {
	            EV << "BaseWaveApplLayer: yes receive wave flooding position.." << endl;
	            receiveWaveFloodNum++;
	            tempStruct.ip = wsm->getSourceIp();  //＝＝senderAddress
	            tempStruct.position = wsm->getSenderPos();  //节点的位置坐标
	            std::list<struct table1>::iterator itr = nodePosition.begin();
	            for (; itr != nodePosition.end(); itr++)
	            {
	                if (itr->ip == tempStruct.ip)  //ip已在list中，只需要更新位置信息即可
	                {
	                    itr->position = tempStruct.position;  //更新位置信息
	                    break;  //跳出for循环
	                }
	            }
	            if (itr == nodePosition.end())  //没有找到对应的ip,如果ip不在list中，则将ip及其对应的位置信息push_back到list中
	            {
	                nodePosition.push_back(tempStruct);  //将节点的信息放入list
							//ipExisted = false;  //重置ipExisted
	            }

	        }

	        if (std::string(wsm->getName()) == "wave routing data")
	        {
	            onData(wsm);
	        }
		//delete(wsm);
	    }
	    else
	    {
	        assert(false);
	    }

	}

	else if(tmprotocolId == 6)//收到IP
	{
//	    Ipv6Pkt* ipkt = dynamic_cast<Ipv6Pkt*>(msg);
	    if(ipkt!=NULL)
	    {

	          if (std::string(ipkt->getName()) == "ip flooding position")  //更新nodePosition
	          {

	                EV << "BaseWaveApplLayer: yes receive ip flooding position.." << endl;
	                receiveIpFloodNum++;
	                tempStruct.ip = ipkt->getSourceIp();  //＝＝senderAddress
	                tempStruct.position = ipkt->getSenderPos();  //节点的位置坐标
	                 std::list<struct table1>::iterator itr = nodePosition.begin();
	                 for (; itr != nodePosition.end(); itr++)
	                 {
	                        if (itr->ip == tempStruct.ip)  //ip已在list中，只需要更新位置信息即可
	                        {
	                             itr->position = tempStruct.position;  //更新位置信息
	                             break;  //跳出for循环
	                        }
	                  }
	                  if (itr == nodePosition.end())  //没有找到对应的ip,如果ip不在list中，则将ip及其对应的位置信息push_back到list中
	                  {
	                        nodePosition.push_back(tempStruct);  //将节点的信息放入list
	                                    //ipExisted = false;  //重置ipExisted
	                  }

	           }

	           if (std::string(ipkt->getName()) == "ip routing data")
	           {
	                 onIpData(ipkt);
	           }

	    }
	    else
	    {
	        assert(false);
	    }
	}
	else
	{
	    DBG << "unknown message received\n";
	}
	delete(msg);
}

void BaseWaveApplLayer::handleSelfMsg(cMessage* msg)
{
	switch (msg->getKind())
	{
		case SEND_BEACON_EVT:
		{  //不会再执行这里了，取消了beacon
			//sendWSM(prepareWSM("beacon", beaconLengthBits, type_CCH, beaconPriority, 0, -1));
			//scheduleAt(simTime() + par("beaconInterval").doubleValue(), sendBeaconEvt);
			break;
		}
		case SEND_WAVE_POS_EVT:
		{
			sendWSM(prepareWSM("wave flooding position", beaconLengthBits, type_CCH, beaconPriority, 0, -1, 0, "wave flooding position"));  //destAddress==0代表广播
			sendWaveFloodNum++;
			//scheduleAt(simTime(), floodingPosEvt);  //调度floodingPosEVt，position update后再次floodingPosEvt
			break;
		}
		case SEND_IP_POS_EVT:
		{
		    sendIpv6Pkt(prepareIpv6Pkt("ip flooding position",beaconLengthBits,type_CCH,-1, 0, "ip flooding position"));  //destAddress==0代表广播
		    sendIpFloodNum++;
		    //scheduleAt(simTime(), floodingPosEvt);  //调度floodingPosEVt，position update后再次floodingPosEvt
		    break;
		}
//		case SEND_NEIGHBOR_POS_EVT: {
//			sendWSM(prepareWSM("broadcast neighbor's position", beaconLengthBits, type_CCH, beaconPriority, 0, -1, 0, "neighbor's position"));
//			break;
//		}
		case SEND_WAVE_DATA_EVT:
		{
			sendingWaveData();  //应用层发送wave数据
			break;
		}
		case SEND_IP_DATA_EVT:
		{
		    sendingIpData();  //应用层发送ip数据
		    break;
		}
		default:
		{
			if (msg)
				DBG << "APP: Error: Got Self Message of unknown kind! Name: " << msg->getName() << endl;
			break;
		}
	}
}

void BaseWaveApplLayer::sendWSM(WaveShortMessage* wsm)
{  //往1609.3发wave报文
    //std::cout<<"appsenddown:"<<sizeof(*wsm)<<endl;
	sendDown(wsm);
}

void BaseWaveApplLayer::sendIpv6Pkt(Ipv6Pkt* ipv6Pkt)
{  //往1609.3发IP报文
    sendDown(ipv6Pkt);
}


void BaseWaveApplLayer::sendingWaveData()
{
	if (!(nodePosition.empty()))  //nodePosition不为空
	{
		std::list<struct table1>::iterator itr = nodePosition.begin();
		//EV << "nodePosition' size is: " << wave1609_3->nodePosition.size() << endl;
		//std::list<struct table>::iterator itr = Wave1609_3::nodePosition.begin();

		int num = nodePosition.size()*0.1;
		if(num>=1)
		{
		    if(num==1)
		    {
		        sendWSM(prepareWSM("wave routing data", beaconLengthBits, type_SCH, beaconPriority, 0, -1,itr->ip, "wave test data"));  //向其它所有节点发送routing data
		        sendWaveDataNum++;
		    }
		    else
		    {
		        for(int i=0; i!=num; i++)
		        {
		            itr = nodePosition.begin();
		            int sendid = rand()%num;
		            for(int j=0;j<sendid;j++)
		            {
		                itr++;
		            }
		            sendWSM(prepareWSM("wave routing data", beaconLengthBits, type_SCH, beaconPriority, 0, -1, itr->ip, "wave test data"));  //向其它所有节点发送routing data
		            sendWaveDataNum++;
		        }
		    }
		}

		//EV << "first ip is: " << itr->ip << endl;
		EV << "haha..." << endl;
	}

	scheduleAt(simTime()+0.01, sendWaveDataEvt);  //重新调度sendDtatEvt,以1Mbps的速率发送数据包 10kbps(0.05)
}

void BaseWaveApplLayer::sendingIpData()
{
    if (!(nodePosition.empty()))  //nodePosition不为空
    {
            std::list<struct table1>::iterator itr = nodePosition.begin();
            //EV << "nodePosition' size is: " << wave1609_3->nodePosition.size() << endl;
            //std::list<struct table>::iterator itr = Wave1609_3::nodePosition.begin();

            int num = nodePosition.size()*0.1;
            if(num>=1)
            {
                if(num==1)
                {
                    sendIpv6Pkt(prepareIpv6Pkt("ip routing data",beaconLengthBits,type_CCH,-1, itr->ip, "ip test data"));  //destAddress==0代表广播
                    sendIpDataNum++;
                }
                else
                {
                    for(int i=0; i!=num; i++)
                    {
                        itr = nodePosition.begin();
                        int sendid = rand()%num;
                        for(int j=0;j<sendid;j++)
                        {
                            itr++;
                        }
                        sendIpv6Pkt(prepareIpv6Pkt("ip routing data",beaconLengthBits,type_CCH,-1, itr->ip, "ip test data"));   //向其它所有节点发送routing data
                        sendIpDataNum++;
                    }
                }
            }

            //EV << "first ip is: " << itr->ip << endl;
            EV << "haha..." << endl;
    }

    scheduleAt(simTime()+0.01, sendIpDataEvt);  //重新调度sendDtatEvt,以1Mbps的速率发送数据包 10kbps(0.05)

}
void BaseWaveApplLayer::finish() {
	//cancelAndDelete(sendBeaconEvt);
	//cancelAndDelete(floodingPosEvt);
	recordScalar("sendWaveDataNumber", sendWaveDataNum);
	recordScalar("sendWaveFloodNumber",sendWaveFloodNum);
	recordScalar("receiveWaveFloodNumber",receiveWaveFloodNum);

	recordScalar("sendIpDataNumber", sendIpDataNum);
	recordScalar("sendIpFloodNumber",sendIpFloodNum);
	recordScalar("receiveIpFloodNumber",receiveIpFloodNum);

	recordScalar("statsReceivedIpData", receivedIpData);
	recordScalar("statsIpDelayTime", ipDelayTime);
	recordScalar("statsIpHop", ipHop);



	std::ofstream f1("rmc/output",std::ios::app);
	//f1<<wave1609_3->myIpAddress<<endl;
	f1<<"sendWaveDataNum "<<"sendWaveFloodNum "<<"receiveWaveFloodNum "<<"sendIpDataNum "<<"sendIpFloodNum "<<"receiveIpFloodNum "<<"statsReceivedIpData "<<"statsIpDelayTime "<<"statsIpHop "<<endl;
	f1<<sendWaveDataNum<<" "<<sendWaveFloodNum<<" "<<receiveWaveFloodNum<<" "<<sendIpDataNum<<" "<<sendIpFloodNum<<" "<<receiveIpFloodNum<<" "<< receivedIpData<<" "<< ipDelayTime <<" "<<ipHop<<endl;
	f1.close();
}

void BaseWaveApplLayer::onIpData(Ipv6Pkt* ipkt)
{
    EV << "received ip data from 1609.3" << endl;
        //int recipientId = wsm->getRecipientAddress();
        //int recipientId = wsm->getDesIp();
        //if (recipientId == myId) {
            //DBG  << "Received data priority  " << wsm->getPriority() << " at " << simTime() << std::endl;
            receivedIpData++;
            ipDelayTime += (ipkt->getArrivalTime() - ipkt->getTimestamp());
            ipHop += ipkt->getHop();
            EV << "arrival time: " << ipkt->getArrivalTime() << endl;

}

BaseWaveApplLayer::~BaseWaveApplLayer()
{
 //   delete wave1609_3;
}
