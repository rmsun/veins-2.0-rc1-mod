/**
 * @author: sun
 * @date: 2012.3.20
 * @date: 2013.5.7byrmc
 */

#include "Wave1609_3.h"

Define_Module(Wave1609_3);

void Wave1609_3::initialize(int stage)
{  //被callInitialization调用
	BaseLayer::initialize(stage);  //初始化BaseLayer

	if (stage == 0)
	{  //在stage0进行初始化
		//floodingPos = new cMessage("floodingPosition");
		//scheduleAt(simTime(), floodingPos);  //洪泛位置信息
		myIpAddress = getId();  //获取模块ID作为IP地址
		EV<<"myIpAddress:"<<myIpAddress<<endl;
		ipExisted = false;  //ipExisted初值为false,表示没有更新位置信息表
		maxCount = 10;
		sendWaveCoordinNum=0;
		receiveWaveCoordinNum=0;
		netwreceiveWaveFloodNum=0;
		netwsendWaveFloodNum=0;
		netwreceiveWaveData=0;
		netwsendWaveData=0;

		sendIpCoordinNum=0;
		receiveIpCoordinNum=0;
        netwreceiveIpFloodNum=0;
        netwsendIpFloodNum=0;
        netwreceiveIpData=0;
        netwsendIpData=0;


		routeMethod = par("routeMethod").longValue();

		traciManager = FindModule<TraCIScenarioManager*>::findGlobalModule();

		cc =  FindModule<BaseConnectionManager* >::findGlobalModule();

        mobility = mobilityaccess->get(findHost());
        assert(mobility);

        EV<<"the name!!!!!:"<<mobility->getExternalId()<<endl;





        if(mobility->getExternalId()=="RSU1")
       {
            //Wave1609_3* wave = FindModule<Wave1609_3*>::findSubModule(traciManager->hosts.find("RSU0")->second);
            //std::cout<<"bbb"<<endl;
            //const NicEntry::GateList& gateList = cc->getGateList(phy->getParentModule()->getId());
            //std::cout<<"ccc"<<endl;
            //NicEntry::GateList::const_iterator i = gateList.begin();

            //NicEntry* tmpnics = cc->nics.find(phy->getParentModule()->getId());
            //cGate* g = phy->getParentModule()->gate("radioIn");
            cGate* g = traciManager->hosts.find("RSU0")->second->gate("dataIn");
            cMessage* msg = new cMessage("hello");


            sendDirect(msg, 0, 0, g->getOwnerModule(),g->getId());

        }

	}

}

void Wave1609_3::finish(){
    recordScalar("sendWaveCoordinNumber",sendWaveCoordinNum);
    recordScalar("receiveWaveCoordinNumber",receiveWaveCoordinNum);
    recordScalar("netwreceiveWaveFloodNum",netwreceiveWaveFloodNum);
    recordScalar("netwsendWaveFloodNum",netwsendWaveFloodNum);
    recordScalar("netwreceiveWaveData",netwreceiveWaveData);
    recordScalar("netwsendWaveData",netwsendWaveData);

    recordScalar("sendIpCoordinNumber",sendIpCoordinNum);
    recordScalar("receiveCoordinNumber",receiveIpCoordinNum);
    recordScalar("netwreceiveFloodNum",netwreceiveIpFloodNum);
    recordScalar("netwsendFloodNum",netwsendIpFloodNum);
    recordScalar("netwreceiveData",netwreceiveIpData);
    recordScalar("netwsendData",netwsendIpData);




    std::ofstream fout("rmc/output",std::ios::app);
//  fout<<myIpAddress<<":"<<endl;
    fout<<"sendWaveCoordinNum "<<"receiveWaveCoordinNum "<<"netwreceiveWaveFloodNum "
            <<"netwsendWaveFloodNum "<<"netwreceiveWaveData "<<"netwsendWaveData "
            <<"sendIpCoordinNum "<<"receiveIpCoordinNum "<<"netwreceiveIpFloodNum "
                        <<"netwsendIpFloodNum "<<"netwreceiveIpData "<<"netwsendIpData "<<endl;
    fout<<sendWaveCoordinNum<<" "<<receiveWaveCoordinNum<<" "<< netwreceiveWaveFloodNum<<" "
            <<netwsendWaveFloodNum <<" "<<netwreceiveWaveData<<" "<<netwsendWaveData<<" "
            <<sendIpCoordinNum<<" "<<receiveIpCoordinNum<<" "<< netwreceiveIpFloodNum<<" "
                        <<netwsendIpFloodNum <<" "<<netwreceiveIpData<<" "<<netwsendIpData<<endl;
    fout.close();


}

