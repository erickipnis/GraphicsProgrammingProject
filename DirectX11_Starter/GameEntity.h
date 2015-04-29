#pragma once

#include "Windows.h"
#include <string>
#include <d3d11.h>
#include <assert.h>
#include <DirectXMath.h>

#include "dxerr.h"
#include "GameTimer.h"
#include "Mesh.h"
#include "Material.h"
#include "Camera.h"

using namespace DirectX;
// Convenience macro for releasing a COM object
#define ReleaseMacro(x) { if(x){ x->Release(); x = 0; } }

// Macro for popping up a text box based
// on a failed HRESULT and then quitting (only in debug builds)
#if defined(DEBUG) | defined(_DEBUG)
#ifndef HR
#define HR(x)												\
		{															\
		HRESULT hr = (x);										\
		if(FAILED(hr))											\
				{														\
			DXTrace(__FILEW__, (DWORD)__LINE__, hr, L#x, true);	\
			PostQuitMessage(0);									\
				}														\
		}														
#endif
#else
#ifndef HR
#define HR(x) (x)
#endif
#endif

class GameEntity
{
private:
	XMFLOAT4X4 mWorldMatrix;
	XMFLOAT3 mPosition;
	XMFLOAT3 mRotation;
	XMFLOAT3 mScale;

	Mesh* mMesh;
	Material* mMaterial;

	BoundingBox mBoundingBox;

public:
	GameEntity(Mesh* mesh, Material* material);
	~GameEntity();

	// Get Methods
	XMFLOAT3 GetPosition() { return mPosition; }
	XMFLOAT3 GetRotation() { return mRotation; }
	XMFLOAT3 GetScale() { return mScale; }
	XMFLOAT4X4 GetWorldMatrix() { return mWorldMatrix; }
	BoundingBox GetBoundingBox() { return mBoundingBox; }

	// Set methods
	void SetPosition(XMFLOAT3 position) { mPosition = position; }
	void SetRotation(XMFLOAT3 rotation) { mRotation = rotation; }
	void SetScale(XMFLOAT3 scale) { mScale = scale; }
	void SetWorldMatrix(XMFLOAT4X4 worldMatrix) { mWorldMatrix = worldMatrix; }

	void Update();
	void Translate(XMFLOAT3 offset);
	void Rotate(XMFLOAT3 offset);
	void Scale(XMFLOAT3 offset);
	void Draw(ID3D11DeviceContext& device, Camera& camera);

	void setMesh(Mesh * mesh);
	void setMaterial(Material* mat);


};
