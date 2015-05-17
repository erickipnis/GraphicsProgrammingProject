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

void GameEntity::ShadowMapDraw(ID3D11DeviceContext& device, Camera& camera, SimpleVertexShader* simpVertShad)
{
	// Copy CPU-side data to a single CPU-side structure
	//  - Allows us to send the data to the GPU buffer in one step
	//  - Do this PER OBJECT, before drawing it
	SimplePixelShader* pixelShader = mMaterial->GetPixelShader();
	simpVertShad->SetMatrix4x4("world", mWorldMatrix);
	simpVertShad->SetMatrix4x4("view", camera.GetViewMatrix());
	simpVertShad->SetMatrix4x4("projection", camera.GetProjectionMatrix());

	simpVertShad->SetShader(true);
	//pixelShader->SetShader(false);
	device.PSSetShader(0, 0, 0);
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

void GameEntity::Draw(ID3D11DeviceContext& device, Camera& camera, Camera& shadowCamera, SimplePixelShader* pixShad, SimpleVertexShader* vertShad, ID3D11ShaderResourceView* shadowSRV, ID3D11SamplerState* comparisonSampler)
{
	// Copy CPU-side data to a single CPU-side structure
	//  - Allows us to send the data to the GPU buffer in one step
	//  - Do this PER OBJECT, before drawing it
	SimplePixelShader* pixelShader = pixShad;
	SimpleVertexShader* vertexShader = vertShad;
	pixelShader->SetSamplerState("basicSampler", mMaterial->GetSamplerState());
	pixelShader->SetSamplerState("shadowSampler", comparisonSampler);
	pixelShader->SetShaderResourceView("diffuseTexture", mMaterial->GetSRV());
	pixelShader->SetShaderResourceView("shadowMap", shadowSRV);

	vertexShader->SetMatrix4x4("world", mWorldMatrix);
	vertexShader->SetMatrix4x4("view", camera.GetViewMatrix());
	vertexShader->SetMatrix4x4("projection", camera.GetProjectionMatrix());
	vertexShader->SetFloat4("eyePos", XMFLOAT4(camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z, 1.0f));
	vertexShader->SetMatrix4x4("lView", shadowCamera.GetViewMatrix());
	vertexShader->SetMatrix4x4("lProjection", shadowCamera.GetProjectionMatrix());
	vertexShader->SetFloat4("lPos", XMFLOAT4(shadowCamera.GetPosition().x, shadowCamera.GetPosition().y, shadowCamera.GetPosition().z, 1.0f));

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

void GameEntity::setMesh(Mesh* mesh)
{
	mMesh = mesh;
}

void GameEntity::setMaterial(Material* mat)
{
	mMaterial = mat;
}

