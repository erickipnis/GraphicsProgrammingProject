#pragma once

#include <Windows.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include "GameTimer.h"

using namespace DirectX;

class Camera
{
public:
	Camera();
	~Camera();

	XMFLOAT4X4 GetViewMatrix() { return mViewMatrix; }
	XMFLOAT4X4 GetProjectionMatrix() { return mProjectionMatrix; }

	void Update();
	void UpdateProjection(float aspectRatio);
	void Rotate(int xDif, int yDif);
	void SetRotation(int xDif, int yDif);
	void SetDirection(XMFLOAT3 direction);
	XMFLOAT3 GetPosition();

private:
	GameTimer timer;

	XMFLOAT4X4 mViewMatrix;
	XMFLOAT4X4 mProjectionMatrix;

	XMFLOAT3 mPosition;
	XMFLOAT3 mDirection;

	float mRotationX;
	float mRotationY;
};