#pragma once
#include <DirectXMath.h>

using namespace DirectX;

class Light
{
public:
	Light();
	Light(XMFLOAT4 ambColor, XMFLOAT4 difColor);
	~Light();
private:
	XMFLOAT4 ambientColor;
	XMFLOAT4 diffuseColor;
};

