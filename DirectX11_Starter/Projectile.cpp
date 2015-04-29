#include "Projectile.h"


Projectile::Projectile(GameEntity* e, int d)
{
	projectileEntity = e;
	speed = 0;
	damage = d;
}


Projectile::~Projectile()
{
}