void Wave1609_3::handleLowerMsg(cMessage *msg){  //处理1609.4模块发来的消息
	EV << "== receive msg from 1609.4 layer ==" << endl;
//	EV << "1609.3: msg->getName() == " << msg->getName() << endl;

//	std::cout<< "1609.3msg" << sizeof(*msg) << endl;
	//cMessage* msgCopy = msg->dup();
	//sendUp(msg);

	ipExisted =false;//重置位置信息更新标志位，准备更新


	Mac80211Pkt* mac = dynamic_cast<Mac80211Pkt*>(msg);
	ASSERT(mac);


	cPacket* pac = mac->decapsulate();

	//判断收到的报文协议类型
	int tmprotocolId = -1;
	WaveShortMessage* wsm = dynamic_cast<WaveShortMessage*>(pac);
	Ipv6Pkt* ipkt = dynamic_cast<Ipv6Pkt*>(pac);
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
//	cPacket* pac = mac->getEncapsulatedMsg();
//	std::cout<< "1609.3pack" << sizeof(*pac) << endl;
//   Wave1609_3ToMac1609_4ControlInfo* addInfo = dynamic_cast<Wave1609_3ToMac1609_4ControlInfo*>(msg->getControlInfo());
//	assert(addInfo);
//	EV << "1609.3: wsm->getName()" << wsm->getName() << endl;
//	EV << "1609.3: wsm->getPacketType()" << wsm->getPacketType() << endl;
	//WaveShortMessage* wsmCopy = wsm->dup();
	//sendUp(wsmCopy);
	//const cObject* pCtrlInfo = NULL;
	//pCtrlInfo = wsm->removeControlInfo();
	//WaveShortMessage* wsmCopy = wsm->dup();
	//delete msg;
	//WaveShortMessage* wsm;
	//if ((wsm = dynamic_cast<WaveShortMessage*>(msg)) != NULL) {
	if(tmprotocolId == 0)
	{
	    if (wsm != NULL)
	    {
	        processWavePkt(wsm);
	    }
	    else
	    {
	        assert(false);
	    }
	}

	else if(tmprotocolId == 6)
	{
	    if(ipkt!=NULL)
	    {
	        processIpkt(ipkt);
	    }
	    else
	    {
	        assert(false);
	    }
	}
	else
	{
		 EV << "Wave1609_3: received unknown msg" << endl;
		 //delete msg;
	}
}

void Wave1609_3::handleUpperMsg(cMessage *msg){  //处理app模块发来的消息
	EV << "== receive msg from appl layer ==" << endl;

//	std::cout<<"net03recvup:"<<sizeof(*msg)<<endl;

//	BaseWaveApplLayerToWave1609_3ControlInfo* addInfo = dynamic_cast<BaseWaveApplLayerToWave1609_3ControlInfo*>(msg->getControlInfo());
//	int tmprotocolId = addInfo->protocolId;
//	msg->removeControlInfo();

//	std::cout<<"tmprotocolId"<<tmprotocolId<<endl;

	//判断收到的报文协议类型
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

	if(tmprotocolId == 0)
	{
//	    WaveShortMessage *wsm = dynamic_cast<WaveShortMessage*>(msg);
	        if (wsm != NULL)
	        {  //将基类类型的引用安全地转换为派生类类型的指针或引用
	            EV << "1609.3: wsm->getPacketType == " << wsm->getPacketType() << endl;
	            //EV << "1609.3: wsm->getPacketType == " << wsm->getPacketType() << endl;
	            myPosition = wsm->getSenderPos();
	            wsm->setSourceIp(myIpAddress);  //设置源IP,不使用appl的ID
	            wsm->setNextIp(myIpAddress);
	            if (wsm->getDesIp() == 0)  //广播报文
	            {
	                //Wave1609_3ToMac1609_4ControlInfo* addInfo1=new Wave1609_3ToMac1609_4ControlInfo(0);
	                //msg->setControlInfo(addInfo1);
	                sendDown(wsm);  //不加任何内容直接发往1609.4

	                //std::cout<<"net03senddown:"<<sizeof(*msg)<<endl;

	                netwsendWaveFloodNum++;
	            }
	            else  //数据
	            {
	                netwsendWaveData++;
	                switch(routeMethod)
	                {

	                 case 0:   waveRouting(wsm);
	                           break;
	                 case 1:   waveBroadcastRouting(wsm);
	                           break;
	                 default:break;
	                 }
	            }
	        }
	        else  //不是广播,也不是数据
	        {
	                assert(false);
	               // EV << "receive unknown msg" << endl;
	               // delete msg;
	        }
	}
	else if(tmprotocolId == 6)
	{
//	    Ipv6Pkt* ipkt = dynamic_cast<Ipv6Pkt*>(msg);

	    if(ipkt!=NULL)
	    {
	        //将基类类型的引用安全地转换为派生类类型的指针或引用
            EV << "1609.3: ipkt->getPacketType == " << ipkt->getIpv6PacketType() << endl;
            //EV << "1609.3: wsm->getPacketType == " << wsm->getPacketType() << endl;
            myPosition = ipkt->getSenderPos();
            ipkt->setSourceIp(myIpAddress);  //设置源IP,不使用appl的ID
            ipkt->setNextIp(myIpAddress);
            if (ipkt->getDesIp() == 0)  //广播报文
            {
                //Wave1609_3ToMac1609_4ControlInfo* addInfo1=new Wave1609_3ToMac1609_4ControlInfo(0);
                //msg->setControlInfo(addInfo1);
                sendDown(ipkt);  //不加任何内容直接发往1609.4

                //std::cout<<"net03senddown:"<<sizeof(*msg)<<endl;

                netwsendIpFloodNum++;
            }
            else  //数据
            {
                netwsendIpData++;
                switch(routeMethod)
                {

                 case 0:   ipRouting(ipkt);
                           break;
                 case 1:   ipBroadcastRouting(ipkt);
                           break;
                 default:break;
                 }
            }

	    }
	    else
	    {
	        assert(false);
	    }
	}

	else  //不是IP,也不是Wave
	{
		EV << "receive unknown msg" << endl;
		delete msg;
	}

}

