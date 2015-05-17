#pragma once
#include "GridTile.h"
//#include "Camera.h"

class Grid
{
public:
	Grid();
	Grid(int rows, int columns, float size, XMFLOAT3 offset, Mesh* mesh, Material* material, ID3D11BlendState* blendState);
	~Grid();

	int GetColumns();
	int GetRows();
	GridTile getTile(int i);
	GridTile* GetNearestTile(float xPos, float yPos, int screenWidth, int screenHeight, Camera* camera);
	void Draw(ID3D11DeviceContext& deivce, Camera& camera);
	void ShadowMapDraw(ID3D11DeviceContext& device, Camera& camera, SimpleVertexShader* simpVertShad);
	void Draw(ID3D11DeviceContext& device, Camera& camera, Camera& shadowCamera, SimplePixelShader* pixShad, SimpleVertexShader* vertShad, ID3D11ShaderResourceView* shadowSRV, ID3D11SamplerState* comparisonSampler);

	int lastIndexUsed;
private:

	int numTiles;

	int mRows;
	int mColumns;
	// the size of each grid
	float mSize;

	ID3D11BlendState* mBlendState;

	// vector of tile pointers
	std::vector<GridTile*> tiles;
};


