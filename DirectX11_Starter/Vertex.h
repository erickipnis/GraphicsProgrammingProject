#pragma once
#include <DirectXMath.h>
#include "DirectXGame.h"

using namespace DirectX;

struct Vertex
{
	XMFLOAT3 Position;
	XMFLOAT3 Normal;
	XMFLOAT2 UV;
	XMFLOAT4 Tangent;
};