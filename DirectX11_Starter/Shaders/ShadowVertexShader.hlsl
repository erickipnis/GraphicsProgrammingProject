
// The constant buffer that holds our "per model" data
// - Each object you draw with this shader will probably have
//   slightly different data (at least for the world matrix)
cbuffer perModel : register(b0)
{
	matrix world;
	matrix view;
	matrix projection;
	float4 eyePos;
	matrix lView;
	matrix lProjection;
	float4 lPos; // light position
};

// Defines what kind of data to expect as input
// - This should match our input layout!
struct VertexShaderInput
{
	float3 position		: POSITION;
	float3 normal		: NORMAL;
	float2 uv			: TEXCOORD;
	float4 tangent		: TANGENT;
};

// Defines the output data of our vertex shader
// - At a minimum, you'll need an SV_POSITION
// - Should match the pixel shader's input
struct VertexToPixel
{
	float4 position			: SV_POSITION;	// System Value Position - Has specific meaning to the pipeline!
	float4 lightSpacePos	: POSITION1;
	float3 normal			: NORMAL0;
	float3 lRay				: NORMAL1;
	float3 view				: NORMAL2;
	float2 uv				: TEXCOORD;
};

// The entry point for our vertex shader
VertexToPixel main(VertexShaderInput input)
{
	// Set up output
	VertexToPixel output;

	// Calculate output position
	matrix worldViewProj = mul(mul(world, view), projection);
	output.position = mul(float4(input.position, 1.0f), worldViewProj);
	output.normal = mul(input.normal, (float3x3)world);
	output.uv = input.uv;

	float4 pos = float4(input.position, 1.0f);
	float4 modelPos = mul(pos, world);
	// Transform the vertex position into projected space from the POV of the light.
	float4 lightSpacePos = mul(modelPos, lView);
	lightSpacePos = mul(lightSpacePos, lProjection);
	output.lightSpacePos = lightSpacePos;

	// Light ray
	float3 lRay = lPos.xyz - modelPos.xyz;
	output.lRay = lRay;

	// Camera ray
	output.view = eyePos.xyz - modelPos.xyz;

	return output;
}