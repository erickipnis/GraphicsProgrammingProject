#include "Enemy.h"


Enemy::Enemy()
{
	spawnTimer = 0;
	round = 1;
	killsForNextRound = 2;
	kills = 0;
}


Enemy::~Enemy()
{
}
