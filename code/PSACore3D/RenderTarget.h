#pragma once
#include "BaseObject.h"

class RenderTarget
{
public:
	ComPtr<ID3D11DeviceContext>			_immediateContext	= nullptr;
	ComPtr<ID3D11RenderTargetView>		_rtv				= nullptr;
	ComPtr<ID3D11DepthStencilView>		_dsv				= nullptr;
	ComPtr<ID3D11ShaderResourceView>	_srv				= nullptr;
	ComPtr<ID3D11ShaderResourceView>	_dsvSrv				= nullptr;
	ComPtr<ID3D11Texture2D>				_texture			= nullptr;
	D3D11_DEPTH_STENCIL_VIEW_DESC		_dsvDesc;
	D3D11_TEXTURE2D_DESC				_texDesc;
	D3D11_VIEWPORT						_viewport;
public:
	UINT							_numVp = 1;
	ComPtr<ID3D11RenderTargetView>	_oldRtv	= nullptr;
	ComPtr<ID3D11DepthStencilView>	_oldDsv	= nullptr;
	D3D11_VIEWPORT					_oldVp[D3D11_VIEWPORT_AND_SCISSORRECT_MAX_INDEX];
public:
	void	SetViewport(FLOAT width, FLOAT height);
	HRESULT	CreateRenderTargetView(ID3D11Device* device, FLOAT width, FLOAT height);
	HRESULT	CreateDepthStencilView(ID3D11Device* device, FLOAT width, FLOAT height);
	HRESULT	Create(ID3D11Device* device,
				ID3D11DeviceContext* immediateContext,
				FLOAT width = 1024.0f, FLOAT height = 1024.0f);
	bool	Begin(Float4 colour = { 1,1,1,1 });
	void	End();
	bool	Release();
	

};

