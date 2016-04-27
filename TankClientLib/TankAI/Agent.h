#pragma once
#include "../TankBattle-net/TankBattleHeaders.h"
#include "../TankBattle-client-dyad-sfw/Vector2.h"

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


	//Steering
	tankNet::TankBattleCommand move;
	float scanTimer = 0;
	float randTimer = 0;

	// Active target location to pursue either rand location or an enemy tank
	Vector2 randtarget;
	Vector2 tanktarget;


	////////Turrent States properties
	enum TurrentState {SCAN, AIM, FIRE} turrentState = SCAN;
	void Scan(float deltaTime);
	void Aim();
	void Fire();
	
	//Tank State properties
	enum CurrentState {EXPLORE, EVADE, PURSUIT, FLANK} tankState = EXPLORE;
	void Explore(const float deltaTime);
	void Evade();
	void Pursuit();
	void Flank();

	//Logic Functions
	void steerToLocation(const Vector2 &currentPos, const Vector2 &currentFor, const Vector2 &location);
	void aimAtLocation(const Vector2 location);
	void reverse();
	void evadeLocation(const Vector2 location);
	void blendBetweenLocations(const Vector2 location1, const Vector2 location2);
	void CheckCollision(float threshold);

public:

	tankNet::TankBattleCommand update(const tankNet::TankBattleStateData *state, float deltaTime);
};

