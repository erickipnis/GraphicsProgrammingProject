#pragma once

#include "GameEntity.h"
#include "Projectile.h"

class Ship
{
public:
	Ship(GameEntity* e);
	~Ship();

	int health;
	int speed;
	int projectileTimer;
	GameEntity* shipEntity;

	//vector of projectiles
	std::vector<Projectile*> projectiles;
};
