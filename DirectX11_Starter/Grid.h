#pragma once
#include "GridTile.h";

class Grid
{
public:
	Grid();
	~Grid(); 

private:
	
	int numTiles;
	GridTile tiles[1][1];

	int rows;
	int columns;	
};


