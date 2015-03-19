
// Defines the input to this pixel shader
// - Should match the output of our corresponding vertex shader
struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float3 normal		: NORMAL;
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

// Entry point for this pixel shader
float4 main(VertexToPixel input) : SV_TARGET
{
	// Just return the input color
	// - Note that this color (like all values that pass through the rasterizer)
	//   is interpolated for each pixel between the corresponding 
	//   vertices of the triangle

	input.normal = normalize(input.normal);
	float3 toLight = -normalize(directionalLight.Direction);
	float lightAmount = dot(toLight, input.normal);

	float4 totalLight = directionalLight.DiffuseColor * lightAmount + directionalLight.AmbientColor;

	toLight = -normalize(directionalLight2.Direction);
	lightAmount = dot(toLight, input.normal);
	totalLight += directionalLight2.DiffuseColor * lightAmount + directionalLight2.AmbientColor;
	return totalLight;
}