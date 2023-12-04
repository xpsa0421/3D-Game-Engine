#pragma once
#include <vector>
#include "CoreMath.h"

enum CollisionType
{
	RECT_OUT,
	RECT_IN,
	RECT_OVERLAP,
};

struct BoundingCircle
{
	float cx, cy;
	float r;

	BoundingCircle();
	BoundingCircle(float x, float y, float r);
	void Set(float cx, float cy, float w, float h);
	void Set(Vector2 pos, Vector2 size);
};

struct BoundingRect
{
	float x1, x2, y1, y2;
	float w, h;
	float cx, cy;

	bool operator == (BoundingRect& rect);
	BoundingRect();
	BoundingRect(float x, float y, float w, float h);
	void Set(float x, float y, float w, float h);
	void Set(Vector2 pos, Vector2 size);
};

struct BoundingSphere
{
	Vector	centre;
	float	radius;
	BoundingSphere() {
		centre = { 0,0,0 };
		radius = 1;
	};
	BoundingSphere(Vector c, float r) {
		centre = c;
		radius = r;
	};
};

// can be used to express AABB and OBB
struct BoundingBox
{
	Vector centre;
	Vector minVertex, maxVertex;
	Vector axes[3];
	float  extents[3];
	
	BoundingBox() {};
	BoundingBox(Vector minV, Vector maxV)
	{
		minVertex = minV;
		maxVertex = maxV;
		centre = (minVertex + maxVertex) * 0.5f;

		axes[0] = { 1,0,0 };
		axes[1] = { 0,1,0 };
		axes[2] = { 0,0,1 };

		extents[0] = maxVertex.x - centre.x;
		extents[1] = maxVertex.y - centre.y;
		extents[2] = maxVertex.z - centre.z;
	}
	BoundingBox(Vector minV, Vector maxV, Vector axes[3]) : BoundingBox(minV, maxV)
	{
		this->axes[0] = axes[0];
		this->axes[1] = axes[1];
		this->axes[2] = axes[2];
	}
};

struct BoundingAABB
{
	Vector centre; 
	Vector minVertex, maxVertex;
};

struct BoundingOBB
{
	Vector centre;
	Vector axis[3];
	float  extent[3];
};

class Collision
{
public:
	static CollisionType	RectToRect(BoundingRect& a, BoundingRect& b);
	static bool				RectToInRect(BoundingRect& a, BoundingRect& b);
	static bool				CircleToCircle(BoundingCircle& a, BoundingCircle& b);
	static bool				RectToPoint(BoundingRect& a, POINT& p);
public:
	static CollisionType	BoxToBox(BoundingBox& a, BoundingBox& b);
	static bool             BoxToInBox(BoundingBox& a, BoundingBox& b);
	static bool             SphereToSphere(BoundingSphere& a, BoundingSphere& b);
};

static BoundingRect ConvertRect(RECT rt)
{
	BoundingRect tRt;
	tRt.x1 = rt.left;
	tRt.y1 = rt.top;
	tRt.w = rt.right;
	tRt.h = rt.bottom;
	return tRt;
}