#pragma once

#include "GameEntity.h"

class Ship
{
public:
	Ship(GameEntity* e);
	~Ship();

	int health;
	int speed;
	GameEntity* shipEntity;
};
