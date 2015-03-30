#include "Light.h"

Light::Light()
{

}

Light::Light(XMFLOAT4 ambColor, XMFLOAT4 difColor)
{
	ambientColor = ambColor;
	diffuseColor = difColor;
}

Light::~Light()
{
}
