// Defines the input to this pixel shader
// - Should match the output of our corresponding vertex shader
struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float2 uv			: TEXCOORD;
};

// Texture related variables
Texture2D diffuseTexture : register(t0);
SamplerState basicSampler : register(s0);

// Entry point for this pixel shader
float4 main(VertexToPixel input) : SV_TARGET
{	
	// Sample all of the pixels from the texture
	float4 color = diffuseTexture.Sample(basicSampler, input.uv);

	// Return the sampled texture color to display to the full screen quad
	return color;
}