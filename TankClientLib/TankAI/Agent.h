#pragma once
#include "../TankBattle-net/TankBattleHeaders.h"


class Agent
{
	//my tanks: 
	//Health, pos, forward, cannonForward, canFire
	//Enemy's:
	//isAlive, inSight, lastKnownPosition, lastKnownDirection, lastKnownCannonForward, lastKnownTankForward
	tankNet::TankBattleStateData current, previous;
	
	//For each enemy not vectors
	float distanceBetween[3];
	float angleBetween[3];
	int	enemyInSight[3];

	//Steering
	tankNet::TankBattleCommand move;
	float scanTimer = 0;


	////////Turrent States properties
	enum TurrentState {SCAN, AIM, FIRE} turrentState = SCAN;
	void Scan(float deltaTime);
	void Aim();
	void Fire();
	
	//Tank State properties
	enum CurrentState {EXPLORE, EVADE, PURSUIT, FLANK} tankState = EXPLORE;
	void Explore();
	void Evade();
	void Pursuit();
	void Flank();

	//Logic Functions
	void steerToLocation(const float *location);
	void aimAtLocation(const float *location);
	void reverse();
	void evadeLocation(const float *location);
	void blendBetweenLocations(const float *location1, const float *location2);

public:

	tankNet::TankBattleCommand update(const tankNet::TankBattleStateData *state, float deltaTime);
};

