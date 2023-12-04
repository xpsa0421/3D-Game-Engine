#include "Box.h"

void Box::SetVertexPoints()
{
	// front side
	_vertices[0].p	= Vector(-1.0f, 1.0f, -1.0f);
	_vertices[1].p	= Vector(1.0f, 1.0f, -1.0f);
	_vertices[2].p	= Vector(1.0f, -1.0f, -1.0f);
	_vertices[3].p	= Vector(-1.0f, -1.0f, -1.0f);

	// back side
	_vertices[4].p	= Vector(1.0f, 1.0f, 1.0f);
	_vertices[5].p	= Vector(-1.0f, 1.0f, 1.0f);
	_vertices[6].p	= Vector(-1.0f, -1.0f, 1.0f);
	_vertices[7].p	= Vector(1.0f, -1.0f, 1.0f);

	// right side
	_vertices[8].p	= Vector(1.0f, 1.0f, -1.0f);
	_vertices[9].p	= Vector(1.0f, 1.0f, 1.0f);
	_vertices[10].p = Vector(1.0f, -1.0f, 1.0f);
	_vertices[11].p = Vector(1.0f, -1.0f, -1.0f);
	
	// left side
	_vertices[12].p	= Vector(-1.0f, 1.0f, 1.0f);
	_vertices[13].p = Vector(-1.0f, 1.0f, -1.0f);
	_vertices[14].p = Vector(-1.0f, -1.0f, -1.0f);
	_vertices[15].p = Vector(-1.0f, -1.0f, 1.0f);
				
	// top side
	_vertices[16].p	= Vector(-1.0f, 1.0f, 1.0f);
	_vertices[17].p = Vector(1.0f, 1.0f, 1.0f);
	_vertices[18].p = Vector(1.0f, 1.0f, -1.0f);
	_vertices[19].p = Vector(-1.0f, 1.0f, -1.0f);
				 
	// bottom side
	_vertices[20].p	= Vector(-1.0f, -1.0f, -1.0f);
	_vertices[21].p = Vector(1.0f, -1.0f, -1.0f);
	_vertices[22].p = Vector(1.0f, -1.0f, 1.0f);
	_vertices[23].p = Vector(-1.0f, -1.0f, 1.0f);
}

void Box::SetVertexNormals()
{
	for ( int i = 23; i > -1; i--)
	{
		if (i >= 20)		
			_vertices[i].n = Vector(0.0f, -1.0f, 0.0f);	// bottom side
		else if (i >= 16)	
			_vertices[i].n = Vector(0.0f, 1.0f, 0.0f);	// top side
		else if (i >= 12)	
			_vertices[i].n = Vector(-1.0f, 0.0f, 0.0f);	// left side
		else if (i >= 8)	
			_vertices[i].n = Vector(1.0f, 0.0f, 0.0f);	// right side
		else if (i >= 4)	
			_vertices[i].n = Vector(0.0f, 0.0f, 1.0f);	// back side
		else                
			_vertices[i].n = Vector(0.0f, 0.0f, -1.0f);	// front side
	}
}

void Box::SetVertexColours()
{
	for (int i = 0; i < 24; i++)
	{
		_vertices[i].c = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	}
}

void Box::SetVertexTextures()
{
	int idx = 0;
	for (int sideIdx = 0; sideIdx < 6; sideIdx++)
	{
		_vertices[idx++].t = Vector2(0.0f, 0.0f);
		_vertices[idx++].t = Vector2(1.0f, 0.0f);
		_vertices[idx++].t = Vector2(1.0f, 1.0f);
		_vertices[idx++].t = Vector2(0.0f, 1.0f);
	}
}

void Box::CreateVertexData()
{
	_vertices.resize(24);
	SetVertexPoints();
	SetVertexNormals();
	SetVertexColours();
	SetVertexTextures();

	_initVertices = _vertices;
}

void Box::CreateIndexData()
{
	_indices.resize(36);
	int i = 0;

	_indices[i++] = 0; 	_indices[i++] = 1; 	_indices[i++] = 2; 
	_indices[i++] = 0;	_indices[i++] = 2; 	_indices[i++] = 3;

	_indices[i++] = 4; 	_indices[i++] = 5; 	_indices[i++] = 6;
	_indices[i++] = 4;	_indices[i++] = 6; 	_indices[i++] = 7;

	_indices[i++] = 8; 	_indices[i++] = 9; 	_indices[i++] = 10;
	_indices[i++] = 8;	_indices[i++] = 10; _indices[i++] = 11;

	_indices[i++] = 12; _indices[i++] = 13; _indices[i++] = 14;
	_indices[i++] = 12;	_indices[i++] = 14; _indices[i++] = 15;

	_indices[i++] = 16; _indices[i++] = 17; _indices[i++] = 18;
	_indices[i++] = 16;	_indices[i++] = 18; _indices[i++] = 19;

	_indices[i++] = 20; _indices[i++] = 21; _indices[i++] = 22;
	_indices[i++] = 20;	_indices[i++] = 22; _indices[i++] = 23;
}

void Box::SetTransformationMatrix(Matrix* worldMat, Matrix* viewMat, Matrix* projMat)
{
	BaseObject::SetTransformationMatrix(worldMat, viewMat, projMat);

	_pos.x = _worldMat._41;
	_pos.y = _worldMat._42;
	_pos.z = _worldMat._43;

	_right.x = _worldMat._11;
	_right.y = _worldMat._12;
	_right.z = _worldMat._13;

	_up.x = _worldMat._21;
	_up.y = _worldMat._22;
	_up.z = _worldMat._23;

	_look.x = _worldMat._31;
	_look.y = _worldMat._32;
	_look.z = _worldMat._33;

	Matrix dirLineWorldMat;
	dirLineWorldMat.Scale(2.0f, 2.0f, 2.0f);
	dirLineWorldMat = dirLineWorldMat * _worldMat;
	if (_dirLine) _dirLine->SetTransformationMatrix(&dirLineWorldMat, viewMat, projMat);
}

void Box::Scale(float x, float y, float z)
{
	for (auto &vertex : _vertices)
	{
		vertex.p.x *= x;
		vertex.p.y *= y;
		vertex.p.z *= z;
	}
	UpdateVertexBuffer();
	UpdateConstantBuffer();
	SetTransformationMatrix(&_worldMat, &_viewMat, &_projMat);
}

bool Box::Init()
{
	_dirLine = new DirLine;
	_dirLine->Create(
		_device.Get(), _immediateContext.Get(),
		L"../../resource/shader/default3DShader.txt"
	);
	return true;
}

bool Box::Frame()
{
	return true;
}

bool Box::Render()
{
	BaseObject::Render();
	if (_showDirLine) 
	{
		_dirLine->Render();
	} 
	return true;
}

bool Box::Release()
{
	if (_dirLine) _dirLine->Release();
	delete _dirLine;
	BaseObject::Release();

	return true;
}