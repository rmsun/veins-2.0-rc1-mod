//
// Author: Emin Ilker Cetinbas (niw3_at_yahoo_d0t_com)
// Copyright (C) 2005 Emin Ilker Cetinbas
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//

#include "LinearMobility.h"

#include <FWMath.h>


Define_Module(LinearMobility);

void LinearMobility::initialize(int stage)
{
    BaseMobility::initialize(stage);

    debugEV << "initializing LinearMobility stage " << stage << endl;

    if (stage == 0){

        move.setSpeed(par("speed").doubleValue());
        acceleration = par("acceleration");
        angle = par("angle");
        angle = fmod(angle,360);
    }
    else if(stage == 1){
    	stepTarget = move.getStartPos();

		if(!world->use2D()) {
			opp_warning("This mobility module does not yet support 3 dimensional movement."\
						"Movements will probably be incorrect.");
		}
		if(!world->useTorus()) {
			opp_warning("You are not using a torus (parameter \"useTorus\" in"\
						"BaseWorldUtility module) playground but this mobility"\
						"module uses WRAP as border policy.");
		}
    }
}


void LinearMobility::fixIfHostGetsOutside()
{
    Coord dummy = Coord::ZERO;
    handleIfOutside(WRAP, stepTarget, dummy, dummy, angle);
}


/**
 * Move the host if the destination is not reached yet. Otherwise
 * calculate a new random position
 */
void LinearMobility::makeMove()
{
	debugEV << "start makeMove " << move.info() << endl;

    move.setStart(stepTarget, simTime());

    stepTarget.x = (move.getStartPos().x + move.getSpeed() * cos(PI * angle / 180) * SIMTIME_DBL(updateInterval));
    stepTarget.y = (move.getStartPos().y + move.getSpeed() * sin(PI * angle / 180) * SIMTIME_DBL(updateInterval));

    move.setDirectionByTarget(stepTarget);

    debugEV << "new stepTarget: " << stepTarget.info() << endl;

    // accelerate
    move.setSpeed(move.getSpeed() + acceleration * SIMTIME_DBL(updateInterval));

    fixIfHostGetsOutside();
}
