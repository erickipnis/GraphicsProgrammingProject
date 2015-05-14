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
	int spawnTimer;	//spawn timer decreases as rounds go up to make the game harder
	int round;		//round goes up every certain number of enemies killed
	int kills;
	int killsForNextRound;

	//vector of ships
	std::vector<Ship*> ships;

private:

};

