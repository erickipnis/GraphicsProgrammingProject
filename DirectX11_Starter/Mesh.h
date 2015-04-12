#pragma once
#include "Windows.h"
#include <string>
#include <d3d11.h>
#include <assert.h>
#include <fstream>

#include "dxerr.h"
#include "GameTimer.h"
#include "Vertex.h"
#include "BoundingBox.h"

// Convenience macro for releasing a COM object
#define ReleaseMacro(x) { if(x){ x->Release(); x = 0; } }

// Macro for popping up a text box based
// on a failed HRESULT and then quitting (only in debug builds)
#if defined(DEBUG) | defined(_DEBUG)
	#ifndef HR
	#define HR(x)													\
		{															\
			HRESULT hr = (x);										\
			if(FAILED(hr))											\
					{												\
				DXTrace(__FILEW__, (DWORD)__LINE__, hr, L#x, true);	\
				PostQuitMessage(0);									\
					}												\
		}														
	#endif
#else
	#ifndef HR
	#define HR(x) (x)
	#endif
#endif


class Mesh
{
public:
	Mesh(Vertex* vertices, int numVertices, UINT* indices, int numIndices, ID3D11Device* d3dDevice);
	Mesh(char* file, ID3D11Device* d3dDevice);
	~Mesh();

	ID3D11Buffer* GetVertexBuffer() { return mVertexBuffer; }
	ID3D11Buffer* GetIndexBuffer() { return mIndexBuffer; }
	int GetIndexCount() { return mIndices; }

private:
	ID3D11Buffer* mVertexBuffer;
	ID3D11Buffer* mIndexBuffer;
	int mIndices;

	BoundingBox mBoundingBox;
};

