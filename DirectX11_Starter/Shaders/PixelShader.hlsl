
// Defines the input to this pixel shader
// - Should match the output of our corresponding vertex shader
struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float3 normal		: NORMAL;
	float2 uv			: TEXCOORD;
};

struct DirectionalLight
{
	float4 AmbientColor;
	float4 DiffuseColor;
	float4 Direction;
	float4 Position;
};

cbuffer light : register(b0)
{
	DirectionalLight directionalLight;
};

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

	// first light
	float3 toLight = -normalize(directionalLight.Direction);
	float lightAmount = dot(toLight, input.normal);
	float4 finalColor = directionalLight.DiffuseColor * lightAmount + directionalLight.AmbientColor;

	//// adding the second light
	//toLight = -normalize(directionalLight2.Direction);
	//lightAmount = dot(toLight, input.normal);
	//finalColor += directionalLight2.DiffuseColor * lightAmount + directionalLight2.AmbientColor;

	// adding the texture color
	finalColor *= surfaceColor;

	return finalColor;
}