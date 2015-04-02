#pragma once
#include "GridTile.h"

class Grid
{
public:
	Grid(int rows, int columns, float size, XMFLOAT3 offset);
	~Grid();

	int GetColumns();
	int GetRows();
	GridTile* GetNearestTile(XMFLOAT3 position);
private:

	int numTiles;

	int mRows;
	int mColumns;
	// the size of each grid
	float mSize;

	// dynamic 2d array of tile pointers
	GridTile*** tiles;
};


