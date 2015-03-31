#include "Light.h"
#pragma once
class Directional : public Light
{
public:
	Directional();
	Directional(XMFLOAT4 ambColor, XMFLOAT4 difColor, XMFLOAT3 dir);
	~Directional();
private:
	XMFLOAT3 direction;
};