void Wave1609_3::handleLowerControl(cMessage *msg){  //控制消息

}

void Wave1609_3::handleUpperControl(cMessage *msg){

}

void Wave1609_3::handleSelfMsg(cMessage *msg){  //处理自消息

    if(mobility->getExternalId()=="RSU0"&&(!strcmp(msg->getName(),"hello")))
    {
          std::cout<<"lala:"<<msg->getName()<<endl;
          EV<<"lala:"<<msg->getName()<<endl;
    }
	//if (msg == floodingPos){  //处理floodingPos消息

	//}
	//error("Wave1609_3 received unknown self-message");

}

void Wave1609_3::waveRouting(WaveShortMessage *wsm)
{
	EV << "Wave1609_3: wave routing..." << endl;
    srand(time(0));//设置时间种子，在随机选择协调节点的时候需要
    if (wsm != NULL)
    {
        if (wsm->getDesIp() == 0) //广播报文,正常情况下不会出现
        {
            error("route broadcast pack drop");
            delete wsm;
        }
        else
        {
            if (wsm->getDesIp() == myIpAddress)
            { //报文目的节点正好是本节点
                wsm->setHop(wsm->getHop()+1);
                sendUp(wsm); //将消息发往appl?
            }
            else
            {
                //查看报文TTL是否到期
                if(wsm->getTtl()>0)
                {
                    //报文下一跳正好是本节点

                    if(wsm->getNextIp()== myIpAddress)
                    {
                        //报文最终目的不是是本节点，需要路由，选择路由模式
                        if(wsm->getRouType() == 0) //贪心模式，wsm里面还要设置rouType
                        {
                            waveGreedyForward(wsm);
                        }
                        else
                        {
                            if (wsm->getRouType() == 1) //恢复模式
                            {
                                waveRecoveryForward(wsm);
                            }
                        }
                    }
                    else
                    {
                        //报文下一跳不是自己，丢掉报文
                        delete wsm;
                    }
                }
                //TTL到期，丢掉报文
                else
                {
                    delete wsm;
                }

             }
         }
     }
}

void Wave1609_3::ipRouting(Ipv6Pkt *ipkt)
{
    EV << "Wave1609_3: ip routing..." << endl;
    srand(time(0));//设置时间种子，在随机选择协调节点的时候需要
    if (ipkt != NULL)
    {
        if (ipkt->getDesIp() == 0) //广播报文,正常情况下不会出现
        {
            error("route broadcast pack drop");
            delete ipkt;
        }
        else
        {
            if (ipkt->getDesIp() == myIpAddress)
            { //报文目的节点正好是本节点
                ipkt->setHop(ipkt->getHop()+1);
                sendUp(ipkt); //将消息发往appl?
            }
            else
            {
                //查看报文TTL是否到期
                if(ipkt->getTtl()>0)
                {
                    //报文下一跳正好是本节点

                    if(ipkt->getNextIp()== myIpAddress)
                    {
                        //报文最终目的不是是本节点，需要路由，选择路由模式
                        if(ipkt->getRouType() == 0) //贪心模式，wsm里面还要设置rouType
                        {
                            ipGreedyForward(ipkt);
                        }
                        else
                        {
                            if (ipkt->getRouType() == 1) //恢复模式
                            {
                                ipRecoveryForward(ipkt);
                            }
                        }
                    }
                    else
                    {
                        //报文下一跳不是自己，丢掉报文
                        delete ipkt;
                    }
                }
                //TTL到期，丢掉报文
                else
                {
                    delete ipkt;
                }

             }
         }
     }
}

