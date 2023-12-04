#pragma once
#include "Std.h"

class DxState
{
public:
	static ID3D11SamplerState*		g_SSSmooth;
	static ID3D11SamplerState*		g_SSWrap;
	static ID3D11SamplerState*		g_SSMirror;
	static ID3D11SamplerState*		g_SSBorder;
public:
	static ID3D11BlendState*		g_BSAlpha;
	static ID3D11BlendState*		g_BSNoAlpha;
	static ID3D11BlendState*		g_BSOneZero;
public:
	static ID3D11RasterizerState*	g_RSWireFrame;
	static ID3D11RasterizerState*	g_RSSolid;
public:
	static ID3D11DepthStencilState* g_DSDepthDisable;
	static ID3D11DepthStencilState* g_DSDepthEnable;
	static ID3D11DepthStencilState* g_DSDepthGreater;
public:
	static bool SetState(ID3D11Device* device);
	static bool Release();
};
