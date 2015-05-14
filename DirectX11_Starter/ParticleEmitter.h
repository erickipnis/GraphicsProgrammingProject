#pragma once
#include "Camera.h"
#include "Particle.h"
#include <vector>
#include "dxerr.h"
#include "Material.h"
#include <time.h>

using namespace std;

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

struct EMITTER_DESC
{
	Material* material;
	ID3D11Device* device;
	ID3D11BlendState* blendState;
	int maxParticles;
	XMFLOAT3 minVelocity; 
	XMFLOAT3 maxVelocity;
	XMFLOAT3 minAcceleration; 
	XMFLOAT3 maxAcceleration;
	XMFLOAT3 position;
	float minSize; 
	float maxSize;
	float emitRate;
	float maxDistance;
	float lifeSpan;
	float fadeTime;
	bool loop;

};

class ParticleEmitter
{
public:
	ParticleEmitter(EMITTER_DESC desc);
	~ParticleEmitter();

	void Update(float dt);
	void Draw(ID3D11DeviceContext& device, Camera& camera);

private:
	Material* mMaterial;
	ID3D11BlendState* mBlendState;

	// Buffers
	ID3D11Buffer* mVertexBuffer;
	ID3D11Buffer* mIndexBuffer;

	// The vector of particles currently alive
	Particle** particles;
	// vector of vertices for the particles
	ParticleVertex* verts;
	
	// values for the emitter
	int mMaxParticles;
	XMFLOAT3 mMinVelocity;
	XMFLOAT3 mMaxVelocity;
	XMFLOAT3 mMinAcceleration;
	XMFLOAT3 mMaxAcceleration;
	XMFLOAT3 mPosition;
	float mMinSize;
	float mMaxSize;
	float mEmitRate;
	float mMaxDistance;
	float mLifeSpan;
	float mFadeTime;
	bool mLoop;

	// current amount of particles
	int numParticles;
	// the current index for the particles
	int index;
	// the total time the emitter has been running
	float totalTime;
	float emitTimer;

	// emit a particle at an offset from the center
	void EmitParticle();

};

