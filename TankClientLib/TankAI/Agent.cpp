#include "Agent.h"

tankNet::TankBattleCommand Agent::update(const tankNet::TankBattleStateData * state, float deltaTime)
{
	previous = current;
	current = *state;

	//Halts all movement so for unfinished code
	move.tankMove = tankNet::TankMovementOptions::HALT;
	move.cannonMove = tankNet::CannonMovementOptions::HALT;

	switch (turrentState)
	{
	case SCAN:
		Scan(deltaTime);
		break;
	case AIM:
		Aim();
		break;
	case FIRE:
		break;
	}

	switch (tankState)
	{
	case EXPLORE:
		break;
	case EVADE:
		break;
	case PURSUIT:
		break;
	case FLANK:
		break;
	}

	return move;
}

void Agent::Scan(float deltaTime)
{
	move.cannonMove = tankNet::CannonMovementOptions::RIGHT;
//	scanTimer += deltaTime;
	for (int i = 0; i < current.tacticoolCount; ++i)
	{
		enemyInSight[i] = current.tacticoolData[i].inSight;
	}
	if (true)
	{
		scanTimer = 0;
		move.cannonMove = tankNet::CannonMovementOptions::HALT;
		turrentState = AIM;
	}
}

void Agent::Aim()
{
	move.tankMove = tankNet::TankMovementOptions::LEFT;
}

void Agent::Fire()
{
}

void Agent::Explore()
{
}

void Agent::Evade()
{
}

void Agent::Pursuit()
{
}

void Agent::Flank()
{
}
