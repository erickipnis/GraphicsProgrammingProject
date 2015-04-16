#pragma once
#include "Ship.h"
#include <vector>
class Player
{
public:
	Player();
	~Player();

	int health;
	int numShips;
	int numRound;
	int materials;
	int score;

	//vector of ships
	std::vector<Ship*> ships;

private:
	void Shoot();

};

