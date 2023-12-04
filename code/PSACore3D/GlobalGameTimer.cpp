#include "GlobalGameTimer.h"

float   g_gameTimer = 0.0f;
float   g_secondPerFrame = 0.0f;
UINT    g_FPS = 0;

bool GlobalGameTimer::Frame()
{
    Timer::Frame();
    _FPSCounter++;
    _FPSTimer += _elapseTime;
    if (_FPSTimer >= 1.0f)
    {
        g_FPS = _FPSCounter;
        _FPSCounter = 0;
        _FPSTimer -= 1.0f;
    }

    g_gameTimer = _timer;
    g_secondPerFrame = _elapseTime;
    return true;
}