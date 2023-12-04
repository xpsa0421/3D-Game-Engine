#pragma once
#include "Window.h"

class Device : public Window
{
public:
	ComPtr<ID3D11Device>			_device				= nullptr;
	ComPtr<ID3D11DeviceContext>		_immediateContext	= nullptr;
	ComPtr<IDXGIFactory>			_factory			= nullptr;
	ComPtr<IDXGISwapChain>			_swapChain			= nullptr;
	ComPtr<ID3D11RenderTargetView>	_rtv				= nullptr;
	ComPtr<ID3D11DepthStencilView>	_dsv				= nullptr;
	D3D11_VIEWPORT					_vp;
public:
	HRESULT		CreateDevice();
	HRESULT		CreateDXGIDevice();
	HRESULT		CreateSwapChain();
	HRESULT		CreateRenderTargetView();
	HRESULT		CreateDepthStencilView();
	void		CreateViewport();
	HRESULT		CreateDXResource();
	HRESULT		DeleteDXResource();
	HRESULT		ResizeDevice(UINT width, UINT height);
public:
	virtual bool		Init();
	virtual bool		Frame();
	virtual bool		Render();
	virtual bool		Release();
};