//wave贪心路由
bool Wave1609_3::waveGreedyForward(WaveShortMessage *wsm)
{
    //取出路由需要的信息
    int lastip = wsm->getLastIp();
    int destip = wsm->getDesIp();
    //查找目的节点是否在邻居中
    std::list<struct table>::iterator Iterlis = neighborInfo.begin();
    for (; Iterlis != neighborInfo.end(); Iterlis++)
    {
        if (Iterlis->ip == destip)
        {
            //邻居中有目的节点，修改上一跳和下一跳ip后，直接投递给目的节点
            wsm->setLastIp(myIpAddress);
            wsm->setNextIp(destip);
            wsm->setTtl(wsm->getTtl() - 1);
            wsm->setHop((wsm->getHop()) + 1);  //hop加1

//修改控制信息
//            cObject* pCtrlInfo = wsm->removeControlInfo();
//            setDownControlInfo(wsm, arp->getMacAddr(destip));

            sendDown(wsm);
            return true;
        }
    }

    //if (Iterlis == neighborInfo.end())  改了？？？
    //{ //目的节点不是邻居，投递给邻居
    Coord desposition;
    bool flag = getIpPosition(destip, desposition, nodePosition);  //查的目的ip对应的position??
    if (!flag)
    {
        if(wsm->getSourceIp()==myIpAddress)
        {
            error("diff in two layer");
        }
        else
        {
            EV<<"media node can find ip"<<endl;
        }
        //error("cannot find ip position");
        return false;
    }
    int nextip = selectNeighbor(lastip, destip, desposition);  //选择下一跳
    if(nextip == myIpAddress)
    {
            //陷入局部最优
        return waveRecoveryForward(wsm);

    }

//修改控制信息
//      int nextHopMacAddr = arp->getMacAddr(nexthip);
//      cObject* pCtrlInfo = wsm->removeControlInfo();
//      setDownControlInfo(wsm, nextHopMacAddr);
    wsm->setLastIp(myIpAddress);
    wsm->setNextIp(nextip);
    wsm->setTtl(wsm->getTtl() - 1);
    wsm->setHop((wsm->getHop())+1);  //hop加1
    sendDown(wsm);
    netwsendWaveData++;
    return true;
    //}
}

//ip贪心路由
bool Wave1609_3::ipGreedyForward(Ipv6Pkt *ipkt)
{
    //取出路由需要的信息
    int lastip = ipkt->getLastIp();
    int destip = ipkt->getDesIp();
    //查找目的节点是否在邻居中
    std::list<struct table>::iterator Iterlis = neighborInfo.begin();
    for (; Iterlis != neighborInfo.end(); Iterlis++)
    {
        if (Iterlis->ip == destip)
        {
            //邻居中有目的节点，修改上一跳和下一跳ip后，直接投递给目的节点
            ipkt->setLastIp(myIpAddress);
            ipkt->setNextIp(destip);
            ipkt->setTtl(ipkt->getTtl() - 1);
            ipkt->setHop((ipkt->getHop()) + 1);  //hop加1

//修改控制信息
//            cObject* pCtrlInfo = wsm->removeControlInfo();
//            setDownControlInfo(wsm, arp->getMacAddr(destip));

            sendDown(ipkt);
            return true;
        }
    }

    //if (Iterlis == neighborInfo.end())  改了？？？
    //{ //目的节点不是邻居，投递给邻居
    Coord desposition;
    bool flag = getIpPosition(destip, desposition, nodePosition);  //查的目的ip对应的position??
    if (!flag)
    {
        if(ipkt->getSourceIp()==myIpAddress)
        {
            error("diff in two layer");
        }
        else
        {
            EV<<"media node can find ip"<<endl;
        }
        //error("cannot find ip position");
        return false;
    }
    int nextip = selectNeighbor(lastip, destip, desposition);  //选择下一跳
    if(nextip == myIpAddress)
    {
            //陷入局部最优
        return ipRecoveryForward(ipkt);

    }

//修改控制信息
//      int nextHopMacAddr = arp->getMacAddr(nexthip);
//      cObject* pCtrlInfo = wsm->removeControlInfo();
//      setDownControlInfo(wsm, nextHopMacAddr);
    ipkt->setLastIp(myIpAddress);
    ipkt->setNextIp(nextip);
    ipkt->setTtl(ipkt->getTtl() - 1);
    ipkt->setHop((ipkt->getHop())+1);  //hop加1
    sendDown(ipkt);
    netwsendIpData++;
    return true;
    //}
}
//选择下一跳
int Wave1609_3::selectNeighbor(int &lastip, int &destip, Coord &desposition)
{
    double nowdistance = myPosition.distance(desposition);//本节点到目的节点的距离
    double mindistance = nowdistance;//邻居到目的节点的最短距离
    double tmpdistance;
    int minnode = myIpAddress;//到目的最近邻居ip
    std::vector<struct table> tempcoordVector;
    Coord tmpcoord;

    //协调节点表是否有匹配,先查找协调节点表。。。
    std::list<struct table>::iterator Iterlis = coordinateInfo.begin();
    for (; Iterlis != coordinateInfo.end(); Iterlis++)
    {
        tmpcoord = Iterlis->position;
        if(tmpcoord.distance(desposition) < mindistance)  //mydistance??
        {
            tempcoordVector.push_back(*Iterlis);
        }

    }
    if(!tempcoordVector.empty())  //既然随机选，干脆选第一个。。。。
    {
        //随机选择一个协调节点投递
        if(tempcoordVector.size()>1)
        {
            int position = rand()%(tempcoordVector.size());
            return tempcoordVector[position].ip;  //????list不能这么用啊。。不能随机访问

        }
        else
            return tempcoordVector.begin()->ip;
    }
    else
    {
        //协调节点表无匹配，查找邻居节点
        Iterlis = neighborInfo.begin();
        for (; Iterlis != neighborInfo.end(); Iterlis++)
        {
            tmpcoord = Iterlis->position;
            tmpdistance = tmpcoord.distance(desposition);
            if(tmpdistance < mindistance)
            {
                mindistance =  tmpdistance;
                minnode = Iterlis->ip;
            }
        }
        if(minnode==myIpAddress)
        {
            //陷入局部最优
            return myIpAddress;
        }
        else
        {
            return minnode;
        }
    }
}

