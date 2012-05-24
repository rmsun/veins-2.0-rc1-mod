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

#ifndef BASEWAVEAPPLLAYER_H_
#define BASEWAVEAPPLLAYER_H_


#include <map>
#include <BaseApplLayer.h>
#include <Consts80211p.h>
#include <Mac80211Pkt_m.h>
#include <WaveShortMessage_m.h>
#include <Ipv6Pkt_m.h>
#include <ChannelAccess.h>
#include <WaveAppToMac1609_4Interface.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
//#include <GlobalTable.h>
#include <Wave1609_3.h>
#include <TraCIMobility.h>

//#include <BaseWaveApplLayerToWave1609_3ControlInfo.h>


#ifndef DBG
#define DBG EV
#endif
//#define DBG std::cerr << "[" << simTime().raw() << "] " << getParentModule()->getFullPath()
class Wave1609_3;

struct table1 //每个节点所要维护的表
{
	//std::string ip;  //ip地址，是否用L3Type??
	//LAddress::L2Type myMacAddr;  //mac地址
	int ip;  //ip地址
	Coord position;  //位置信息
	int count;  //删除邻居节点计数。。。
};

class BaseWaveApplLayer : public BaseApplLayer {

	public:
		~BaseWaveApplLayer();
		virtual void initialize(int stage);
		virtual void finish();

		virtual  void receiveSignal(cComponent* source, simsignal_t signalID, cObject* obj);

		enum WaveApplMessageKinds {  //增加SEND_POS_EVT
			SERVICE_PROVIDER = LAST_BASE_APPL_MESSAGE_KIND,
			SEND_BEACON_EVT,
			SEND_WAVE_POS_EVT,  //以wave模式向所有节点广播位置信息
//			SEND_NEIGHBOR_POS_EVT,  //向邻居广播位置信息,暂时不用了
			SEND_WAVE_DATA_EVT,  //以wave模式发送数据
			SEND_IP_POS_EVT,//以ip模式广播位置
			SEND_IP_DATA_EVT,//以ip模式发送数据
		};

	protected:

		static const simsignalwrap_t mobilityStateChangedSignal;
		//cIListener *mobilityStateChangerListener;


		/** @brief handle messages from below */
		virtual void handleLowerMsg(cMessage* msg);
		/** @brief handle self messages */
		virtual void handleSelfMsg(cMessage* msg);

		virtual WaveShortMessage* prepareWSM(std::string name, int dataLengthBits, t_channel channel, int priority, int rcvId, int serial=0);
		virtual WaveShortMessage* prepareWSM(std::string name, int dataLengthBits, t_channel channel, int priority, int rcvId, int serial, int destAddress, char* data);  //暂时用id来代替ip
		virtual Ipv6Pkt* prepareIpv6Pkt(std::string name, int dataLengthBits,t_channel channel,int rcvId, int destAddress, char* data);//IPv6报文

		virtual void sendWSM(WaveShortMessage* wsm);
		virtual void sendIpv6Pkt(Ipv6Pkt* Ipv6Pkt);
		virtual void onBeacon(WaveShortMessage* wsm) = 0;
		virtual void onData(WaveShortMessage* wsm) = 0;

		virtual void sendingWaveData();
		virtual void sendingIpData();

		virtual void handlePositionUpdate(cObject* obj);

		void onIpData(Ipv6Pkt* ipkt) ;

	protected:
		int beaconLengthBits;
		int beaconPriority;
		bool sendData;
		bool sendBeacons;
		int dataLengthBits;
		bool dataOnSch;
		int dataPriority;
		Coord curPosition;  //当前位置
		int mySCH;
		int myId;

		int packetNum;  //包编号
		int TTL;  //TTL
//		double time;
		int sendIpDataNum;  //发送ip数据包数目
		int sendIpFloodNum; //发送ip位置信息数目
		int receiveIpFloodNum; //接收到ip的位置洪泛信息

		int sendWaveDataNum;  //发送wave数据包数目
		int sendWaveFloodNum; //发送wave位置信息数目
        int receiveWaveFloodNum; //接收到wave的位置洪泛信息

		cMessage* sendBeaconEvt;  //暂时不用了
		cMessage* floodingWavePosEvt;  //wave洪泛位置信息事件
//		cMessage* neighborPosEvt;  //广播邻居位置信息事件
		cMessage* sendWaveDataEvt;  //wave发送数据事件
		cMessage* floodingIpPosEvt;  //ip洪泛位置信息事件
		cMessage* sendIpDataEvt;  //ip发送数据事件

		WaveAppToMac1609_4Interface* myMac;
		Wave1609_3* wave1609_3;
//		TraCIMobilityAccess* mobilityaccess;
//		TraCIMobility* mobility;

		struct table1 tempStruct;
		std::list <struct table1> nodePosition;  //各节点的位置信息

		uint32_t receivedIpData;
		simtime_t ipDelayTime;
		int ipHop;
};

#endif /* BASEWAVEAPPLLAYER_H_ */
