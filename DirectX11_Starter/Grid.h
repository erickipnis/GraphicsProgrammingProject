#pragma once
#include "GridTile.h"
//#include "Camera.h"

class Grid
{
public:
	Grid();
	Grid(int rows, int columns, float size, XMFLOAT3 offset, Mesh* mesh, Material* material);
	~Grid();

	int GetColumns();
	int GetRows();
	GridTile getTile(int i);
	GridTile* GetNearestTile(float xPos, float yPos, int screenWidth, int screenHeight, Camera* camera);
	void Draw(ID3D11DeviceContext& deivce, Camera& camera);

	int lastIndexUsed;
private:

	int numTiles;

	int mRows;
	int mColumns;
	// the size of each grid
	float mSize;

	// vector of tile pointers
	std::vector<GridTile*> tiles;
};