//通过ip查找位置信息
bool Wave1609_3::getIpPosition(int ip, Coord & desposition,std::list<struct table> & tablelist)
{
    if (tablelist.empty())
    {
        std::cerr << "search empty table" << endl;
        return false;
    }
    std::list<struct table>::iterator Iterlist = tablelist.begin();
    for (; Iterlist != tablelist.end(); Iterlist++)
    {
        if (Iterlist->ip == ip)
        {
            desposition = Iterlist->position;
            return true;  //找到
        }
    }
    //if (Iterlist == tablelist.end())
    //{
        return false;
    //}
}

//判断是否是协调节点
bool Wave1609_3::coordJudge(std::list <struct table> &tablelist)
{
    if(tablelist.size()==1||tablelist.empty())
    {
        return false;
    }
    double aver_x=0,aver_y=0;
    double co_xy=0,co_x=0,co_y=0;
    //double co_node;
    std::list <struct table>:: iterator Iterlis = tablelist.begin();
    for(;Iterlis!=tablelist.end();Iterlis++)
    {
        aver_x += Iterlis->position.x;
        aver_y += Iterlis->position.y;
    }
    aver_x = aver_x/tablelist.size();
    aver_y = aver_y/tablelist.size();
    Iterlis = tablelist.begin();
    for(;Iterlis!=tablelist.end();Iterlis++)
    {
        co_xy  += (Iterlis->position.x-aver_x)*(Iterlis->position.y-aver_y);
        co_x += (Iterlis->position.x-aver_x)*(Iterlis->position.x-aver_x);
        co_y += (Iterlis->position.y-aver_y)*(Iterlis->position.y-aver_y);
     }

//    std::cout<<"pre x:"<<co_x<<endl;
//    std::cout<<"pre y:"<<co_y<<endl;
     co_x = sqrt(co_x);
     co_y = sqrt(co_y);
     if(co_xy<0)
     {
         co_xy = (double)(-1)*co_xy;
     }
     if(co_x==0 || co_y ==0)
     {
         return false;
     }
     double co_node = (co_xy)/(co_x*co_y);

//     std::cout<<"xy:"<<co_xy<<endl;
//    std::cout<<"x:"<<co_x<<endl;
//     std::cout<<"y:"<<co_y<<endl;
//     std::cout<<"co:"<<co_node<<endl;

     if(co_node<0.5)
     {
         return true;
     }
     return false;


}

//wave恢复模式路由
bool Wave1609_3::waveRecoveryForward(WaveShortMessage *wsm)
{
    return true;
}

//ip恢复模式路由
bool Wave1609_3::ipRecoveryForward(Ipv6Pkt *ipkt)
{
    return true;
}

//wave广播路由算法
void Wave1609_3::waveBroadcastRouting(WaveShortMessage *wsm)
{
    EV << "wave broadcasting routing..." << endl;
//  WaveShortMessage *wsm;
    if (wsm != NULL)
    {
           if (wsm->getDesIp() == myIpAddress)
           { //报文目的节点正好是本节点
                wsm->setHop(wsm->getHop() + 1);
                sendUp(wsm); //将消息发往appl?
           }
           else
           {
                 //查看报文TTL是否到期
                 if(wsm->getTtl()>0)
                 {
                     wsm->setTtl(wsm->getTtl()-1);
                     wsm->setHop(wsm->getHop() + 1);
                     sendDown(wsm);
                     netwreceiveWaveData++;
                 }
                 else
                 {
                     delete wsm;
                 }
           }
    }

}

