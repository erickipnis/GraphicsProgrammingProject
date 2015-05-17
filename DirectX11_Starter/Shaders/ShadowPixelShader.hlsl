
// Defines the input to this pixel shader
// - Should match the output of our corresponding vertex shader
struct VertexToPixel
{
	float4 position			: SV_POSITION;
	float4 lightSpacePos	: POSITION1;
	float3 normal			: NORMAL0;
	float3 lRay				: NORMAL1;
	float3 view				: NORMAL2;
	float2 uv				: TEXCOORD;
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
Texture2D shadowMap : register(t1);
SamplerComparisonState shadowSampler : register(s0);
SamplerState basicSampler : register(s1);

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

	// NdotL for shadow offset, lighting.
	float3 N = normalize(input.normal);
	float3 L = normalize(input.lRay);
	float NdotL = dot(N, L);

	// Compute texture coordinates for the current point's location on the shadow map.
	float2 shadowTexCoords;
	shadowTexCoords.x = 0.5f + (input.lightSpacePos.x / input.lightSpacePos.w * 0.5f);
	shadowTexCoords.y = 0.5f - (input.lightSpacePos.y / input.lightSpacePos.w * 0.5f);
	float pixelDepth = input.lightSpacePos.z / input.lightSpacePos.w;

	float lighting = 1;
	//return surfaceColor;
	if ((saturate(shadowTexCoords.x) == shadowTexCoords.x) &&
		(saturate(shadowTexCoords.y) == shadowTexCoords.y) &&
		(pixelDepth > 0))
	{
		// Use an offset value to mitigate shadow artifacts due to imprecise 
		// floating-point values (shadow acne).
		//
		// This is an approximation of epsilon * tan(acos(saturate(NdotL))):
		float margin = acos(saturate(NdotL));
		// The offset can be slightly smaller with smoother shadow edges.
		float epsilon = 0.001 / margin;
		// Clamp epsilon to a fixed range so it doesn't go overboard.
		epsilon = clamp(epsilon, 0, 0.1);
		// Use the SampleCmpLevelZero Texture2D method (or SampleCmp) to sample from 
		// the shadow map, just as you would with Direct3D feature level 10_0 and
		// higher.  Feature level 9_1 only supports LessOrEqual, which returns 0 if
		// the pixel is in the shadow.
		lighting = float(shadowMap.SampleCmpLevelZero(
			shadowSampler,
			shadowTexCoords,
			pixelDepth + epsilon));

		if (lighting == 0.f)
		{
			return (float4((float3)surfaceColor * (float3)directionalLight.AmbientColor, 1.0f));
		}
	}
	return (float4((float3)surfaceColor * (directionalLight.AmbientColor + directionalLight.DiffuseColor * lightAmount), 1.f));
}