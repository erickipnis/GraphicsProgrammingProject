#include "Camera.h"

Camera::Camera()
{
	mPosition = XMFLOAT3(0.0f, 10.0f, 0.0f);
	mDirection = XMFLOAT3(0.0f, 0.0f, 0.0f);

	mRotationX = 0.0f;
	mRotationY = 0.0f;

	timer = GameTimer();
}

Camera::~Camera()
{

}

void Camera::Update()
{
	timer.Tick();
	XMVECTOR rotation = XMQuaternionRotationRollPitchYaw(mRotationY, mRotationX, 0.0f);
	XMVECTOR forward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	XMVECTOR direction = XMVector3Rotate(forward, rotation);
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	XMStoreFloat3(&mDirection, direction);
	XMVECTOR right = XMVectorSet(-mDirection.z, 0.0f, mDirection.x, 0.0f);
	XMVECTOR offset = XMVectorZero();


	if (GetAsyncKeyState('W') && 0x8000)
	{
		offset += direction * timer.DeltaTime();
	}
	if (GetAsyncKeyState('S') && 0x8000)
	{
		offset -= direction * timer.DeltaTime();
	}
	if (GetAsyncKeyState('A') && 0x8000)
	{
		offset += right * timer.DeltaTime();
	}
	if (GetAsyncKeyState('D') && 0x8000)
	{
		offset -= right * timer.DeltaTime();
	}
	if (GetAsyncKeyState(VK_SPACE) && 0x8000)
	{
		offset += up * timer.DeltaTime();
	}
	if (GetAsyncKeyState('X') && 0x8000)
	{
		offset -= up * timer.DeltaTime();
	}

	offset *= 2.0f;

	XMVECTOR tempPosition = XMLoadFloat3(&mPosition);
	tempPosition += offset;
	XMMATRIX tempView = XMMatrixLookToLH(tempPosition, direction, up);
	XMStoreFloat4x4(&mViewMatrix, XMMatrixTranspose(tempView));
	XMStoreFloat3(&mPosition, tempPosition);
}

void Camera::UpdateProjection(float aspectRatio)
{
	// Create the Projection matrix
	XMMATRIX P = XMMatrixPerspectiveFovLH(
		0.25f * 3.1415926535f,		// Field of View Angle
		aspectRatio,				// Aspect ratio
		0.1f,						// Near clip plane distance
		100.0f);					// Far clip plane distance
	XMStoreFloat4x4(&mProjectionMatrix, XMMatrixTranspose(P));
}

void Camera::Rotate(int xDif, int yDif)
{
	mRotationX += xDif * 0.005f;
	mRotationY += yDif * 0.005f;
}