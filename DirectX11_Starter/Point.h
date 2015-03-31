#include "Light.h"
#pragma once
class Point : Light
{
public:
	Point();
	Point(XMFLOAT4 ambColor, XMFLOAT4 difColor, XMFLOAT3 pos);
	~Point();
private:
	XMFLOAT3 position;
};

