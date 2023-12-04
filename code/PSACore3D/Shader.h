#pragma once
#include "Std.h"

class Shader
{
public:
	ComPtr<ID3D11Device>		_device				= nullptr;
	ComPtr<ID3D11DeviceContext>	_immediateContext	= nullptr;
public:
	ComPtr<ID3D11VertexShader>		_vertexShader		= nullptr;
	ComPtr<ID3D11PixelShader>		_pixelShader		= nullptr;
	ComPtr<ID3DBlob>				_VSCode				= nullptr;
	ComPtr<ID3DBlob>				_PSCode				= nullptr;
public:
	bool	Init();
	bool	Frame();
	bool	Render();
	bool	Release();
public:
	HRESULT Create(
		ID3D11Device* device, ID3D11DeviceContext* immediateContext, W_STR shaderFileName, C_STR VSFuncName, C_STR PSFuncName);
	HRESULT CreateVertexShader(W_STR VSFileName, C_STR funcName);
	HRESULT CreatePixelShader(W_STR PSFileName, C_STR funcName);
};

