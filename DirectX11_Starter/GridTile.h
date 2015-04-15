#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include "GameEntity.h"

using namespace DirectX;

class GridTile : public GameEntity
{
public:
	GridTile(XMFLOAT3 position, Mesh* mesh, Material* material);
	~GridTile();

	bool IsOccupied();
	void setStatus(bool status);

private:

	bool occupied;
};