//ip广播路由算法
void Wave1609_3::ipBroadcastRouting(Ipv6Pkt *ipkt)
{
    EV << "ip broadcasting routing..." << endl;
//  WaveShortMessage *wsm;
    if (ipkt != NULL)
    {
           if (ipkt->getDesIp() == myIpAddress)
           { //报文目的节点正好是本节点
               ipkt->setHop(ipkt->getHop() + 1);
                sendUp(ipkt); //将消息发往appl?
           }
           else
           {
                 //查看报文TTL是否到期
                 if(ipkt->getTtl()>0)
                 {
                     ipkt->setTtl(ipkt->getTtl()-1);
                     ipkt->setHop(ipkt->getHop() + 1);
                     sendDown(ipkt);
                     netwreceiveIpData++;
                 }
                 else
                 {
                     delete ipkt;
                 }
           }
    }

}
void Wave1609_3::processWavePkt(WaveShortMessage *wsm)
{
    EV << "heiiiiiiiiiiiiiiiiiii....wave" << endl;

    if ((wsm->getDesIp() == 0))
    { //广播
        if (wsm->getSourceIp() == myIpAddress)  //源IP是自己直接丢掉。。
        {
           delete wsm;
           //return;
        }
        else  //源ip不等于myIpAddress
        {
               //cMessage *msgCopy = msg->dup();
               //sendUp(msgCopy);
              if (wsm->getPacketType() == WPOSITION)  //位置信息
              {
                     EV << "receive wave position information..." << endl;
                     netwreceiveWaveFloodNum++;
                     //WaveShortMessage* wsmCopy = wsm->dup();
                     //sendUp(wsmCopy);
                     tempStruct.ip = wsm->getSourceIp();  //＝＝senderAddress
                     tempStruct.position = wsm->getSenderPos();  //节点的位置坐标
                     tempStruct.count = wsm->getPacketNum();  //获取包编号
                     //此后最好不要修改tempStruct里面的值

                     std::list<struct table>::iterator itr1 = nodePosition.begin();//itr1指向位置表
                     for (; itr1 != nodePosition.end(); itr1++)
                     {
                          if (itr1->ip == tempStruct.ip)
                          {
                             if( itr1->count < tempStruct.count)
                             {
                                  //需要更新
                                 ipExisted = true;
                                 itr1->position = tempStruct.position;  //更新位置信息
                                 itr1->count = tempStruct.count;  //更新count
                                 WaveShortMessage* wsmCopy2 = wsm->dup();
                                 sendUp(wsmCopy2);
                                  break;//跳出for
                              }
                              else
                              {
                                  //找到但不需要更新
                                  break;//跳出for
                              }

                          }
                     }

                     if (itr1 == nodePosition.end())  //没有找到对应的ip,如果ip不在list中，则将ip及其对应的位置信息push_back到list中
                     {
                         nodePosition.push_back(tempStruct);  //将节点的信息放入list
                         WaveShortMessage* wsmCopy2 = wsm->dup();
                         sendUp(wsmCopy2);
                         ipExisted = true;  //重置ipExisted
                      }
                      if(wsm->getGpsFlag()==1)//是邻居发来的位置信息，同时需要更新邻居表;
                      {
                          EV << "receive  wave neighbor's information..." << endl;
                          std::list<struct table>::iterator itr2 = neighborInfo.begin();
                          for (; itr2 != neighborInfo.end(); itr2++)
                          {
                               if (itr2->ip == tempStruct.ip)  //ip已在list中，只需要更新位置信息即可
                              {
                                  //ipExisted = true;
                                  itr2->position = tempStruct.position;  //更新位置信息
                                  itr2->count = 0;//同时count置0
                                  break;  //跳出for循环
                               }
                          }
                          if (itr2 == neighborInfo.end())  //如果ip不在list中，则将ip及其对应的位置信息push_back到list中
                          {

                                neighborInfo.push_back(tempStruct);  //将节点的信息放入list
                                neighborInfo.rbegin()->count = 0;//新加入的邻居表相count值要置0;
                                //ipExisted = false;  //重置ipExisted
                          }

                          //删除超时表项
                          int tmpcount1;
                          for (itr2 = neighborInfo.begin();itr2 != neighborInfo.end();)
                          {
                                tmpcount1 = itr2->count;
                                tmpcount1++;
                                if(tmpcount1 >=maxCount)
                                {

                                      itr2=neighborInfo.erase(itr2);
                                }
                                else
                                {
                                      itr2->count = tmpcount1;
                                      itr2++;
                                 }
                           }

                           //协调节点判断
                           bool flag = coordJudge(neighborInfo);
                           if(flag)
                           {
                                //生成协调节点信息广播给邻居
                                 WaveShortMessage* wsmCopy = wsm->dup();
                                 wsmCopy->setPacketType(WCOORDINATE_INFO);
                                 wsmCopy->setSenderAddress(myIpAddress);
                                 wsmCopy->setSourceIp(myIpAddress);
                                 wsmCopy->setSenderPos(myPosition);
                                 wsmCopy->setTtl(1);
                                 //sendDown(msg);
                                 sendDown(wsmCopy);
                                 sendWaveCoordinNum++;

                            }

                            wsm->setGpsFlag(0);//修改标志位代表以后这个包仅为非邻居位置信息
                        }
                        int ttl = wsm->getTtl();
                        if (ttl > 0)
                        {  //检查ttl, 如果tt>0,继续广播

                            if(ipExisted == true)//只有更新了位置信息的包才继续转发
                            {
                                 wsm->setTtl(--ttl);  //TTL减1
                                 sendDown(wsm);  //发往1609.4,继续广播位置信息。
                                 netwsendWaveFloodNum++;
                             }
                         }
                         else
                         {
                             delete wsm;
                         }


              }



              else
              {
                    if((wsm->getPacketType() == WCOORDINATE_INFO)) //协调节点信息，协调节点的判断
                    {
                         //协调节点信息
                         if (wsm->getSourceIp() == myIpAddress)  //源IP是自己,不可能发生这种情况，前面应该丢掉
                         {
                               //std::cout<<"?"<<endl;
                               assert(false);
                         }
                          EV << "receive wave coordinate's information..." << endl;
                          receiveWaveCoordinNum++;
                          tempStruct.ip = wsm->getSourceIp();  //＝＝senderAddress
                          tempStruct.position = wsm->getSenderPos();  //节点的位置坐标

                          std::list<struct table>::iterator itr = coordinateInfo.begin();
                          for (; itr != coordinateInfo.end(); itr++)
                          {
                              if (itr->ip == tempStruct.ip)  //ip已在list中，只需要更新位置信息即可
                              {
                                  //ipExisted = true;
                                  itr->position = tempStruct.position;  //更新位置信息
                                  itr->count = 0;//同时count置0
                                  break;
                               }
                           }
                           if (itr == coordinateInfo.end())  //如果ip不在list中，则将ip及其对应的位置信息push_back到list中
                           {
                               tempStruct.count = 0;//新加入的协调节点表相count值要置0;
                               coordinateInfo.push_back(tempStruct);  //将节点的信息放入list
                               //ipExisted = false;  //重置ipExisted
                           }
                           //删除超时表项
                           int tmpcount;
                           for (itr = coordinateInfo.begin();itr != coordinateInfo.end();)
                           {
                               tmpcount = itr->count;
                               tmpcount++;
                               if(tmpcount++ >= maxCount)
                               {
                                    itr=coordinateInfo.erase(itr);
                               }
                               else
                               {
                                    itr->count = tmpcount;
                                    itr++;
                                }
                            }

                    }
                    else
                    {
                         //不可能运行到这里，因为没有其他类型的广播包了
                         //std::cout<<"??"<<endl;
                         assert(false);
                    }

               }

         }

    }

    else
    {
            EV << "Wave1609_3: received wave data msg" << endl;
            netwreceiveWaveData++;
            //delete msg;
            switch(routeMethod)
            {

                case 0:   waveRouting(wsm);
                          break;
                case 1:   waveBroadcastRouting(wsm);
                          break;
                default:break;
            }

     }

}


