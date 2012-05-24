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

enum Ipv6PacketType
{
    IPOSITION = 0;
 //   NEIGHBOR = 1暂时不用了
    ICOORDINATE_INFO  = 1;
    IDATA = 2;
};

packet Ipv6Pkt {
	//Version of the IP
	int Version = 6;
	//报文类型标志
	int Ipv6PacketType @enum(Ipv6PacketType);
	//Channel Number on which this packet was sent
	int channelNumber;
	
	
	int TraClass = 0;
	int FlowLabel = 0;
	int PayloadLength = 0;
	
	
	int senderAddress = 0;
	int recipientAddress = -1;
//	int serial = 0;
	Coord senderPos;
	simtime_t timestamp = 0;

	
	//以下为自己增加的
	int sourceIp;
    int desIp;
    int  lastIp;
    int nextIp;  //新增用于路由
    int rouType;  //路由模式：0为贪心模式，1为恢复模式
    int ttl;//相当于Ipv6中的hoplimit
    int packetNum;
    int dataLength;
    string data;
    int hop;
    int gpsFlag; //1代表为邻居消息，0代表为位置消息
}
