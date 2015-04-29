
// The constant buffer that holds our "per model" data
// - Each object you draw with this shader will probably have
//   slightly different data (at least for the world matrix)
cbuffer perModel : register(b0)
{
	matrix world;
	matrix view;
	matrix projection;
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
	float4 position		: SV_POSITION;	// System Value Position
	float3 normal		: NORMAL;
	float2 uv			: TEXCOORD;
	float3 worldPos		: TEXCOORD1;
	float2 screenUV		: TEXCOORD2;
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

	output.worldPos = mul(float4(input.position, 1.0f), world).xyz;

	output.uv = input.uv;

	output.screenUV = (output.position.xy / output.position.w);
	output.screenUV.x = output.screenUV.x * 0.5f + 0.5f;
	output.screenUV.y = output.screenUV.y = -output.screenUV.y * 0.5 + 0.5f;

	return output;
}