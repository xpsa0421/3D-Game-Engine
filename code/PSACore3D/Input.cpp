#include "Input.h"

DWORD Input::GetKey(DWORD key)
{
	return _keyStates[key];
}

bool Input::Init()
{
	ZeroMemory(_keyStates, sizeof(DWORD) * 256);
	GetCursorPos(&_mousePos);
	ScreenToClient(g_wnd, &_mousePos);
	_mousePosPrev = _mousePos;
	return true;
}

bool Input::Frame()
{
	GetCursorPos(&_mousePos);
	ScreenToClient(g_wnd, &_mousePos);
	_mouseOffset.x = _mousePos.x - _mousePosPrev.x;
	_mouseOffset.y = _mousePos.y - _mousePosPrev.y;

	for (int i = 0; i < 256; i++)
	{
		SHORT keyState = GetAsyncKeyState(i);

		if (keyState & 0x8000) // if pressed
		{
			if (_keyStates[i] == KEY_FREE || _keyStates[i] == KEY_UP)
				_keyStates[i] = KEY_DOWN;
			else
				_keyStates[i] = KEY_HOLD;
		}
		else
		{
			if (_keyStates[i] == KEY_DOWN || _keyStates[i] == KEY_HOLD)
				_keyStates[i] = KEY_UP;
			else
				_keyStates[i] = KEY_FREE;
		}
	}

	_mousePosPrev = _mousePos;
	return true;
}

bool Input::Render()
{
	return true;
}

bool Input::Release()
{
	return true;
}