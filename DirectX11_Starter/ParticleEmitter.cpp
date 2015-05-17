#include "ParticleEmitter.h"


ParticleEmitter::ParticleEmitter(EMITTER_DESC desc)
{
	// initial values for particles
	mMaxParticles = desc.maxParticles;
	mMinVelocity = desc.minVelocity;
	mMaxVelocity = desc.maxVelocity;
	mMinAcceleration = desc.minAcceleration;
	mMaxAcceleration = desc.maxAcceleration;
	mPosition = desc.position;
	mMinSize = desc.minSize;
	mMaxSize = desc.maxSize;
	mEmitRate = desc.emitRate;
	mMaxDistance =desc.maxDistance;
	mLifeSpan = desc.lifeSpan;
	mLoop = desc.loop;
	mFadeTime = desc.fadeTime;

	// make sure you can't inifintely loop particles
	if (mEmitRate <= 0.0f)
		mLoop = false;

	mMaterial = desc.material;
	mBlendState = desc.blendState;
	particles = new Particle*[mMaxParticles];
	verts = new ParticleVertex[mMaxParticles * 4];
	index = 0;
	totalTime = 0;
	numParticles = 0;
	emitTimer = 0;

	//ZeroMemory(&particles, sizeof(Particle) * mMaxParticles);
	EmitParticle();


	// Create the vertex buffer
	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_DYNAMIC;
	vbd.ByteWidth = sizeof(ParticleVertex) * mMaxParticles * 4; // Number of vertices in the "model" you want to draw
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA initialVertexData;
	ZeroMemory(&initialVertexData, sizeof(D3D11_SUBRESOURCE_DATA));
	initialVertexData.pSysMem = &verts[0];
	HR(desc.device->CreateBuffer(&vbd, &initialVertexData, &mVertexBuffer));

	UINT* indices = new UINT[mMaxParticles * 6];
	for (int i = 0; i < mMaxParticles; i++)
	{
		indices[i * 6] = i * 4;
		indices[i * 6 + 1] = i * 4 + 1;
		indices[i * 6 + 2] = i * 4 + 2;
		indices[i * 6 + 3] = i * 4 + 2;
		indices[i * 6 + 4] = i * 4 + 3;
		indices[i * 6 + 5] = i * 4;
	}

	// Create the index buffer
	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * mMaxParticles * 6; // Number of indices in the "model" you want to draw
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA initialIndexData;
	initialIndexData.pSysMem = indices;
	HR(desc.device->CreateBuffer(&ibd, &initialIndexData, &mIndexBuffer));
}


ParticleEmitter::~ParticleEmitter()
{
	ReleaseMacro(mVertexBuffer);
	ReleaseMacro(mIndexBuffer);

	/*for (int i = 0; i < numParticles; i++)
	{
		delete particles[i];
	}*/
	delete[] particles;
	delete[] verts;
}

void ParticleEmitter::Draw(ID3D11DeviceContext& device, Camera& camera)
{
	SimpleVertexShader* vertexShader = mMaterial->GetVertexShader();
	SimplePixelShader* pixelShader = mMaterial->GetPixelShader();

	pixelShader->SetSamplerState("basicSampler", mMaterial->GetSamplerState());
	pixelShader->SetShaderResourceView("diffuseTexture", mMaterial->GetSRV());
	pixelShader->SetFloat("lifeSpan", mLifeSpan);
	pixelShader->SetFloat("fadeTime", mFadeTime);

	vertexShader->SetMatrix4x4("view", camera.GetViewMatrix());
	vertexShader->SetMatrix4x4("projection", camera.GetProjectionMatrix());
	vertexShader->SetFloat3("cameraPos", camera.GetPosition());
	vertexShader->SetFloat("totalTime", totalTime);

	vertexShader->SetShader(true);
	pixelShader->SetShader(true);

	// copy the vertex data into the vertex buffer
	D3D11_MAPPED_SUBRESOURCE subresourse;
	ZeroMemory(&subresourse, sizeof(D3D11_MAPPED_SUBRESOURCE));
	device.Map(mVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subresourse);
	memcpy(subresourse.pData, &verts[0], sizeof(ParticleVertex) * numParticles * 4);
	device.Unmap(mVertexBuffer, 0);

	UINT stride = sizeof(ParticleVertex);
	UINT offset = 0;
	// Draw the mesh
	device.IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
	device.IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	device.OMSetBlendState(mBlendState, NULL, 0xffffffff);

	device.DrawIndexed(
		6 * numParticles,
		0,
		0);

	device.OMSetBlendState(NULL, NULL, 0xffffffff);
}

void ParticleEmitter::EmitParticle()
{


	if (index >= mMaxParticles && mLoop)
		index = 0;

	XMFLOAT3 distance = XMFLOAT3(rand() % 101 - 50, rand() % 101 - 50, rand() % 101 - 50);
	float dist = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (mMaxDistance)));
	XMVECTOR distVector = XMLoadFloat3(&XMFLOAT3(dist, dist, dist));
	XMVECTOR tempDistance = XMLoadFloat3(&distance);
	XMVECTOR tempPosition = XMLoadFloat3(&mPosition);
	tempDistance = XMVector3Normalize(tempDistance);
	tempDistance = XMVectorMultiply(tempDistance, distVector);
	tempDistance += tempPosition;

	XMStoreFloat3(&distance, tempDistance);

	// velocity
	float xVel = mMinVelocity.x + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (mMaxVelocity.x - mMinVelocity.x)));
	float yVel = mMinVelocity.y + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (mMaxVelocity.y - mMinVelocity.y)));
	float zVel = mMinVelocity.z + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (mMaxVelocity.z - mMinVelocity.z)));

	XMFLOAT3 velocity = XMFLOAT3(xVel, yVel, zVel);

	// acceleration
	float xAcc = mMinAcceleration.x + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (mMaxAcceleration.x - mMinAcceleration.x)));
	float yAcc = mMinAcceleration.y + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (mMaxAcceleration.y - mMinAcceleration.y)));
	float zAcc = mMinAcceleration.z + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (mMaxAcceleration.z - mMinAcceleration.z)));

	XMFLOAT3 acceleration = XMFLOAT3(xAcc, yAcc, zAcc);

	// size
	float size = mMinSize + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (mMaxSize - mMinSize)));

	// create the particles
	particles[index] = new Particle(totalTime, velocity, acceleration, distance, size);

	verts[index * 4] = particles[index]->GetVerts()[0];
	verts[index * 4 + 1] = particles[index]->GetVerts()[1];
	verts[index * 4 + 2] = particles[index]->GetVerts()[2];
	verts[index * 4 + 3] = particles[index]->GetVerts()[3];

	if (numParticles < mMaxParticles)
		numParticles++;

	if (particles[index] != nullptr)
	{
		delete particles[index];
		particles[index] = nullptr;
	}


	index++;

}

void ParticleEmitter::Update(float dt)
{
	while ((index < mMaxParticles || (index == mMaxParticles && mLoop)) && emitTimer >= mEmitRate)
	{
		EmitParticle();
		emitTimer -= mEmitRate;
	}
	totalTime += dt;
	emitTimer += dt;
}
