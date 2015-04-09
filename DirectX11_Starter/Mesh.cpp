#include "Mesh.h"


Mesh::Mesh(Vertex* vertices, int numVertices, UINT* indices, int numIndices, ID3D11Device* device)
{
	mIndices = numIndices;

	// Create the vertex buffer
	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex) * numVertices; // Number of vertices in the "model" you want to draw
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA initialVertexData;
	initialVertexData.pSysMem = vertices;
	HR(device->CreateBuffer(&vbd, &initialVertexData, &mVertexBuffer));

	// Create the index buffer
	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * numIndices; // Number of indices in the "model" you want to draw
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA initialIndexData;
	initialIndexData.pSysMem = indices;
	HR(device->CreateBuffer(&ibd, &initialIndexData, &mIndexBuffer));
}

Mesh::Mesh(char* file, ID3D11Device* device)
{
	// File input object
	std::ifstream obj(file); // <-- Replace filename with your parameter

	// Check for successful open
	if (!obj.is_open())
		return;

	// Variables used while reading the file
	std::vector<XMFLOAT3> positions;     // Positions from the file
	std::vector<XMFLOAT3> normals;       // Normals from the file
	std::vector<XMFLOAT2> uvs;           // UVs from the file
	std::vector<Vertex> verts;           // Verts we're assembling
	std::vector<UINT> indices;           // Indices of these verts
	unsigned int triangleCounter = 0;    // Count of triangles/indices
	char chars[100];                     // String for line reading

	// Still good?
	while (obj.good())
	{
		// Get the line (100 characters should be more than enough)
		obj.getline(chars, 100);

		// Check the type of line
		if (chars[0] == 'v' && chars[1] == 'n')
		{
			// Read the 3 numbers directly into an XMFLOAT3
			XMFLOAT3 norm;
			sscanf_s(
				chars,
				"vn %f %f %f",
				&norm.x, &norm.y, &norm.z);

			// Add to the list of normals
			normals.push_back(norm);
		}
		else if (chars[0] == 'v' && chars[1] == 't')
		{
			// Read the 2 numbers directly into an XMFLOAT2
			XMFLOAT2 uv;
			sscanf_s(
				chars,
				"vt %f %f",
				&uv.x, &uv.y);

			// Add to the list of uv's
			uvs.push_back(uv);
		}
		else if (chars[0] == 'v')
		{
			// Read the 3 numbers directly into an XMFLOAT3
			XMFLOAT3 pos;
			sscanf_s(
				chars,
				"v %f %f %f",
				&pos.x, &pos.y, &pos.z);

			// Add to the positions
			positions.push_back(pos);
		}
		else if (chars[0] == 'f')
		{
			// Read the 9 face indices into an array
			unsigned int i[9];
			sscanf_s(
				chars,
				"f %d/%d/%d %d/%d/%d %d/%d/%d",
				&i[0], &i[1], &i[2],
				&i[3], &i[4], &i[5],
				&i[6], &i[7], &i[8]);

			// - Create the verts by looking up
			//    corresponding data from vectors
			// - OBJ File indices are 1-based, so
			//    they need to be adusted
			Vertex v1;
			v1.Position = positions[i[0] - 1];
			v1.UV = uvs[i[1] - 1];
			v1.Normal = normals[i[2] - 1];

			Vertex v2;
			v2.Position = positions[i[3] - 1];
			v2.UV = uvs[i[4] - 1];
			v2.Normal = normals[i[5] - 1];

			Vertex v3;
			v3.Position = positions[i[6] - 1];
			v3.UV = uvs[i[7] - 1];
			v3.Normal = normals[i[8] - 1];

			// Add the verts to the vector
			verts.push_back(v1);
			verts.push_back(v2);
			verts.push_back(v3);

			// Add three more indices
			indices.push_back(triangleCounter++);
			indices.push_back(triangleCounter++);
			indices.push_back(triangleCounter++);
		}
	}

	// Close
	obj.close();

	// - At this point, "verts" is a vector of Vertex structs, and can be used
	//    directly to create a vertex buffer:  &verts[0] is the first vert
	// - The vector "indices" is similar. It's a vector of unsigned ints and
	//    can be used directly for the index buffer: &indices[0] is the first int
	// - "triangleCounter" is BOTH the number of vertices and the number of indices

	mIndices = triangleCounter;

	// Create the vertex buffer
	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex) * triangleCounter; // Number of vertices in the "model" you want to draw
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA initialVertexData;
	initialVertexData.pSysMem = &verts[0];
	HR(device->CreateBuffer(&vbd, &initialVertexData, &mVertexBuffer));

	// Create the index buffer
	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * triangleCounter; // Number of indices in the "model" you want to draw
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA initialIndexData;
	initialIndexData.pSysMem = &indices[0];
	HR(device->CreateBuffer(&ibd, &initialIndexData, &mIndexBuffer));
}

Mesh::~Mesh()
{
	ReleaseMacro(mVertexBuffer);
	ReleaseMacro(mIndexBuffer);
}

void Mesh::CalculateTangents(int numVertices, vector<XMFLOAT3> positionsVect, vector<XMFLOAT3> normalsVect, vector<XMFLOAT2> uvsVect, vector<Vertex> vertsVect, vector<UINT> indicesVect)
{
	XMFLOAT3* tan1 = new XMFLOAT3[numVertices * 2];
	XMFLOAT3* tan2 = tan1 + numVertices;
	ZeroMemory(tan1, numVertices * sizeof(XMFLOAT3) * 2);

	for (unsigned int i = 0; i < numVertices; i += 3)
	{
		// Get the three vertices that make up a triangle
		Vertex v1 = vertsVect[i];
		Vertex v2 = vertsVect[i + 1];
		Vertex v3 = vertsVect[i + 2];

		// Get the three texture coordinates associated with the triangle
		XMFLOAT2 w1 = v1.UV;
		XMFLOAT2 w2 = v2.UV;
		XMFLOAT2 w3 = v3.UV;

		float x1 = v2.Position.x - v1.Position.x;
		float x2 = v3.Position.x - v1.Position.x;
		float y1 = v2.Position.y - v1.Position.y;
		float y2 = v3.Position.y - v1.Position.y;
		float z1 = v2.Position.z - v1.Position.z;
		float z2 = v3.Position.z - v1.Position.z;

		float s1 = w2.x - w1.x;
		float s2 = w3.x - w1.x;
		float t1 = w2.y - w1.y;
		float t2 = w3.y - w1.y;

		float r = 1.0f / (s1 * t2 - s2 * t1);

		XMFLOAT3 sDirection = XMFLOAT3((t2 * x1 - t1 * x2) * r, (t2 * y1 - t1 * y2) * r, (t2 * z1 - t1 * z2) * r);
		XMFLOAT3 tDirection = XMFLOAT3((s1 * x2 - s2 * x1) * r, (s1 * y2 - s2 * y1) * r, (s1 * z2 - s2 * z1) * r);
	}

	for (j = 0; j < numVertices; j++)
	{
		XMFLOAT3 n = normalsVect[j];
		XMFLOAT3 t = tan1[j];

		//XMFLOAT4* tangent;

		// Gram-Schmidt Orthogonalize
		// tangent[j] = (t - n * Dot(n, t)).Normalize();

		// Calculate handedness
		// tangent[j].w = (Dot(Cross(n, t), tan2[j]) < 0.0f) ? -1.0f : 1.0f;

	}

	delete[] tan1;
	delete[] tan2;
}