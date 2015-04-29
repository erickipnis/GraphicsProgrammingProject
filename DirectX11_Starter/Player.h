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
	int resources;
	int score;

	//vector of ships
	std::vector<Ship*> ships;

	//the current ship will determine the cost of resources and which temp ship shows up when the player hovers over a grid
	char currentShip;
	//a = assault ship
	//b = battle ship
	//s = submarine
	int nextShipCost;

private:
	void Shoot();

};

