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
	float GetXMin();
	float GetXMax();
	float GetYMin();
	float GetYMax();
	float GetZMin();
	float GetZMax();

private:
	// the verts of the mesh to be used for the box
	std::vector<Vertex> mVerts;

	// values for the AABB
	float xMin;
	float xMax;
	float yMin;
	float yMax;
	float zMin;
	float zMax;

};

