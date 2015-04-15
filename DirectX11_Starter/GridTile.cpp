#include "GridTile.h"


GridTile::GridTile(XMFLOAT3 position, Mesh* mesh, Material* material) 
	: GameEntity(mesh, material)
{
	occupied = false;
	SetPosition(position);
}


GridTile::~GridTile()
{
}

bool GridTile::IsOccupied()
{
	return occupied;
}

void GridTile::setStatus(bool status)
{
	occupied = status;
}
