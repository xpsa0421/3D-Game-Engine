#pragma once
#include "Std.h"
#include "dxtk/WICTextureLoader.h"
#include "dxtk/DDSTextureLoader.h"
#pragma comment(lib, "DirectXTK.lib")

class Texture
{
public:
	ID3D11Texture2D*			_texture = nullptr;
	ID3D11ShaderResourceView*	_textureSRV		= nullptr;
	D3D11_TEXTURE2D_DESC				_desc;
public:
	HRESULT	Create(ID3D11Device* device, ID3D11DeviceContext* immediateContext, W_STR fileName);
	void	Apply(ID3D11DeviceContext* immediateContext, UINT slot = 0);
	bool	Release();
};

