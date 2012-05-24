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

cplusplus {{
#include <Coord.h>
}}

class noncobject Coord;

enum PacketType
{
    WPOSITION = 0;
 //   NEIGHBOR = 1暂时不用了
    WCOORDINATE_INFO  = 1;
    WDATA = 2;
};

//struct route_msg
//{
    //string sourceIp;
    //string desIp;
    //string lastIp;
    //int ttl;
    //int packetNum;
    //int packetLength;
    //string data;
//};

packet WaveShortMessage {
	//Version of the Wave Short Message
	int wsmVersion = 0;
	//报文类型标志
	int packetType @enum(PacketType);
	//Determine which security mechanism was used
	int securityType = 0;
	//Channel Number on which this packet was sent
	int channelNumber;
	//Data rate with which this packet was sent
	int dataRate = 1;
	//Power Level with which this packet was sent
	int priority = 3;
	//Unique number to identify the service
	int psid = 0;
	//Provider Service Context
	string psc = "Service with some Data";
	//Length of Wave Short Message
	int wsmLength;
	//Data of Wave Short Message
	string wsmData = "Some Data";
	//struct route_msg routeMsg;

	int senderAddress = 0;
	int recipientAddress = -1;
	int serial = 0;
	Coord senderPos;
	simtime_t timestamp = 0;
	
	//以下为自己增加的
	int sourceIp;
    int desIp;
    int  lastIp;
    int nextIp;  //新增用于路由
    int rouType;  //路由模式：0为贪心模式，1为恢复模式
    int ttl;
    int packetNum;
    int dataLength;
    string data;
    int hop;
    int gpsFlag; //1代表为邻居消息，0代表为位置消息
}
