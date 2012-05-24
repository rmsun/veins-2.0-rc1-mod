/**
 * @author: sun
 * @date: 2012.3.20
 */
#ifndef WAVE1609_3_H_
#define WAVE1609_3_H_

#include <assert.h>
#include <omnetpp.h>
#include <queue>
#include <BaseLayer.h>
#include <Mac80211pToPhy11pInterface.h>
#include <Mac1609_4To80211pControlInfo.h>
#include <MacToPhyInterface.h>
#include <NetwToMacControlInfo.h>
#include <Mac80211pToPhy11pInterface.h>
#include <Mac80211pToMac1609_4Interface.h>
#include <WaveAppToMac1609_4Interface.h>
#include <Consts80211p.h>
#include "FindModule.h"
#include <Mac80211Pkt_m.h>
#include <WaveShortMessage_m.h>
#include <Ipv6Pkt_m.h>
#include <Coord.h>
#include <list>
#include <iostream>
#include <fstream>
#include <string>
#include <TraCIScenarioManager.h>
#include <TraCIMobility.h>
#include <BaseConnectionManager.h>
#include <PhyLayer80211p.h>
//#include <BaseWaveApplLayerToWave1609_3ControlInfo.h>
//#include <Wave1609_3ToMac1609_4ControlInfo.h>
//#include <GlobalTable.h>

#ifndef DBG
#define DBG EV
#endif

struct table  //每个节点所要维护的表
{
	//std::string ip;  //ip地址，是否用L3Type??
	//LAddress::L2Type myMacAddr;  //mac地址
	int ip;  //ip地址
	Coord position;  //位置信息
	int count;  //删除邻居节点计数。。。,在nodePosition中用于防止重得转发
};

class Wave1609_3 : public BaseLayer
{

	protected:
		/** @brief Initialization of the module and some variables.*/
		virtual void initialize(int);

		/** @brief Delete all dynamically allocated objects of the module.*/
		virtual void finish();

		/** @brief Handle messages from lower layer.*/
		virtual void handleLowerMsg(cMessage*);

		/** @brief Handle messages from upper layer.*/
		virtual void handleUpperMsg(cMessage*);

		/** @brief Handle control messages from upper layer.*/
		virtual void handleUpperControl(cMessage* msg);


		/** @brief Handle self messages such as timers.*/
		virtual void handleSelfMsg(cMessage*);

		/** @brief Handle control messages from lower layer.*/
		virtual void handleLowerControl(cMessage* msg);

		/** @brief routing function. */

		void waveRouting(WaveShortMessage *wsm);//wave路由主函数

		bool waveGreedyForward(WaveShortMessage *wsm);//wave贪心模式路由

		bool waveRecoveryForward(WaveShortMessage *wsm);//wave恢复模式路由

		void waveBroadcastRouting(WaveShortMessage *wsm);//wave广播路由算法

		void ipRouting(Ipv6Pkt *ipkt);//ip路由主函数

		bool ipGreedyForward(Ipv6Pkt *ipkt);//ip贪心模式路由

		bool ipRecoveryForward(Ipv6Pkt *ipkt);//ip恢复模式路由

		void ipBroadcastRouting(Ipv6Pkt *ipkt);//ip广播路由算法


		int selectNeighbor(int &lastip,int &destip,Coord &desposition);//选择下一跳

		bool getIpPosition(int ip,Coord & desposition,std::list <struct table> & tablelist);//由ip查表得到位置信息

		bool coordJudge(std::list <struct table> &tablelist);//协调节点判定

		void processWavePkt(WaveShortMessage *wsm);//处理下层传来的wave报文

		void processIpkt(Ipv6Pkt* ipkt);//处理下层传来的ip报文
		/** @brief 每个节点所要维护的三张表 */
		//std::list <struct table> nodePosition;  //各节点的位置信息
		//std::list <struct table> neighborInfo;  //邻居信息
		//std::list <struct table> coordinateInfo;  //协调节点信息

		//cMessage *floodingPos;  //洪泛位置信息的消息
		Coord myPosition;  //当前位置

		int maxCount;
		struct table tempStruct;
		int routeMethod;//具体选择的路由算法，0为地理位置（默认），1为广播
		bool ipExisted;  //位置信息表是否更新，初始值为false，以此判定是否转发包



		int sendWaveCoordinNum;
		int receiveWaveCoordinNum;
		int netwreceiveWaveFloodNum;
		int netwsendWaveFloodNum;
		int netwreceiveWaveData;
		int netwsendWaveData;

		int sendIpCoordinNum;
		int receiveIpCoordinNum;
		int netwreceiveIpFloodNum;
		int netwsendIpFloodNum;
		int netwreceiveIpData;
		int netwsendIpData;
	public:
		std::list <struct table> nodePosition;  //各节点的位置信息
		std::list <struct table> neighborInfo;  //邻居信息
		std::list <struct table> coordinateInfo;  //协调节点信息
		int myIpAddress;  //IP地址
		//friend class BaseWaveApplLayer;
		//增加，获取模块名字时使用
		TraCIMobilityAccess* mobilityaccess;
		TraCIMobility* mobility;
		TraCIScenarioManager* traciManager;
		BaseConnectionManager* cc;
};

#endif
