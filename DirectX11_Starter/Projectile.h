#pragma once
#include "GameEntity.h"
class Projectile
{
public:
	Projectile(GameEntity* e, int d);
	~Projectile();

	int speed;
	int damage;
	GameEntity* projectileEntity;
};

