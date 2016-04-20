#pragma once
#include "../TankBattle-net/TankBattleHeaders.h"


class Agent
{
	//Health, pos, forward, cannonForward, canFire, enemy data
	tankNet::TankBattleStateData current, previous;

	//Steering
	tankNet::TankBattleCommand move;

	////////Turrent States properties
	enum TurrentState {SCAN, AIM, FIRE} turrentState = SCAN;
	void Scan();
	void Aim();
	void Fire();
	
	//Tank State properties
	enum CurrentState {EXPLORE, EVADE, PURSUIT, FLANK} tankState = EVADE;
	void Explore();
	void Evade();
public:

	tankNet::TankBattleCommand update(const tankNet::TankBattleStateData *state);
};