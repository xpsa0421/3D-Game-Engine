#include "Frustum.h"

// create a plane from three vertices
void Plane::Create(Vector v0, Vector v1, Vector v2)
{
	Vector edge0 = v1 - v0;
	Vector edge1 = v2 - v0;
	Vector normal = edge0.cross(edge1);
	normal.Normalise();

	a = normal.x;
	b = normal.y;
	c = normal.z;
	d = -(normal.dot(v0));
}

// create a plane from normal vector and one vertex
void Plane::Create(Vector normal, Vector  v0)
{
	normal.Normalise();
	a = normal.x;
	b = normal.y;
	c = normal.z;
	d = -(normal.dot(v0));
}

//------------------------------------------------------------
//------------------------------------------------------------

void Frustum::Create(ID3D11Device* device, ID3D11DeviceContext* context, Matrix* viewMat, Matrix* projMat)
{
	_vertices[0] = Vector(-1.0f, -1.0f, 0.0f);
	_vertices[1] = Vector(-1.0f,  1.0f, 0.0f);
	_vertices[2] = Vector( 1.0f,  1.0f, 0.0f);
	_vertices[3] = Vector( 1.0f, -1.0f, 0.0f);
	_vertices[4] = Vector(-1.0f, -1.0f, 1.0f);
	_vertices[5] = Vector(-1.0f,  1.0f, 1.0f);
	_vertices[6] = Vector( 1.0f,  1.0f, 1.0f);
	_vertices[7] = Vector( 1.0f, -1.0f, 1.0f);

	Matrix invViewProj = (*viewMat * *projMat).XMMatrixInverse();
	for (auto &vertex : _vertices)
	{
		vertex = Vector::XMVector3TransformCoord(vertex, invViewProj);
	}

	_planes[0].Create(_vertices[1], _vertices[5], _vertices[0]); // left
	_planes[1].Create(_vertices[3], _vertices[6], _vertices[2]); // right
	_planes[2].Create(_vertices[5], _vertices[2], _vertices[6]); // top
	_planes[3].Create(_vertices[0], _vertices[7], _vertices[3]); // bottom
	_planes[4].Create(_vertices[2], _vertices[1], _vertices[0]); // near
	_planes[5].Create(_vertices[5], _vertices[6], _vertices[4]); // far

	if (!_frustumBox) {
		_frustumBox = new Box;
		_frustumBox->Create(device, context, L"../../resource/shader/default3DShader.txt");
	}
	
	// front side
	_frustumBox->_vertices[0].p = _vertices[1];
	_frustumBox->_vertices[1].p = _vertices[2];
	_frustumBox->_vertices[2].p = _vertices[3];
	_frustumBox->_vertices[3].p = _vertices[0];

	// back side
	_frustumBox->_vertices[4].p = _vertices[6];
	_frustumBox->_vertices[5].p = _vertices[5];
	_frustumBox->_vertices[6].p = _vertices[4];
	_frustumBox->_vertices[7].p = _vertices[7];

	// right side
	_frustumBox->_vertices[8].p	= _vertices[2];
	_frustumBox->_vertices[9].p	= _vertices[6];
	_frustumBox->_vertices[10].p = _vertices[7];
	_frustumBox->_vertices[11].p = _vertices[3];

	// left side
	_frustumBox->_vertices[12].p = _vertices[5];
	_frustumBox->_vertices[13].p = _vertices[1];
	_frustumBox->_vertices[14].p = _vertices[0];
	_frustumBox->_vertices[15].p = _vertices[4];

	// top side
	_frustumBox->_vertices[16].p = _vertices[5];
	_frustumBox->_vertices[17].p = _vertices[6];
	_frustumBox->_vertices[18].p = _vertices[2];
	_frustumBox->_vertices[19].p = _vertices[1];

	// bottom side
	_frustumBox->_vertices[20].p = _vertices[0];
	_frustumBox->_vertices[21].p = _vertices[3];
	_frustumBox->_vertices[22].p = _vertices[7];
	_frustumBox->_vertices[23].p = _vertices[4];
	
	_frustumBox->_constantData.colour = Vector4(0, 1, 1, 0.3f);
	_frustumBox->UpdateVertexBuffer();
	_frustumBox->UpdateConstantBuffer();
}

bool Frustum::Render()
{
	_frustumBox->Render();
	return true;
}

POSITION Frustum::ClassifyPoint(Vector* point)
{
	for (int iPlane = 0; iPlane < 6; iPlane++)
	{
		float planeToCentre =
			_planes[iPlane].a * point->x +
			_planes[iPlane].b * point->y +
			_planes[iPlane].c * point->z +
			_planes[iPlane].d;
		if (planeToCentre < 0)
			return POSITION::BACK;
	}
	return POSITION::FRONT;
}

POSITION Frustum::ClassifySphere(BoundingSphere* sphere)
{
	for (int iPlane = 0; iPlane < 6; iPlane++)
	{
		float planeToCentre =
			_planes[iPlane].a * sphere->centre.x + 
			_planes[iPlane].b * sphere->centre.y + 
			_planes[iPlane].c * sphere->centre.z + 
			_planes[iPlane].d;
		if (planeToCentre <= -(sphere->radius))
			return POSITION::BACK;
	}
	return POSITION::FRONT;
}

POSITION Frustum::ClassifyBox(BoundingBox* box)
{
	Vector dir[3];
	float distance = 0.0f;
	for (int iPlane = 0; iPlane < 6; iPlane++)
	{
		for (int iAxis = 0; iAxis < 3; iAxis++)
		{
			dir[iAxis] = box->axes[iAxis] * box->extents[iAxis];
			distance += fabs(_planes[iPlane].a * dir[iAxis].x +
						_planes[iPlane].b * dir[iAxis].y + 
						_planes[iPlane].c * dir[iAxis].z);
		}

		float planeToCentre = 
			_planes[iPlane].a * box->centre.x +
			_planes[iPlane].b * box->centre.y +
			_planes[iPlane].c * box->centre.z +
			_planes[iPlane].d;

		if (planeToCentre > 0)
		{
			if (planeToCentre < distance)
			{
				return POSITION::SPANNING;
			}
		}
		else if (planeToCentre < 0)
		{
			if (planeToCentre > -distance)
			{
				return POSITION::SPANNING;
			}
			return POSITION::BACK;
		}
	}
	return POSITION::FRONT;
}

Frustum::~Frustum()
{
	if (_frustumBox) _frustumBox->Release();
	delete _frustumBox;
}