#pragma once
#include "Std.h"
#include "Collision.h"
#include "Box.h"

enum POSITION
{
	BACK,
	FRONT,
	ONPLANE,
	SPANNING
};

struct Plane
{
	float a, b, c, d;
	void  Create(Vector v0, Vector v1, Vector v2);
	void  Create(Vector normal, Vector  v0);
};

class Frustum
{
private:
	Matrix	_viewMat;
	Matrix	_projMat;
public:
	Vector	_vertices[8];
	Plane	_planes[6];
	Box*	_frustumBox = nullptr;
public:
	void		Create(ID3D11Device* device, ID3D11DeviceContext* context, Matrix* viewMat, Matrix* projMat);
	bool		Render();
	POSITION	ClassifyPoint(Vector* point);
	POSITION	ClassifySphere(BoundingSphere* sphere);
	POSITION	ClassifyBox(BoundingBox* box);
public:
	~Frustum();
};

