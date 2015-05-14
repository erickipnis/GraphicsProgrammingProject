#include "Ship.h"


Ship::Ship(GameEntity* e, char type)
{
	shipEntity = e;
	speed = 0;
	projectileTimer = 0;
	shootRate = 0;
	health = 100;
	cost = 0;
	shipType = type;
	if (shipType == 'a')
	{
		cost = 5;
	}
	else if (shipType == 's')
	{
		cost = 10;
	}
	else if (shipType == 'b')
	{
		cost = 15;
	}
	else if (shipType == 'm')
	{
		cost = 20;
	}
}


Ship::~Ship()
{
}
