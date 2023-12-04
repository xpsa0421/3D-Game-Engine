#include "Minimap.h"

bool Minimap::Create(ID3D11Device* device, ID3D11DeviceContext* context, FLOAT width, FLOAT height)
{
	_device = device;
	_immediateContext = context;
	_width = width;	_height = height;

	_renderTarget = std::make_shared<RenderTarget>();
	_plane = std::make_shared<BaseObject>();
	_renderTarget->Create(device, context, width, height);
	_plane->Create(device, context, L"../../resource/shader/defaultRTShader.txt");

	SetViewport(0, 0, width, height, 0, 1);
	CreateViewMatrix(Vector(0, 60, 0), Vector(0, 0, 0.001f), Vector(0, 1, 0));
	CreateProjMatrix(0.1f, 1000.0f, PI * 0.25f, width / height);

	return true;
}

void Minimap::SetViewport(FLOAT TopLeftX, FLOAT TopLeftY,
	FLOAT Width, FLOAT Height, FLOAT MinDepth, FLOAT MaxDepth)
{
	_vp.TopLeftX	= TopLeftX;
	_vp.TopLeftY	= TopLeftY;
	_vp.Width		= Width;
	_vp.Height		= Height;
	_vp.MinDepth	= MinDepth;
	_vp.MaxDepth	= MaxDepth;
	_renderTarget->_viewport = _vp;
}

void Minimap::SetViewport(D3D11_VIEWPORT vp)
{
	_vp = vp;
	_renderTarget->SetViewport(_vp.Width, _vp.Height);
}

bool Minimap::Frame()
{
	return true;
}

bool Minimap::PreRender()
{
	if (_renderTarget->Begin() == false)
	{
		_renderTarget->End();
		return false;
	}
	return true;
}

bool Minimap::PostRender()
{
	_renderTarget->End();
	return true;
}

bool Minimap::Render()
{
	_immediateContext->RSSetViewports(1, &_vp);
	_plane->_textureSRV = _renderTarget->_srv.Get();
	_plane->SetTransformationMatrix(nullptr, nullptr, nullptr);
	_plane->UpdateConstantBuffer();
	_plane->Render();
	return true;
}

bool Minimap::Release()
{
	return true;
}
