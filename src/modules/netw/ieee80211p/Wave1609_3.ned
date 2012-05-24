//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

package org.mixim.modules.netw.ieee80211p;

simple Wave1609_3
{
    parameters:
        @class(Wave1609_3);
        int routeMethod = default(0);//新增路由算法选项
    gates:
    	output upperLayerOut;   // to upper layer
        output upperControlOut; // control to upper layer
        output lowerControlOut; // control to lower layer
        output lowerLayerOut;   // to lower layer
        input lowerLayerIn;     // from lower layer
        input lowerControlIn;   // control from lower layer
        input upperControlIn;
        input upperLayerIn;
        
        input dataIn@direct;
        

}