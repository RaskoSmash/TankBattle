#include "Agent.h"


tankNet::TankBattleCommand Agent::update(const tankNet::TankBattleStateData * state, float deltaTime)
{
	previous = current;
	current = *state;

	//Halts all movement so for unfinished code
	move.msg = tankNet::TankBattleMessage::GAME;
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
		Fire();
		break;
	}

	switch (tankState)
	{
	case EXPLORE:
		Explore(deltaTime);
		break;
	case EVADE:
		Evade();
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
	Vector2 tf = Vector2::fromXZ(current.cannonForward);
	Vector2 cp = Vector2::fromXZ(current.position);

	move.fireWish = false;
	move.cannonMove = tankNet::CannonMovementOptions::RIGHT;
//	scanTimer += deltaTime;
	for (int iTarget = 0; iTarget < current.tacticoolCount; ++iTarget)
	{
		if (current.tacticoolData[iTarget].inSight && current.tacticoolData[iTarget].isAlive)
		{
			scanTimer = 0;
//			move.cannonMove = tankNet::CannonMovementOptions::HALT;
			tanktarget = Vector2::fromXZ(current.tacticoolData[iTarget].lastKnownPosition);
			turrentState = AIM;
			break;
		}
	}
}

void Agent::Aim()
{
	move.fireWish = false;
	Vector2 tf = Vector2::fromXZ(current.cannonForward);
	Vector2 cp = Vector2::fromXZ(current.position);
	float an = cross(cp, tanktarget);

	int test = dot(tf, normal(tanktarget - cp)) > .50f;

	if (an < 0 && !test)
	{
		move.cannonMove = tankNet::CannonMovementOptions::LEFT;
	}
	else if (an >= 0 && !test)
	{
		move.cannonMove = tankNet::CannonMovementOptions::RIGHT;
	}
	
	if (test)
	{
		turrentState = FIRE;
	}
		

}

void Agent::Fire()
{
	move.fireWish = current.canFire;
	turrentState = AIM;
}

void Agent::Explore(const float deltaTime)
{
	Vector2 cp = Vector2::fromXZ(current.position); // current position
	Vector2 cf = Vector2::fromXZ(current.forward);  // current forward

	//if (distance(tanktarget, cp) < 30)
	//{
	//	tankState = EVADE;
	//}
	randTimer -= deltaTime;

	// If we're pretty close to the target, get a new target           
	if (distance(randtarget, cp) < 20 || randTimer < 0)
	{
		randtarget = Vector2::random() * Vector2 { 50, 50 };
		randTimer = rand() % 4 + 2; // every 2-6 seconds randomly pick a new target
	}

	steerToLocation(cp, cf, randtarget);

}

void Agent::Evade()
{
	
	randTimer = 0;
}

void Agent::Pursuit()
{

}

void Agent::Flank()
{

}

void Agent::steerToLocation(const Vector2 &currentPos, const Vector2 &currentFor, const Vector2 &location)
{

	Vector2 direction = normal(location - currentPos);

	if (dot(currentFor, direction) > .87f) // if the dot product is close to lining up, move forward
		move.tankMove = tankNet::TankMovementOptions::FWRD;
	else // otherwise turn right until we line up!
		move.tankMove = tankNet::TankMovementOptions::RIGHT;
}

//randTimer = rand() % 2;
//switch ((bool)randTimer)
//{
//case 0:
//	move.tankMove = tankNet::TankMovementOptions::BACK;
//	break;
//case 1:
//	move.tankMove = tankNet::TankMovementOptions::FWRD;
//	break;
//}