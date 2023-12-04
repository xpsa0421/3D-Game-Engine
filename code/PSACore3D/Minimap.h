#pragma once
#include "Camera.h"
#include "RenderTarget.h"

class Minimap : public Camera
{
private:
	D3D11_VIEWPORT	_vp;
	std::shared_ptr<RenderTarget>	_renderTarget;
	std::shared_ptr<BaseObject>		_plane;
	FLOAT			_width;
	FLOAT			_height;
public:
	bool	Create(ID3D11Device* device, ID3D11DeviceContext* context, FLOAT width, FLOAT height);
	void	SetViewport(FLOAT TopLeftX, FLOAT TopLeftY, 
						FLOAT Width, FLOAT Height, FLOAT MinDepth, FLOAT MaxDepth);
	void	SetViewport(D3D11_VIEWPORT vp);
	bool	Frame()			override;
	bool	PreRender()		override;
	bool	PostRender()	override;
	bool	Render()		override;
	bool	Release()		override;
};

