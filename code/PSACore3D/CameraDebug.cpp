#include "CameraDebug.h"

bool CameraDebug::Frame()
{
	if (S_Input.GetKey(VK_SPACE) == KEY_HOLD)
	{
		_speed += g_secondPerFrame * 100.0f;
	}
	else
	{
		_speed -= g_secondPerFrame * 100.0f;
	}
	_speed = max(10.0f, _speed); // min speed 10
	_speed = min(100.0f, _speed); // max speed 100

	if (S_Input.GetKey('W') == KEY_HOLD)
	{
		_pos += _look * _speed * g_secondPerFrame;
	}
	if (S_Input.GetKey('S') == KEY_HOLD)
	{
		_pos += _look * _speed * g_secondPerFrame;
	}
	if (S_Input.GetKey('A') == KEY_HOLD)
	{
		_pos += _right * _speed * g_secondPerFrame;
	}
	if (S_Input.GetKey('D') == KEY_HOLD)
	{
		_pos += _right * _speed * g_secondPerFrame;
	}
	if (S_Input.GetKey('Q') == KEY_HOLD)
	{
		_pos += _up * _speed * g_secondPerFrame;
	}
	if (S_Input.GetKey('E') == KEY_HOLD)
	{
		_pos += _up * _speed * g_secondPerFrame;
	}

	return true;
}