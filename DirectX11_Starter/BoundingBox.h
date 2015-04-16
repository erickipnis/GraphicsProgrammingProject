#pragma once
#include <vector>

#include "Vertex.h"

class BoundingBox
{
public:
	BoundingBox();
	BoundingBox(std::vector<Vertex>& verts);
	~BoundingBox();

	bool IsColliding(BoundingBox& other);

	// Accessors
	XMFLOAT3 GetAABBMin();
	XMFLOAT3 GetAABBMax();

	void Update(XMFLOAT4X4 world);

private:
	// the verts of the mesh to be used for the box
	std::vector<Vertex> mVerts;

	// values for the non-transformed bounding box, starting values
	XMFLOAT3 startMin;
	XMFLOAT3 startMax;

	// array of teh OBB verts
	XMFLOAT3 OBBVerts[8];

	// values for the AABB
	XMFLOAT3 AABBMin;
	XMFLOAT3 AABBMax;

	// helper methods to calculate the bounding boxes
	void CalculateOBBVerts(XMFLOAT3 min, XMFLOAT3 max);
	void CalculateAABBVerts();

};

