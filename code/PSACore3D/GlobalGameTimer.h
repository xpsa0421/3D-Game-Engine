#pragma once
#include "Timer.h"
#define S_GameTimer GlobalGameTimer::GetInstance()

class GlobalGameTimer : public Timer, public Singleton<GlobalGameTimer>
{
private:
	float	_FPSTimer;
	UINT	_FPSCounter;
public:
	virtual bool	Frame();
};

