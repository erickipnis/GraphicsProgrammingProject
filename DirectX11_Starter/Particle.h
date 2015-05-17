#pragma once

#include <Windows.h>
#include <DirectXMath.h>

using namespace DirectX;

// the struct for a vertex passed into a particle emitter shader
struct ParticleVertex
{
	XMFLOAT3 Center; // Center of the particle
	XMFLOAT3 Offset; // offset from the center
	XMFLOAT3 Acceleration; // initial acceleration
	XMFLOAT3 Velocity; // initial velocity
	XMFLOAT2 UV; // uv coordinates
	float Time; // initial time created
	float Size; // initial size in units
};

class Particle
{
public:
	Particle();
	Particle(float initTime, XMFLOAT3 initVel, XMFLOAT3 accel, XMFLOAT3 initPos, float initSize);
	~Particle();

	ParticleVertex* GetVerts();

private:
	float mInitTime;
	float mInitSize;
	XMFLOAT3 mInitVel;
	XMFLOAT3 mAccel;
	XMFLOAT3 mInitPos;

	// an array of the verts
	ParticleVertex* mVerts;

};

