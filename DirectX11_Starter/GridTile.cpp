#include "GridTile.h"


GridTile::GridTile(XMFLOAT3 position)
{
	occupied = false;
	mPosition = position;
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

XMFLOAT3 GridTile::GetPosition()
{
	return mPosition;
}