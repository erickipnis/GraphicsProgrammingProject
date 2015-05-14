#pragma once

#include "SimpleShader.h"

class Material
{
public:
	Material(SimplePixelShader* pixelShader, SimpleVertexShader* vertexShader,
		ID3D11ShaderResourceView* srv, ID3D11SamplerState* samplerState);
	~Material();

	SimplePixelShader* GetPixelShader() { return mPixelShader; }
	SimpleVertexShader* GetVertexShader() { return mVertexShader; }
	ID3D11ShaderResourceView* GetSRV() { return mSRV; }
	ID3D11SamplerState* GetSamplerState() { return mSamplerState; }
private:
	SimplePixelShader* mPixelShader;
	SimpleVertexShader* mVertexShader;
	ID3D11ShaderResourceView* mSRV;
	ID3D11SamplerState* mSamplerState;
};