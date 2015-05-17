#include "Grid.h"

Grid::Grid()
{

}
Grid::Grid(int rows, int columns, float size, XMFLOAT3 offset, Mesh* mesh, Material* material)
{
	mRows = rows;
	mColumns = columns;
	mSize = size;

	numTiles = mColumns * mRows;

	tiles = std::vector<GridTile*>();

	for (int i = 0; i < mRows; i++)
	{
		for (int j = 0; j < mColumns; j++)
		{
			float xPos = offset.x + j * size;
			float yPos = offset.z + i * size;

			tiles.push_back(new GridTile(XMFLOAT3(xPos, offset.y, yPos), mesh, material));
			tiles[i * mColumns + j]->SetScale(XMFLOAT3(mSize, mSize, mSize));
			tiles[i * mColumns + j]->Update();
		}
	}

	lastIndexUsed = 0;
}


Grid::~Grid()
{
	for (int i = 0; i < numTiles; i++)
	{
		delete tiles[i];
	}
}

int Grid::GetColumns()
{
	return mColumns;
}
int Grid::GetRows()
{
	return mRows;
}

// Calculate the nearest grid tile given the mouses screen space coordinates
GridTile* Grid::GetNearestTile(float xPos, float yPos, int screenWidth, int screenHeight, Camera* camera)
{
	float x = (2.0f * xPos) / screenWidth - 1.0f;
	float y = 1.0f - (2.0f * yPos) / screenHeight;
	
	XMFLOAT4 vec = XMFLOAT4(x, y, 0.0f, 1.0f);
	XMVECTOR tempVec = XMLoadFloat4(&vec);
	XMVECTOR cameraPos = XMLoadFloat3(&XMFLOAT3(0.0f, 10.0f, 0.0f));

	XMMATRIX projection = XMMatrixTranspose(XMLoadFloat4x4(&camera->GetProjectionMatrix()));

	XMMATRIX view = XMMatrixTranspose(XMLoadFloat4x4(&camera->GetViewMatrix()));

	XMMATRIX viewProj = XMMatrixMultiply(view, projection);
	viewProj = XMMatrixInverse(&XMMatrixDeterminant(viewProj), viewProj);

	XMVECTOR position = XMVector4Transform(tempVec, viewProj);

	XMFLOAT4 tempPos;
	XMStoreFloat4(&tempPos, position);

	tempPos.w = 1.0f / tempPos.w;

	tempPos.x *= tempPos.w;
	tempPos.y *= tempPos.w;
	tempPos.z *= tempPos.w;

	position = XMLoadFloat4(&tempPos);


	XMFLOAT3 closest = XMFLOAT3(999.0f, 999.0f, 999.0f);
	int index = 0;
	for (int i = 0; i < numTiles; i++)
	{
		XMVECTOR tilePos = XMLoadFloat3(&tiles[i]->GetPosition());
		XMVECTOR tempDistance = XMVector3LinePointDistance(cameraPos, position, tilePos);
		
		XMFLOAT3 distance;
		XMStoreFloat3(&distance, tempDistance);
		if (distance.x < closest.x)
		{
			closest = distance;
			index = i;
		}
	}
	lastIndexUsed = index;
	return tiles[index];
}

void Grid::Draw(ID3D11DeviceContext& device, Camera& camera)
{
	for (int i = 0; i < numTiles; i++)
	{
		tiles[i]->Draw(device, camera);
	}
}

void Grid::ShadowMapDraw(ID3D11DeviceContext& device, Camera& camera, SimpleVertexShader* simpVertShad)
{
	for (int i = 0; i < numTiles; i++)
	{
		tiles[i]->ShadowMapDraw(device, camera, simpVertShad);
	}
}

void Grid::Draw(ID3D11DeviceContext& device, Camera& camera, Camera& shadowCamera, SimplePixelShader* pixShad, SimpleVertexShader* vertShad, ID3D11ShaderResourceView* shadowSRV, ID3D11SamplerState* comparisonSampler)
{
	for (int i = 0; i < numTiles; i++)
	{
		tiles[i]->Draw(device, camera, shadowCamera, pixShad, vertShad, shadowSRV, comparisonSampler);
	}
}

GridTile Grid::getTile(int i)
{
	return *tiles[i];
}


