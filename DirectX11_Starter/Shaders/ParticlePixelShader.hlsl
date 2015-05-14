
// Defines the input to this pixel shader
// - Should match the output of our corresponding vertex shader
struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float3 normal		: NORMAL;
	float2 uv			: TEXCOORD;
	float age			: PSIZE0;
};

cbuffer particle : register(b0)
{
	float lifeSpan;
	float fadeTime;
}

Texture2D diffuseTexture : register(t0);

SamplerState basicSampler : register(s0);

// Entry point for this pixel shader
float4 main(VertexToPixel input) : SV_TARGET
{
	// Just return the input color
	// - Note that this color (like all values that pass through the rasterizer)
	//   is interpolated for each pixel between the corresponding 
	//   vertices of the triangle

	input.normal = normalize(input.normal);
	float4 surfaceColor = diffuseTexture.Sample(basicSampler, input.uv);
	surfaceColor.a *= clamp((lifeSpan - input.age) / fadeTime, 0.0f, 1.0f);
	surfaceColor.a = clamp(surfaceColor.a * 0.1f, 0.0f, 0.1f);
	return surfaceColor;
}