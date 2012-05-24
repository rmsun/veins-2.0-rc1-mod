/* author:rmc
 * date:2012.4.21
 */

#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<stdlib.h>
#include<vector>
#include<map>

using namespace std;

int main(int argc,char* argv[])
{
	map<int,vector<double> > collectmap;
	vector<double> tempvec;
	ifstream fin;
	fin.open("output");
	string line;
	while(getline(fin,line))//每次循环读取一个节点的三组信息
	{
		istringstream data(line);
		string pury;
		
		int i=0;
		while(data>>pury)
		{
			i++;
		}
		if(i==4)//读取TestApp层的信息
		{
			if(tempvec.empty())
			{
			    string ip,apprecvdata,delay,hop;
			    getline(fin,line);
			    istringstream tempdata(line);
			    tempdata>>ip>>apprecvdata>>delay>>hop;
			    tempvec.push_back(atof(ip.c_str()));
			    tempvec.push_back(atof(apprecvdata.c_str()));
			    tempvec.push_back(atof(delay.c_str()));
			    tempvec.push_back(atof(hop.c_str()));
			}
			else
			{
			    perror("read error1!");
			    cin.get();
			    exit(0);
			}
			
		}
		else if(i==9)//读取BaseApp层的信息
		{
		    string swdata,swflood,rwflood,sidata,siflood,riflood,recvipdata,ipdatadelay,ipdatahop;
		    getline(fin,line);
		    istringstream tempdata(line);
		    tempdata>>swdata>>swflood>>rwflood>>sidata>>siflood>>riflood>>recvipdata>>ipdatadelay>>ipdatahop;
		    tempvec.push_back(atof(swdata.c_str()));
		    tempvec.push_back(atof(swflood.c_str()));
		    tempvec.push_back(atof(rwflood.c_str()));
		    tempvec.push_back(atof(sidata.c_str()));
		    tempvec.push_back(atof(siflood.c_str()));
		    tempvec.push_back(atof(riflood.c_str()));
		    tempvec.push_back(atof(recvipdata.c_str()));
		    tempvec.push_back(atof(ipdatadelay.c_str()));
		    tempvec.push_back(atof(ipdatahop.c_str()));
		}
		else
		{
		    if(i==12)//读取wave1609.3层的信息
		    {
		        string swavecoord,rwavecoord,wrwaveflood,wswaveflood,wrwavedata,wswavedata;
		        string sipcoord,ripcoord,wripflood,wsipflood,wripdata,wsipdata;
		        getline(fin,line);
		        istringstream tempdata(line);
		        tempdata>>swavecoord>>rwavecoord>>wrwaveflood>>wswaveflood>>wrwavedata>>wswavedata
		        >>sipcoord>>ripcoord>>wripflood>>wsipflood>>wripdata>>wsipdata;
		        tempvec.push_back(atof(swavecoord.c_str()));
		        tempvec.push_back(atof(rwavecoord.c_str()));
		        tempvec.push_back(atof(wrwaveflood.c_str()));
		        tempvec.push_back(atof(wswaveflood.c_str()));
		        tempvec.push_back(atof(wrwavedata.c_str()));
		        tempvec.push_back(atof(wswavedata.c_str()));

		        tempvec.push_back(atof(sipcoord.c_str()));
		        tempvec.push_back(atof(ripcoord.c_str()));
		        tempvec.push_back(atof(wripflood.c_str()));
		        tempvec.push_back(atof(wsipflood.c_str()));
		        tempvec.push_back(atof(wripdata.c_str()));
		        tempvec.push_back(atof(wsipdata.c_str()));

		        //finish one node
		        int name = tempvec[0];
		        collectmap.insert(make_pair(name,tempvec));
		        tempvec.clear();
		    }
		    else
		    {
		        perror("read error2!");
		        cin.get();
		        exit(0);
		    }
		}
	}

	//统计
	int nodenum = collectmap.size();
	cout<<"nodenum:"<<nodenum<<endl;

//	double statsReceivedData=0;
//	double statsDelayTime=0;
//	double statsHop=0;
//	double sendDataNum=0;
//	double sendFloodNum=0;
//	double receiveFloodNum=0;
//	double sendCoordinNum=0;
//	double receiveCoordinNum=0;
//	double netwreceiveFloodNum=0;
//	double netwsendFloodNum=0;
//	double netwreceiveData=0;
//	double netwsendData=0;
//	0statsReceivedwaveData 1statswaveDelayTime 2statswaveHop
//	3sendWaveDataNum 4sendWaveFloodNum 5receiveWaveFloodNum
//	6sendIpDataNum 7sendIpFloodNum 8receiveIpFloodNum 9statsReceivedIpData 10statsIpDelayTime 11statsIpHop
//	12sendWaveCoordinNum 13receiveWaveCoordinNum 14netwreceiveWaveFloodNum 15netwsendWaveFloodNum
//	16netwreceiveWaveData 17netwsendWaveData 18sendIpCoordinNum 19receiveIpCoordinNum 20netwreceiveIpFloodNum
//	21netwsendIpFloodNum 22netwreceiveIpData 23netwsendIpData


	map<int,vector<double> >::iterator mapIter= collectmap.begin();
	int num = mapIter->second.size();
	vector<double> collectvec;
	for(int i=1;i<num;i++)
	{
	    mapIter = collectmap.begin();
	    double sum = 0;
	    for(;mapIter!=collectmap.end();mapIter++)
	    {
	        sum += mapIter->second[i];
	    }
	    collectvec.push_back(sum);
	}

	for(int i=0;i<collectvec.size();i++)
	{
	    cout<<"No."<<i+1<<":"<<collectvec[i]<<endl;
	}

	ofstream fout;
	fout.open("total");
	fout<<"Nodenumber:"<<nodenum<<endl;
	fout<<"AppLayer Send Wave DataNumber:"<<collectvec[3]<<endl;
	fout<<"AppLayer Receive Wave DataNumber:"<<collectvec[0]<<endl;
	fout<<"AppLayer Send Wave PositionNumber:"<<collectvec[4]<<endl;
	fout<<"AppLayer Receive Wave PositionNumber:"<<collectvec[5]<<endl;
	fout<<"NetworkLayer Send Wave DataNumber:"<<collectvec[17]<<endl;
	fout<<"NetworkLayer Receive Wave DataNumber:"<<collectvec[16]<<endl;
	fout<<"NetworkLayer Send Wave PositionNumber:"<<collectvec[15]<<endl;
	fout<<"NetworkLayer Receive Wave PositionNumber:"<<collectvec[14]<<endl;

	fout<<"AppLayer Send Ip DataNumber:"<<collectvec[6]<<endl;
	fout<<"AppLayer Receive Ip DataNumber:"<<collectvec[9]<<endl;
	fout<<"AppLayer Send Ip PositionNumber:"<<collectvec[7]<<endl;
	fout<<"AppLayer Receive Ip PositionNumber:"<<collectvec[8]<<endl;
	fout<<"NetworkLayer Send Ip DataNumber:"<<collectvec[23]<<endl;
	fout<<"NetworkLayer Receive Ip DataNumber:"<<collectvec[22]<<endl;
	fout<<"NetworkLayer Send Ip PositionNumber:"<<collectvec[21]<<endl;
	fout<<"NetworkLayer Receive Ip PositionNumber:"<<collectvec[20]<<endl;

	fout<<"Averyge Wave Data Delay:"<<(collectvec[1]/collectvec[0])<<endl;
	fout<<"Averyge Wave Data Hop:"<<(collectvec[2]/collectvec[0])<<endl;
	fout<<"success Wave data rate:"<<(collectvec[0]/collectvec[3])<<endl;

	fout<<"Averyge IP Data Delay:"<<(collectvec[10]/collectvec[9])<<endl;
	fout<<"Averyge IP Data Hop:"<<(collectvec[11]/collectvec[9])<<endl;
	fout<<"success IP data rate:"<<(collectvec[9]/collectvec[6])<<endl;

	fout<<"Averyge Data Delay:"<<((collectvec[1]+collectvec[10])/(collectvec[0]+collectvec[9]))<<endl;
	fout<<"Averyge Data Hop:"<<((collectvec[2]+collectvec[11])/(collectvec[0]+collectvec[9]))<<endl;
	fout<<"Success Data Rate:"<<((collectvec[0]+collectvec[9])/(collectvec[3]+collectvec[6]))<<endl;




	fout.close();


	 	
}


