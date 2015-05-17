#include "Particle.h"

Particle::Particle()
{

}
Particle::Particle(float initTime, XMFLOAT3 initVel, XMFLOAT3 accel, XMFLOAT3 initPos, float initSize)
{
	mInitTime = initTime;
	mInitVel = initVel;
	mInitPos = initPos;
	mAccel = accel;
	mInitSize = initSize;

	// pass the positions into the array of particle vertices
	mVerts = new ParticleVertex[4];
	mVerts[0] = { mInitPos, XMFLOAT3(0.5f, 0.5f, 0.0f), mAccel, mInitVel, XMFLOAT2(0.0f, 0.0f), mInitTime, mInitSize };
	mVerts[1] = { mInitPos, XMFLOAT3(0.5f, -0.5f, 0.0f), mAccel, mInitVel, XMFLOAT2(1.0f, 0.0f), mInitTime, mInitSize };
	mVerts[2] = { mInitPos, XMFLOAT3(-0.5f, -0.5f, 0.0f), mAccel, mInitVel, XMFLOAT2(1.0f, 1.0f), mInitTime, mInitSize };
	mVerts[3] = { mInitPos, XMFLOAT3(-0.5f, 0.5f, 0.0f), mAccel, mInitVel, XMFLOAT2(0.0f, 1.0f), mInitTime, mInitSize };
}


Particle::~Particle()
{
	delete[] mVerts;
}

ParticleVertex* Particle::GetVerts()
{ 
	return mVerts;
}
