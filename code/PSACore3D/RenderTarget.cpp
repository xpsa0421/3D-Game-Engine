#include "RenderTarget.h"

void RenderTarget::SetViewport(FLOAT width, FLOAT height)
{
	_viewport.Width		= width;
	_viewport.Height	= height;
	_viewport.TopLeftX	= 0.0f;
	_viewport.TopLeftY	= 0.0f;
	_viewport.MinDepth	= 0.0f;
	_viewport.MaxDepth	= 1.0f;
}

HRESULT RenderTarget::CreateRenderTargetView(ID3D11Device* device, FLOAT width, FLOAT height)
{
	HRESULT hr;

	_texDesc.Width				= (UINT)width;
	_texDesc.Height				= (UINT)height;
	_texDesc.MipLevels			= 1;
	_texDesc.ArraySize			= 1;
	_texDesc.Format				= DXGI_FORMAT_R8G8B8A8_UNORM;
	_texDesc.SampleDesc.Count	= 1;
	_texDesc.SampleDesc.Quality = 0;
	_texDesc.Usage				= D3D11_USAGE_DEFAULT;
	_texDesc.BindFlags			= D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	_texDesc.CPUAccessFlags		= 0;
	_texDesc.MiscFlags			= 0;
	
	if (FAILED(hr = device->CreateTexture2D(&_texDesc, nullptr, _texture.GetAddressOf())))
	{
		return hr;
	}
	if (FAILED(hr = device->CreateShaderResourceView(_texture.Get(), nullptr, _srv.GetAddressOf())))
	{
		return hr;
	}
	if (FAILED(hr = device->CreateRenderTargetView(_texture.Get(), nullptr, _rtv.GetAddressOf())))
	{
		return hr;
	}
	return S_OK;
}

HRESULT RenderTarget::CreateDepthStencilView(ID3D11Device* device, FLOAT width, FLOAT height)
{
	HRESULT hr;
	ComPtr<ID3D11Texture2D> dsTexture = nullptr;
	D3D11_TEXTURE2D_DESC dsTexDesc;
	dsTexDesc.Width					= (UINT)width;
	dsTexDesc.Height				= (UINT)height;
	dsTexDesc.MipLevels				= 1;
	dsTexDesc.ArraySize				= 1;
	dsTexDesc.Format				= DXGI_FORMAT_R24G8_TYPELESS;
	dsTexDesc.SampleDesc.Count		= 1;
	dsTexDesc.SampleDesc.Quality	= 0;
	dsTexDesc.Usage					= D3D11_USAGE_DEFAULT;
	dsTexDesc.BindFlags				= D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	dsTexDesc.CPUAccessFlags		= 0;
	dsTexDesc.MiscFlags				= 0;	
	if (FAILED(hr = device->CreateTexture2D(&dsTexDesc, nullptr, &dsTexture)))
	{
		return hr;
	}

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	ZeroMemory(&dsvDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	dsvDesc.ViewDimension	= D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Format			= DXGI_FORMAT_D24_UNORM_S8_UINT;
	if (FAILED(hr = device->CreateDepthStencilView(dsTexture.Get(), &dsvDesc, _dsv.GetAddressOf())))
	{
		return hr;
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC dsvSrvDesc;
	ZeroMemory(&dsvSrvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	dsvSrvDesc.ViewDimension		= D3D11_SRV_DIMENSION_TEXTURE2D;
	dsvSrvDesc.Texture2D.MipLevels	= 1;
	dsvSrvDesc.Format				= DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	if (FAILED(hr = device->CreateShaderResourceView(dsTexture.Get(), &dsvSrvDesc, _dsvSrv.GetAddressOf())))
	{
		return hr;
	}

	return S_OK;
}

HRESULT RenderTarget::Create(ID3D11Device* device,
	ID3D11DeviceContext* immediateContext, FLOAT width, FLOAT height)
{
	HRESULT hr;
	_immediateContext = immediateContext;
	SetViewport(width, height);

	if (FAILED(hr = CreateRenderTargetView(device, width, height)))
	{
		return hr;
	}
	if (FAILED(hr = CreateDepthStencilView(device, width, height)))
	{
		return hr;
	}

	return S_OK;
}

bool RenderTarget::Begin(Float4 colour)
{
	const FLOAT clearColour[4] = { colour.x, colour.y, colour.z, colour.w };
	ComPtr<ID3D11RenderTargetView> nullRtv = NULL;

	_immediateContext->RSGetViewports(&_numVp, _oldVp);
	_immediateContext->OMGetRenderTargets(1, _oldRtv.GetAddressOf(), _oldDsv.GetAddressOf());
	_immediateContext->OMSetRenderTargets(1, nullRtv.GetAddressOf(), NULL);
	_immediateContext->OMSetRenderTargets(1, _rtv.GetAddressOf(), _dsv.Get());
	_immediateContext->ClearRenderTargetView(_rtv.Get(), clearColour);
	_immediateContext->ClearDepthStencilView(_dsv.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0, 0);
	_immediateContext->RSSetViewports(1, &_viewport);

	return true;
}

void RenderTarget::End()
{
	_immediateContext->OMSetRenderTargets(1, _oldRtv.GetAddressOf(), _oldDsv.Get());
	_immediateContext->RSSetViewports(1, _oldVp);
}

bool RenderTarget::Release()
{
	return true;
}