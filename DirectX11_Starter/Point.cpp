#include "Point.h"

Point::Point()
{
}

Point::Point(XMFLOAT4 ambColor, XMFLOAT4 difColor, XMFLOAT3 pos)
	:Light(ambColor, difColor)
{
	position = pos;
}


Point::~Point()
{
}
