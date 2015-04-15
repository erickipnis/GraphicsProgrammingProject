#include "GameEntity.h"

GameEntity::GameEntity(Mesh* mesh, Material* material)
{
	mMesh = mesh;
	mMaterial = material;
	mPosition = XMFLOAT3(0.0f, 0.0f, 0.0f);
	mRotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	mScale = XMFLOAT3(1.0f, 1.0f, 1.0f);
	XMStoreFloat4x4(&mWorldMatrix, XMMatrixTranspose(XMMatrixIdentity()));

	mBoundingBox = mMesh->GetBoundingBox();
}

GameEntity::~GameEntity()
{
	
}

void GameEntity::Draw(ID3D11DeviceContext& device, Camera& camera)
{
	// Copy CPU-side data to a single CPU-side structure
	//  - Allows us to send the data to the GPU buffer in one step
	//  - Do this PER OBJECT, before drawing it
	SimplePixelShader* pixelShader = mMaterial->GetPixelShader();
	SimpleVertexShader* vertexShader = mMaterial->GetVertexShader();
	pixelShader->SetSamplerState("basicSampler", mMaterial->GetSamplerState());
	pixelShader->SetShaderResourceView("diffuseTexture", mMaterial->GetSRV());
	vertexShader->SetMatrix4x4("world", mWorldMatrix);
	vertexShader->SetMatrix4x4("view", camera.GetViewMatrix());
	vertexShader->SetMatrix4x4("projection", camera.GetProjectionMatrix());

	vertexShader->SetShader(true);
	pixelShader->SetShader(true);

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	// Draw the mesh
	ID3D11Buffer* vertexBuffer = mMesh->GetVertexBuffer();
	device.IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	device.IASetIndexBuffer(mMesh->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);

	device.DrawIndexed(
		mMesh->GetIndexCount(),	// The number of indices we're using in this draw
		0,
		0);



}

void GameEntity::Update()
{
	XMVECTOR tempPosition = XMLoadFloat3(&mPosition);
	XMVECTOR tempRotation = XMLoadFloat3(&mRotation);
	XMVECTOR tempScale = XMLoadFloat3(&mScale);

	XMMATRIX translate = XMMatrixTranslationFromVector(tempPosition);
	XMMATRIX rotation = XMMatrixRotationRollPitchYawFromVector(tempRotation);
	XMMATRIX scale = XMMatrixScalingFromVector(tempScale);

	XMMATRIX world = scale * rotation * translate;

	XMStoreFloat4x4(&mWorldMatrix, XMMatrixTranspose(world));

	mBoundingBox.Update(mWorldMatrix);
}
void GameEntity::Translate(XMFLOAT3 offset)
{
	
	XMVECTOR tempPosition = XMLoadFloat3(&mPosition);
	XMVECTOR tempOffset = XMLoadFloat3(&offset);

	tempPosition = XMVectorAdd(tempPosition, tempOffset);

	XMStoreFloat3(&mPosition, tempPosition);

}

void GameEntity::Rotate(XMFLOAT3 offset)
{

	XMVECTOR tempRotation = XMLoadFloat3(&mRotation);
	XMVECTOR tempOffset = XMLoadFloat3(&offset);

	tempRotation = XMVectorAdd(tempRotation, tempOffset);

	XMStoreFloat3(&mRotation, tempRotation);

}
void GameEntity::Scale(XMFLOAT3 offset)
{

	XMVECTOR tempScale = XMLoadFloat3(&mScale);
	XMVECTOR tempOffset = XMLoadFloat3(&offset);

	tempScale = XMVectorAdd(tempScale, tempOffset);

	XMStoreFloat3(&mScale, tempScale);

}