void Wave1609_3::processIpkt(Ipv6Pkt *ipkt)
{
    EV << "heiiiiiiiiiiiiiiiiiii....ip" << endl;

    EV << "yyyyyyyyyyyyyyyyyyyy:"<<ipkt->getSourceIp()<<endl;
    if ((ipkt->getDesIp() == 0))
    { //广播ipkt
        if(ipkt->getSourceIp() == myIpAddress)  //源IP是自己直接丢掉。。
        {
           delete ipkt;
           //return;
        }
        else  //源ip不等于myIpAddress
        {
               //cMessage *msgCopy = msg->dup();
               //sendUp(msgCopy);
              if (ipkt->getIpv6PacketType() == IPOSITION)  //位置信息
              {
                     EV << "receive ip position information..." << endl;
                     netwreceiveIpFloodNum++;
                     //WaveShortMessage* wsmCopy = wsm->dup();
                     //sendUp(wsmCopy);
                     tempStruct.ip = ipkt->getSourceIp();  //＝＝senderAddress
                     tempStruct.position = ipkt->getSenderPos();  //节点的位置坐标
                     tempStruct.count = ipkt->getPacketNum();  //获取包编号
                     //此后最好不要修改tempStruct里面的值

                     std::list<struct table>::iterator itr1 = nodePosition.begin();//itr1指向位置表
                     for (; itr1 != nodePosition.end(); itr1++)
                     {
                          if (itr1->ip == tempStruct.ip)
                          {
                             if( itr1->count < tempStruct.count)
                             {
                                  //需要更新
                                 ipExisted = true;
                                 itr1->position = tempStruct.position;  //更新位置信息
                                 itr1->count = tempStruct.count;  //更新count
                                 Ipv6Pkt* ipktCopy2 = ipkt->dup();
                                 sendUp(ipktCopy2);
                                  break;//跳出for
                              }
                              else
                              {
                                  //找到但不需要更新
                                  break;//跳出for
                              }

                          }
                     }

                     if (itr1 == nodePosition.end())  //没有找到对应的ip,如果ip不在list中，则将ip及其对应的位置信息push_back到list中
                     {
                         nodePosition.push_back(tempStruct);  //将节点的信息放入list
                         Ipv6Pkt* ipktCopy2 = ipkt->dup();
                         sendUp(ipktCopy2);
                         ipExisted = true;  //重置ipExisted
                      }
                      if(ipkt->getGpsFlag()==1)//是邻居发来的位置信息，同时需要更新邻居表;
                      {
                          EV << "receive ip neighbor's information..." << endl;
                          std::list<struct table>::iterator itr2 = neighborInfo.begin();
                          for (; itr2 != neighborInfo.end(); itr2++)
                          {
                               if (itr2->ip == tempStruct.ip)  //ip已在list中，只需要更新位置信息即可
                              {
                                  //ipExisted = true;
                                  itr2->position = tempStruct.position;  //更新位置信息
                                  itr2->count = 0;//同时count置0
                                  break;  //跳出for循环
                               }
                          }
                          if (itr2 == neighborInfo.end())  //如果ip不在list中，则将ip及其对应的位置信息push_back到list中
                          {

                                neighborInfo.push_back(tempStruct);  //将节点的信息放入list
                                neighborInfo.rbegin()->count = 0;//新加入的邻居表相count值要置0;
                                //ipExisted = false;  //重置ipExisted
                          }

                          //删除超时表项
                          int tmpcount1;
                          for (itr2 = neighborInfo.begin();itr2 != neighborInfo.end();)
                          {
                                tmpcount1 = itr2->count;
                                tmpcount1++;
                                if(tmpcount1 >=maxCount)
                                {

                                      itr2=neighborInfo.erase(itr2);
                                }
                                else
                                {
                                      itr2->count = tmpcount1;
                                      itr2++;
                                 }
                           }

                           //协调节点判断
                           bool flag = coordJudge(neighborInfo);
                           if(flag)
                           {
                                //生成协调节点信息广播给邻居
                                 Ipv6Pkt* ipktCopy = ipkt->dup();
                                 ipktCopy->setIpv6PacketType(ICOORDINATE_INFO);
                                 ipktCopy->setSenderAddress(myIpAddress);
                                 ipktCopy->setSourceIp(myIpAddress);
                                 ipktCopy->setSenderPos(myPosition);
                                 ipktCopy->setTtl(1);
                                 //sendDown(msg);
                                 sendDown(ipktCopy);
                                 sendIpCoordinNum++;

                            }

                           ipkt->setGpsFlag(0);//修改标志位代表以后这个包仅为非邻居位置信息
                        }
                        int ttl = ipkt->getTtl();
                        if (ttl > 0)
                        {  //检查ttl, 如果tt>0,继续广播

                            if(ipExisted == true)//只有更新了位置信息的包才继续转发
                            {
                                ipkt->setTtl(--ttl);  //TTL减1
                                 sendDown(ipkt);  //发往1609.4,继续广播位置信息。
                                 netwsendIpFloodNum++;
                             }
                         }
                         else
                         {
                             delete ipkt;
                         }


              }



              else
              {
                    if((ipkt->getIpv6PacketType() == ICOORDINATE_INFO)) //协调节点信息，协调节点的判断
                    {
                         //协调节点信息
                         if (ipkt->getSourceIp() == myIpAddress)  //源IP是自己,不可能发生这种情况，前面应该丢掉
                         {
                               //std::cout<<"?"<<endl;
                               assert(false);
                         }
                          EV << "receive ip coordinate's information..." << endl;
                          receiveIpCoordinNum++;
                          tempStruct.ip = ipkt->getSourceIp();  //＝＝senderAddress
                          tempStruct.position = ipkt->getSenderPos();  //节点的位置坐标

                          std::list<struct table>::iterator itr = coordinateInfo.begin();
                          for (; itr != coordinateInfo.end(); itr++)
                          {
                              if (itr->ip == tempStruct.ip)  //ip已在list中，只需要更新位置信息即可
                              {
                                  //ipExisted = true;
                                  itr->position = tempStruct.position;  //更新位置信息
                                  itr->count = 0;//同时count置0
                                  break;
                               }
                           }
                           if (itr == coordinateInfo.end())  //如果ip不在list中，则将ip及其对应的位置信息push_back到list中
                           {
                               tempStruct.count = 0;//新加入的协调节点表相count值要置0;
                               coordinateInfo.push_back(tempStruct);  //将节点的信息放入list
                               //ipExisted = false;  //重置ipExisted
                           }
                           //删除超时表项
                           int tmpcount;
                           for (itr = coordinateInfo.begin();itr != coordinateInfo.end();)
                           {
                               tmpcount = itr->count;
                               tmpcount++;
                               if(tmpcount++ >= maxCount)
                               {
                                    itr=coordinateInfo.erase(itr);
                               }
                               else
                               {
                                    itr->count = tmpcount;
                                    itr++;
                                }
                            }

                    }
                    else
                    {
                         //不可能运行到这里，因为没有其他类型的广播包了
                         //std::cout<<"??"<<endl;
                         assert(false);
                    }

               }

         }

    }

    else
    {
            EV << "Wave1609_3: received ip data msg" << endl;
            netwreceiveIpData++;
            //delete msg;
            switch(routeMethod)
            {

                case 0:   ipRouting(ipkt);
                          break;
                case 1:   ipBroadcastRouting(ipkt);
                          break;
                default:break;
             }

     }

}


