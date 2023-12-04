#include "Camera.h"

bool Camera::CreateFrustum(ID3D11Device* device, ID3D11DeviceContext* immediateContext)
{
	_device = device;
	_immediateContext = immediateContext;
	_frustum = std::make_shared<Frustum>();
	_frustum->Create(_device.Get(), _immediateContext.Get(), &_viewMat, &_projMat);
	return true;
}

void Camera::CreateViewMatrix(Vector pos, Vector target, Vector up)
{
	_pos = pos;
	_target = target;
	_up = up;
	_distance = (_pos - _target).GetLength();
	_viewMat.ViewLookAtCC(pos, target, up);

	Matrix mInvView = _viewMat.XMMatrixInverse();
	Vector pZBasis = Vector(mInvView._31, mInvView._32, mInvView._33);
	float fLen = sqrtf(pZBasis.z * pZBasis.z + pZBasis.x * pZBasis.x);

	_yaw = atan2f(pZBasis.x, pZBasis.z);
	_pitch = -atan2f(pZBasis.y, fLen);
}

void Camera::CreateProjMatrix(float zNear, float zFar, float fovH, float aspectRatio)
{
	_zNear = zNear;
	_zFar = zFar;
	_fovH = fovH;
	_aspectRatio = aspectRatio;
	_projMat.PerspectiveFovLH(zNear, zFar, fovH, aspectRatio);
}

void Camera::SetObjectView(Vector minV, Vector maxV)
{
	Vector up = Vector(0.0f, 1.0f, 0.0f);
	float radius = (maxV - minV).GetLength() * 0.5f;
	float distance = radius / tanf(_fovH * 0.5f);
	
	_target = (minV + maxV) / 2.0f;
	_look = (_target - _pos).GetNormal();
	Vector v = _look * (distance * 1.5f);
	_pos = _target - v;
	_distance = (_pos - _target).GetLength();
	
	_viewMat.XMMatrixLookAt(_pos, _target, up);
}

void Camera::MoveFrontBack(float amount)
{
	_pos += _look * amount;
}

void Camera::MoveLeftRight(float amount)
{
	_pos += _right * amount;
}

void Camera::MoveUpDown(float amount)
{
	_pos += _up * amount;
}

void Camera::UpdateCameraVector()
{
	_right.x = _viewMat._11;
	_right.y = _viewMat._21;
	_right.z = _viewMat._31;

	_up.x = _viewMat._12;
	_up.y = _viewMat._22;
	_up.z = _viewMat._32;

	_look.x = _viewMat._13;
	_look.y = _viewMat._23;
	_look.z = _viewMat._33;

	_right.Normalise();
	_up.Normalise();
	_look.Normalise();

	_frustum->Create(_device.Get(), _immediateContext.Get(), &_viewMat, &_projMat);
}

void Camera::Update(float roll, float pitch, float yaw)
{
	Quaternion rotQuat;
	Quaternion::XMQuaternionRotationRollPitchYaw(&rotQuat, roll, pitch, yaw);

	Matrix rotMat;
	rotMat = Matrix::XMMatrixAffineTransformation(
		Vector(1.0f, 1.0f, 1.0f), nullptr, &rotQuat, &_pos);
	_viewMat = rotMat.XMMatrixInverse();

	UpdateCameraVector();
	return;
}

bool Camera::Frame()
{
	// view direction control
	if (S_Input.GetKey(VK_LBUTTON) == KEY_HOLD)
	{
		_yaw += S_Input._mouseOffset.x * 0.002f;
		_pitch += S_Input._mouseOffset.y * 0.002f;
	}

	// move speed control
	if (S_Input.GetKey(VK_SPACE) == KEY_HOLD)
	{
		_speed += g_secondPerFrame * 100.0f;
		_speed = min(100.0f, _speed);
	}
	else
	{
		_speed -= g_secondPerFrame * 100.0f;
		_speed = max(20.0f, _speed);
	}

	// move distance control
	if (S_Input.GetKey('W') == KEY_HOLD)
	{
		MoveFrontBack(g_secondPerFrame * _speed);
	}
	if (S_Input.GetKey('S') == KEY_HOLD)
	{
		MoveFrontBack(g_secondPerFrame * -_speed);
	}
	if (S_Input.GetKey('A') == KEY_HOLD)
	{
		MoveLeftRight(g_secondPerFrame * -_speed);
	}
	if (S_Input.GetKey('D') == KEY_HOLD)
	{
		MoveLeftRight(g_secondPerFrame * _speed);
	}
	if (S_Input.GetKey('Q') == KEY_HOLD)
	{
		MoveUpDown(g_secondPerFrame * -_speed);
	}
	if (S_Input.GetKey('E') == KEY_HOLD)
	{
		MoveUpDown(g_secondPerFrame * _speed);
	}

	Update(_roll, _pitch, _yaw);
	UpdateCameraVector();

	return true;
}
