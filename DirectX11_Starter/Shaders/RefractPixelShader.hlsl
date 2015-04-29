// External variables
cbuffer RefractData : register(b0)
{
	float4 ambientColor;
	float4 lightColor;
	float3 lightPos;
	float3 lightDir;
	float3 camPos;
}


// Defines the input to this pixel shader
// - Should match the output of our corresponding vertex shader
struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float3 normal		: NORMAL;
	float2 uv			: TEXCOORD;
	float3 worldPos		: TEXCOORD1;
	float2 screenUV		: TEXCOORD2;
};

// Texture variables
Texture2D diffuseTexture : register(t0);
Texture2D normalTexture : register(t1);
SamplerState basicSampler : register(s0);

// Entry point for this pixel shader
float4 main(VertexToPixel input) : SV_TARGET
{
	// Re-normalize the normals coming in from the vertex shader
	input.normal = normalize(input.normal);

	float lightAmount = 0;
	float specular = 0;

	// Basic directional light calculation
	lightAmount = saturate(dot(input.normal, -lightDir));

	// Unpack the normals and normalize them
	float3 normalMap = normalTexture.Sample(basicSampler, input.uv).xyz * 2 - 1;
	float3 adjustedNormal = normalize(input.normal + normalMap * 0.1f);

	// Direction light with specularity
	float3 toCamera = normalize(camPos - input.worldPos);
	float3 reflection = reflect(normalize(lightDir), adjustedNormal);
	specular = pow(max(dot(reflection, toCamera), 0), 8);

	// Refract and alter final uv for refraction
	float3 refraction = refract(-toCamera, adjustedNormal, 0.9f);
	input.screenUV.x += refraction.x * 0.5f;
	input.screenUV.y += refraction.y * 0.5f;

	// Sample the screen texture
	float4 diffuse = diffuseTexture.Sample(basicSampler, input.screenUV);

	// Return the finished refraction lighting calculation
	return (diffuse * lightColor * lightAmount) + (diffuse * ambientColor) + specular;
}