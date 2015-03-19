#include "Material.h"

Material::Material(SimplePixelShader* pixelShader, SimpleVertexShader* vertexShader)
{
	mPixelShader = pixelShader;
	mVertexShader = vertexShader;
}