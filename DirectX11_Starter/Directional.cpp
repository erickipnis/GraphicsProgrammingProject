#include "Directional.h"


Directional::Directional(XMFLOAT4 ambColor, XMFLOAT4 difColor, XMFLOAT3 dir)
	:Light(ambColor, difColor)
{
	direction = dir;
}


Directional::~Directional()
{
}
