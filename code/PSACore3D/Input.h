#pragma once
#include "Std.h"
#define S_Input Input::GetInstance()

enum KeyState
{
	KEY_FREE = 0,
	KEY_UP,
	KEY_DOWN,
	KEY_HOLD
};

class Input : public Singleton<Input>
{
private:
	DWORD _keyStates[256];
public:
	POINT	_mousePos;
	POINT	_mousePosPrev;
	POINT	_mouseOffset;
public:
	virtual bool	Init();
	virtual bool	Frame();
	virtual bool	Render();
	virtual bool	Release();
public:
	DWORD	GetKey(DWORD key);
};

