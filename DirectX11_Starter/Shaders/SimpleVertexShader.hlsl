cbuffer perModel : register(b0)
{
	matrix world;
	matrix view;
	matrix projection;
};

// Per-vertex data used as input to the vertex shader.
struct VertexShaderInput
{
	//float3 pos : POSITION;
	//float2 tex : TEXCOORD0;
	//float3 norm : NORMAL0;
	//float3 color : COLOR0;
	float3 pos		: POSITION;
	float3 normal		: NORMAL;
	float2 uv			: TEXCOORD;
	float4 tangent		: TANGENT;
};

// Per-pixel color data passed to the pixel shader.
struct PixelShaderInput
{
	float4 pos : SV_POSITION;
};

// Shader to do vertex processing for camera view position and light view position.
PixelShaderInput main(VertexShaderInput input)
{
	PixelShaderInput output;
	//float4 pos = float4(input.pos, 1.0f);

	// Transform the vertex position into projected space.
	//pos = mul(pos, world);
	//pos = mul(pos, view);
	//pos = mul(pos, projection);
	//output.pos = pos;

	matrix worldViewProj = mul(mul(world, view), projection);
	output.pos = mul(float4(input.pos, 1.0f), worldViewProj);

	return output;
}