// External variables
cbuffer RefractData : register(b0)
{
	float3 camPos;
}

// Defines the input to this pixel shader
// - Should match the output of our corresponding vertex shader
struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float3 normal		: NORMAL;
	float2 uv			: TEXCOORD;
	float4 tangent		: TANGENT;
	float3 worldPos		: TEXCOORD1;
	noperspective float2 screenUV		: TEXCOORD2;
};

struct DirectionalLight
{
	float4 AmbientColor;
	float4 DiffuseColor;
	float4 Direction;
	float4 Position;
};

cbuffer light : register(b1)
{
	float time;
	DirectionalLight directionalLight;
};

Texture2D diffuseTexture : register(t0);
Texture2D waterNormalMap : register(t1);
SamplerState basicSampler : register(s0);

// Entry point for this pixel shader
float4 main(VertexToPixel input) : SV_TARGET
{
	//return float4(input.screenUV, 0, 1);
	//return float4(input.worldPos, 1);
	
	float2 scrollUV1 = input.uv;
	float2 scrollUV2 = input.uv;

	float scaledTime = time * 0.01f;

	scrollUV1.x += scaledTime;
	scrollUV1.y += scaledTime;
	
	scrollUV2.x += scaledTime;
	scrollUV2.y -= scaledTime;

	input.normal = normalize(input.normal);

	float4 waterNormalFromTexture = waterNormalMap.Sample(basicSampler, scrollUV1);
	float4 waterNormalFromTextureScaled = waterNormalMap.Sample(basicSampler, scrollUV2 * 1.25);
	//return waterNormalFromTexture;
	
	// Unpack normal from water normal map texture sample
	float3 unpackedWaterNormal = (float3)(waterNormalFromTexture * 2.0f - 1.0f);
	float3 unpackedWaterNormalScaled = (float3)(waterNormalFromTextureScaled * 2.0f - 1.0f);

	//return float4(unpackedWaterNormal, 1);

	// Refract and alter final uv for refraction
	float3 toCamera = normalize(camPos - input.worldPos);

	//return float4(toCamera, 1);

	float3 refractionNormal = normalize(input.normal + unpackedWaterNormal * 0.1f);
	float3 refraction = refract(-toCamera, refractionNormal, 0.5f);
	input.screenUV.x += refraction.x * 0.1f;
	input.screenUV.y += refraction.y * 0.025f;
	

	unpackedWaterNormal += unpackedWaterNormalScaled;
	unpackedWaterNormal /= 2;

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

	//// adding the second light
	//toLight = -normalize(directionalLight2.Direction);
	//lightAmount = dot(toLight, finalWaterNormal);
	//finalColor += directionalLight2.DiffuseColor * lightAmount + directionalLight2.AmbientColor;
	
	//float4 waterColor = float4(0.0f, 0.46667f, 0.74510f, 1.0f);
	//float4 waterColor = float4(0.0f, 0.0f, 1.0f, 1.0f);
	float4 surfaceColor = diffuseTexture.Sample(basicSampler, input.screenUV);
		
	//return surfaceColor;

	// adding the texture color
	finalColor *= surfaceColor;
	return finalColor;
}