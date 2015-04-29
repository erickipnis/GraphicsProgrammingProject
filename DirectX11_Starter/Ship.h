#pragma once

#include "GameEntity.h"
#include "Projectile.h"

class Ship
{
public:
	Ship(GameEntity* e, char type);
	~Ship();

	int health;
	int speed;
	int projectileTimer;
	float shootRate;
	GameEntity* shipEntity;

	//vector of projectiles
	std::vector<Projectile*> projectiles;

	char shipType;
	//a = assault ship
	//b = battle ship
	//s = submarine
	//e = enemy
	//m = mine...i know this doesn't make much sense but it makes coding it easier

	int cost;
};
