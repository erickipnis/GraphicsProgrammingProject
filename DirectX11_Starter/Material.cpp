#include "Material.h"

Material::Material(SimplePixelShader* pixelShader, SimpleVertexShader* vertexShader,
	ID3D11ShaderResourceView* srv, ID3D11SamplerState* samplerState)
{
	mPixelShader = pixelShader;
	mVertexShader = vertexShader;
	mSRV = srv;
	mSamplerState = samplerState;
}

Material::~Material()
{
	if (mSRV)
	{
		mSRV->Release();
		mSRV = 0;
	}
	if (mSamplerState)
	{
		mSamplerState->Release();
		mSamplerState = 0;
	}
}