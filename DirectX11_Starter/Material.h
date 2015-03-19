#include "SimpleShader.h"

class Material
{
public:
	Material(SimplePixelShader* pixelShader, SimpleVertexShader* mVertexShader);

	SimplePixelShader* GetPixelShader() { return mPixelShader; }
	SimpleVertexShader* GetVertexShader() { return mVertexShader; }
private:
	SimplePixelShader* mPixelShader;
	SimpleVertexShader* mVertexShader;
};