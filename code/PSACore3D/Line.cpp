#include "Line.h"

void Line::CreateVertexData()
{
	_vertices.resize(2);
	_vertices[0] = Vertex(Vector(0.0f, 0.0f, 0.0f), Vector(0.0f, 0.0f, -1.0f), Vector4(1.0f, 0.0f, 0.0f, 1.0f), Vector2(0.0f, 0.0f));
	_vertices[1] = Vertex(Vector(0.0f, 0.0f, 1000.0f), Vector(0.0f, 0.0f, -1.0f), Vector4(1.0f, 0.0f, 0.0f, 1.0f), Vector2(1.0f, 0.0f));
	_initVertices = _vertices;
}

void Line::CreateIndexData()
{
	int idx = 0; 
	_indices.resize(2);

	_indices[idx++] = 0;	_indices[idx++] = 1;
}

bool Line::Frame()
{
	return true;
}

bool Line::Render()
{
	PreRender();
	_immediateContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
	PostRender();
	return true;
}

//-----------------------------------------------------------

void DirLine::CreateVertexData()
{
	_vertices.resize(6);

	// x axis - red
	_vertices[0] = Vertex(Vector(0.0f, 0.0f, 0.0f), Vector(0.0f, 0.0f, -1.0f), Vector4(1.0f, 0.0f, 0.0f, 1.0f), Vector2(0.0f, 0.0f));
	_vertices[1] = Vertex(Vector(1.0f, 0.0f, 0.0f), Vector(0.0f, 0.0f, -1.0f), Vector4(1.0f, 0.0f, 0.0f, 1.0f), Vector2(1.0f, 0.0f));

	// y axis - green
	_vertices[2] = Vertex(Vector(0.0f, 0.0f, 0.0f), Vector(0.0f, 0.0f, -1.0f), Vector4(0.0f, 1.0f, 0.0f, 1.0f), Vector2(0.0f, 0.0f));
	_vertices[3] = Vertex(Vector(0.0f, 1.0f, 0.0f), Vector(0.0f, 0.0f, -1.0f), Vector4(0.0f, 1.0f, 0.0f, 1.0f), Vector2(1.0f, 0.0f));

	// z axis - blue
	_vertices[4] = Vertex(Vector(0.0f, 0.0f, 0.0f), Vector(0.0f, 0.0f, -1.0f), Vector4(0.0f, 0.0f, 1.0f, 1.0f), Vector2(0.0f, 0.0f));
	_vertices[5] = Vertex(Vector(0.0f, 0.0f, 1.0f), Vector(0.0f, 0.0f, -1.0f), Vector4(0.0f, 0.0f, 1.0f, 1.0f), Vector2(1.0f, 0.0f));

	_initVertices = _vertices;
}

void DirLine::CreateIndexData()
{
	int idx = 0;
	_indices.resize(6);

	_indices[idx++] = 0; 	_indices[idx++] = 1;
	_indices[idx++] = 2; 	_indices[idx++] = 3;
	_indices[idx++] = 4; 	_indices[idx++] = 5;
}