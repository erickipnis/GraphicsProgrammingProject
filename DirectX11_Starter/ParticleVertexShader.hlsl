
// The constant buffer that holds our "per model" data
// - Each object you draw with this shader will probably have
//   slightly different data (at least for the world matrix)
cbuffer perModel : register(b0)
{
	matrix view;
	matrix projection;
	float3 cameraPos;
	float totalTime;
	float fadeTime;
};

// Defines what kind of data to expect as input
// - This should match our input layout!
struct VertexShaderInput
{
	float3 center		: POSITION0;
	float3 offset		: POSITION1;
	float3 acceleration	: POSITION2;
	float3 velocity		: POSITION3;
	float2 uv			: TEXCOORD;
	float time			: PSIZE0;
	float size			: PSIZE1;
};

// Defines the output data of our vertex shader
// - At a minimum, you'll need an SV_POSITION
// - Should match the pixel shader's input
struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float3 normal		: NORMAL;
	float2 uv			: TEXCOORD;
	float age			: PSIZE0;
};

// The entry point for our vertex shader
VertexToPixel main(VertexShaderInput input)
{
	// Set up output
	VertexToPixel output;

	// age, in seconds, of the particle
	float age = totalTime - input.time;

	float3 worldPos = 0.5f * age * age * input.acceleration +
		age * input.velocity + input.center;

	// Calculate output position
	matrix viewProj = mul(view, projection);

	//
	// Billboarding the particle
	//
	// vector to the camera
	float3 toCamera = cameraPos - worldPos;
	// the camera's up vector
	float3 up = normalize(float3(view[1][0], view[1][1], view[1][2]));
	// the right vector, from the cross product of the up vector and the vector to the camera
	float3 right = -normalize(cross(up, toCamera));
	// multiply offsets by the right and up vectors to get the billboarded positions
	float3 pos = (right * input.offset.x + up * input.offset.y) * input.size;
	pos += worldPos;
	output.position = mul(float4(pos, 1.0f), viewProj);
	output.normal = toCamera;
	output.uv = input.uv;
	output.age = age;

	return output;
}