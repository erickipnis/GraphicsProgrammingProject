#include "Grid.h"


Grid::Grid(int rows, int columns, float size, XMFLOAT3 offset)
{
	mRows = rows;
	mColumns = columns;
	mSize = size;

	tiles = new GridTile**[mRows];

	for (int i = 0; i < mRows; i++)
	{
		tiles[i] = new GridTile*[mColumns];
		for (int j = 0; j < mColumns; j++)
		{
			float xPos = offset.x + j * size;
			float yPos = offset.y + i * size;

			tiles[i][j] = new GridTile(XMFLOAT3(xPos, yPos, offset.z));
		}
	}


}


Grid::~Grid()
{
	for (int i = 0; i < mRows; i++)
	{
		for (int j = 0; j < mColumns; j++)
		{
			delete tiles[i][j];
		}
		delete tiles[i];
	}

	delete tiles;
}

int Grid::GetColumns()
{
	return mColumns;
}
int Grid::GetRows()
{
	return mRows;
}

GridTile* Grid::GetNearestTile(XMFLOAT3 position)
{
	return tiles[0][0];
}
