#pragma once
#include <windows.h>
#include <algorithm>
#include <map>
#include <vector>
#include <list>
#include <set>
#include <string>
#include <d3d11.h>
#include <tchar.h>
#include <d3dcompiler.h>
#include <dxgidebug.h>
#include <codecvt>
#include <iterator>
#include <fstream>
#include <iostream>
#include <wrl.h>
#include "CoreMath.h"
#include "DxState.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")
#define GAMECORE
using namespace Microsoft::WRL;

extern HWND		g_wnd;
extern RECT		g_rectClient;
extern float	g_gameTimer;
extern float	g_secondPerFrame;
extern UINT		g_FPS;

#define GAME_START int APIENTRY wWinMain(HINSTANCE instance,HINSTANCE prevInstance, LPWSTR cmdLine, int cmdShow){
#define GAME_WINDOW(s,x,y) Sample demo;demo.SetWindow(instance,L#s,x,y);demo.Run(); return 1;
#define GAME_END    }
#define GAME_RUN(s,x,y)  GAME_START  GAME_WINDOW(s,x,y)  GAME_END

#define randf(x) (x*rand()/(float)RAND_MAX)
#define randf2(x,off) (off+x*rand()/(float)RAND_MAX)
#define randstep(fMin,fMax) (fMin+((float)fMax-(float)fMin)*rand()/(float)RAND_MAX)
#define clamp(x,MinX,MaxX) if (x>MaxX) x=MaxX; else if (x<MinX) x=MinX;S

typedef std::basic_string<TCHAR>				T_STR;
typedef std::basic_string<wchar_t>				W_STR;
typedef std::basic_string<char>					C_STR;
typedef std::string								STR;
typedef std::vector<std::basic_string<TCHAR>>	TSTR_VECTOR;
typedef std::vector<DWORD>						DWORD_VECTOR;

template<class T> class Singleton
{
public:
	static T& GetInstance()
	{
		static T theSingleInstance;
		return theSingleInstance;
	}
};

static W_STR mtw(STR str)
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;
	return conv.from_bytes(str);
}

static STR wtm(W_STR str)
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;
	return conv.to_bytes(str);
}

