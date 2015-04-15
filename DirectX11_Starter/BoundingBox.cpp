#include "BoundingBox.h"

BoundingBox::BoundingBox()
{

}
BoundingBox::BoundingBox(std::vector<Vertex>& verts)
{

	startMin = XMFLOAT3(999.0f, 999.0f, 999.0f);
	startMax = XMFLOAT3(-999.0f, -999.0f, -999.0f);

	// loop through the vector of verts provided by the mesh
	// and apply to set up the original min/max values
	for (int i = 0; i < verts.size(); i++)
	{
		if (verts[i].Position.x < startMin.x)
			startMin.x = verts[i].Position.x;
		if (verts[i].Position.x > startMax.x)
			startMax.x = verts[i].Position.x;

		if (verts[i].Position.y < startMin.y)
			startMin.y = verts[i].Position.y;
		if (verts[i].Position.y > startMax.y)
			startMax.y = verts[i].Position.y;

		if (verts[i].Position.z < startMin.z)
			startMin.z = verts[i].Position.z;
		if (verts[i].Position.z > startMax.z)
			startMax.z = verts[i].Position.z;
	}
}

// Accessors
XMFLOAT3 BoundingBox::GetAABBMin() { return AABBMin; }
XMFLOAT3 BoundingBox::GetAABBMax() { return AABBMax; }


BoundingBox::~BoundingBox()
{
}

// Pass in the GameEntity's worldMatrix to update the bounding box
void BoundingBox::Update(XMFLOAT4X4 world)
{
	XMMATRIX tempWorld = XMMatrixTranspose(XMLoadFloat4x4(&world));

	XMVECTOR tempMin = XMLoadFloat3(&startMin);
	XMVECTOR tempMax = XMLoadFloat3(&startMax);

	tempMin = XMVector3Transform(tempMin, tempWorld);
	tempMax = XMVector3Transform(tempMax, tempWorld);

	XMFLOAT3 min;
	XMFLOAT3 max;
	XMStoreFloat3(&min, tempMin);
	XMStoreFloat3(&max, tempMax);

	CalculateOBBVerts(min, max);
	CalculateAABBVerts();
}


// Check if two bounding boxes are colliding (this one and other)
bool BoundingBox::IsColliding(BoundingBox& other)
{
	// AABB collision detection
	if (AABBMin.x < other.GetAABBMax().z || AABBMax.x > other.GetAABBMin().x || // test x-axis
		AABBMin.y < other.GetAABBMax().y || AABBMax.y > other.GetAABBMin().y || // test y-axis
		AABBMin.z < other.GetAABBMax().z || AABBMax.z > other.GetAABBMin().z)// test z-axis
		return true;

	return false;
}

void BoundingBox::CalculateOBBVerts(XMFLOAT3 min, XMFLOAT3 max)
{
	OBBVerts[0] = max;
	OBBVerts[1] = XMFLOAT3(max.x, max.y, min.z);
	OBBVerts[2] = XMFLOAT3(min.x, max.y, max.z);
	OBBVerts[3] = XMFLOAT3(min.x, max.y, min.z);
	OBBVerts[4] = XMFLOAT3(max.x, min.y, min.z);
	OBBVerts[5] = XMFLOAT3(min.x, min.y, max.z);
	OBBVerts[6] = XMFLOAT3(max.x, min.y, max.z);
	OBBVerts[7] = min;
}

void BoundingBox::CalculateAABBVerts()
{
	AABBMin = XMFLOAT3(999.0f, 999.0f, 999.0f);
	AABBMax = XMFLOAT3(-999.0f, -999.0f, -999.0f);

	for (int i = 0; i < 8; i++)
	{
		if (OBBVerts[i].x < AABBMin.x)
			AABBMin.x = OBBVerts[i].x;
		if (OBBVerts[i].x > AABBMax.x)
			AABBMax.x = OBBVerts[i].x;

		if (OBBVerts[i].y < AABBMin.y)
			AABBMin.y = OBBVerts[i].y;
		if (OBBVerts[i].y > AABBMax.y)
			AABBMax.y = OBBVerts[i].y;

		if (OBBVerts[i].z < AABBMin.z)
			AABBMin.z = OBBVerts[i].z;
		if (OBBVerts[i].z > AABBMax.z)
			AABBMax.z = OBBVerts[i].z;
	}
}
