#pragma once
#include "GameEntity.h"
class Projectile
{
public:
	Projectile(GameEntity* e);
	~Projectile();

	int speed;
	GameEntity* projectileEntity;
};

