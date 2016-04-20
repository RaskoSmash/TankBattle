#include "Agent.h"

tankNet::TankBattleCommand Agent::update(const tankNet::TankBattleStateData * state)
{
	previous = current;
	current = *state;
	switch (turrentState)
	{
	case SCAN:
		Scan();
		break;
	}


	return move;
}

void Agent::Scan()
{
	move.cannonMove = tankNet::CannonMovementOptions::RIGHT;

	//if()
}