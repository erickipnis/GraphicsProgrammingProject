#pragma once
#include <d3d11.h>
#include <DirectXMath.h>

using namespace DirectX;

class GridTile
{
public:
	GridTile(XMFLOAT3 position);
	~GridTile();

	bool IsOccupied();
	XMFLOAT3 GetPosition();
	void setStatus(bool status);

private:

	XMFLOAT3 mPosition;
	bool occupied;
};

