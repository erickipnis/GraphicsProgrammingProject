
// Defines the input to this pixel shader
// - Should match the output of our corresponding vertex shader
struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float3 normal		: NORMAL;
	float2 uv			: TEXCOORD;
	float4 tangent		: TANGENT;
};

struct DirectionalLight
{
	float4 AmbientColor;
	float4 DiffuseColor;
	float3 Direction;
};

cbuffer light : register(b0)
{
	DirectionalLight directionalLight;
	DirectionalLight directionalLight2;
};

Texture2D diffuseTexture : register(t0);
Texture2D waterNormalMap : register(t1);

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

	float4 waterNormalFromTexture = waterNormalMap.Sample(basicSampler, input.uv);

	// Unpack normal from water normal map texture sample
	float3 unpackedWaterNormal = (float3)(waterNormalFromTexture * 2.0f - 1.0f);

	float3 N = normalize(input.normal);

	float3 T = float3(input.tangent.x, input.tangent.y, input.tangent.z);
	T = normalize(T - dot(T, N) * N);

	float3 B = cross(N, T);

	float3x3 TBN = float3x3(T, B, N);

	float3 finalWaterNormal = mul(unpackedWaterNormal, TBN);

	// first light
	float3 toLight = -normalize(directionalLight.Direction);
	float lightAmount = dot(toLight, finalWaterNormal);
	float4 finalColor = directionalLight.DiffuseColor * lightAmount + directionalLight.AmbientColor;

	// adding the second light
	toLight = -normalize(directionalLight2.Direction);
	lightAmount = dot(toLight, finalWaterNormal);
	finalColor += directionalLight2.DiffuseColor * lightAmount + directionalLight2.AmbientColor;

	// adding the texture color
	finalColor *= surfaceColor;
	return finalColor;
}