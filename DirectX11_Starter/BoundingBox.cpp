#include "BoundingBox.h"

BoundingBox::BoundingBox()
{

}
BoundingBox::BoundingBox(std::vector<Vertex>& verts)
{
	// set up initial values
	xMin = 999.0f;
	xMax = -999.0f;

	yMin = 999.0f;
	yMax = -999.0f;

	zMin = 999.0f;
	zMax = -999.0f;

	// loop through the vector of verts provided by the mesh
	// and apply set up the AABB values
	for (int i = 0; i < verts.size(); i++)
	{
		if (verts[i].Position.x < xMin)
			xMin = verts[i].Position.x;
		if (verts[i].Position.x > xMax)
			xMax = verts[i].Position.x;

		if (verts[i].Position.y < yMin)
			yMin = verts[i].Position.y;
		if (verts[i].Position.y > yMax)
			yMax = verts[i].Position.y;

		if (verts[i].Position.z < zMin)
			zMin = verts[i].Position.z;
		if (verts[i].Position.z > zMax)
			zMax = verts[i].Position.z;
	}
}

// Accessors
float BoundingBox::GetXMin() { return xMin; }
float BoundingBox::GetXMax() { return xMax; }
float BoundingBox::GetYMin() { return yMin; }
float BoundingBox::GetYMax() { return yMax; }
float BoundingBox::GetZMin() { return zMin; }
float BoundingBox::GetZMax() { return zMax; }


BoundingBox::~BoundingBox()
{
}

// Check if two bounding boxes are colliding (this one and other)
bool BoundingBox::IsColliding(BoundingBox& other)
{
	if (xMin < other.GetXMax() || xMax > other.GetXMin() || // test x-axis
		yMin < other.GetYMax() || yMax > other.GetYMin() || // test y-axis
		zMin < other.GetZMax() || zMax > other.GetZMin())	// test z-axis
		return true;

	return false;
}
