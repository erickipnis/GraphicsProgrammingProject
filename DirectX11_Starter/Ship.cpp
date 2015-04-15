#include "Ship.h"


Ship::Ship(GameEntity* e)
{
	shipEntity = e;
	speed = 0;
	projectileTimer = 0;
}


Ship::~Ship()
{
}
