#pragma once
#include "Input.h"
#include "Frustum.h"
#include "Frustum.h"

class Camera : public BaseObject
{
public:
	std::shared_ptr<Frustum>	_frustum;
	Vector	_pos;
	Vector	_target		= { 0,0,0 };
	Vector	_look;
	Vector	_up			= { 0,1,0 };
	Vector	_right;
	float	_distance	= 0.0f;
	float	_speed		= 20.0f;
public:
	float	_zNear;
	float	_zFar;
	float	_fovH;
	float	_aspectRatio;
public:
	virtual bool	CreateFrustum(ID3D11Device* device, ID3D11DeviceContext* immediateContext);
	virtual void	CreateViewMatrix(Vector pos, Vector target, Vector up);
	virtual void	CreateProjMatrix(float zNear, float zFar, float fovH, float aspectRatio);
	virtual void	SetObjectView(Vector minV, Vector maxV);
public:
	void			MoveFrontBack(float amount);
	void			MoveLeftRight(float amount);
	void			MoveUpDown(float amount);
public:
	virtual void	UpdateCameraVector();
	virtual void	Update(float roll, float pitch, float yaw);
	virtual bool	Frame();
};

