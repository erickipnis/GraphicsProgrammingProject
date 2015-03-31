#include "GridTile.h"


GridTile::GridTile()
{
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