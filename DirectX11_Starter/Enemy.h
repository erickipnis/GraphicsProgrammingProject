#pragma once
#include "Ship.h"
#include <vector>
class Enemy
{
public:
	Enemy();
	~Enemy();

	int health;
	int numShips;
	int numRound;
	int spawnTimer;

	//vector of ships
	std::vector<Ship*> ships;

private:

};

