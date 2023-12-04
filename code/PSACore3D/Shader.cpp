#include "Shader.h"

bool Shader::Init()
{
	return true;
}

bool Shader::Frame()
{
	return true;
}

bool Shader::Render()
{
	return true;
}

bool Shader::Release()
{
	_pixelShader	= nullptr;
	_vertexShader	= nullptr;
	_VSCode			= nullptr;
	_PSCode			= nullptr;

	return true;
}

HRESULT Shader::Create(
		ID3D11Device* device, ID3D11DeviceContext* immediateContext,
		W_STR shaderFileName, C_STR VSFuncName, C_STR PSFuncName)
{
	_device = device;
	_immediateContext = immediateContext;

	HRESULT VSResult = CreateVertexShader(shaderFileName, VSFuncName);
	HRESULT PSResult = CreatePixelShader(shaderFileName, PSFuncName);

	if (SUCCEEDED(VSResult) && SUCCEEDED(PSResult))
		return S_OK;
	else
		return E_FAIL;
}

HRESULT	Shader::CreateVertexShader(W_STR fileName, C_STR funcName)
{
	HRESULT result;
	ComPtr<ID3DBlob> errorCode = nullptr;
	DWORD shaderFlags = D3DCOMPILE_SKIP_OPTIMIZATION;
#if defined( _DEBUG ) 
	shaderFlags |= D3DCOMPILE_DEBUG;
#endif

	result = D3DCompileFromFile(
		fileName.c_str(),
		NULL,
		NULL,
		funcName.c_str(),
		"vs_5_0",
		shaderFlags,
		0,
		_VSCode.GetAddressOf(),
		errorCode.GetAddressOf()
	);
	if (FAILED(result))
	{
		if (errorCode)
		{
			OutputDebugStringA((char*)errorCode->GetBufferPointer());
		}
		return E_FAIL;
	}

	result = _device->CreateVertexShader(
		_VSCode->GetBufferPointer(),
		_VSCode->GetBufferSize(),
		NULL,
		_vertexShader.GetAddressOf()
	);

	return result;
}

HRESULT	Shader::CreatePixelShader(W_STR fileName, C_STR funcName)
{
	HRESULT result;
	ComPtr<ID3DBlob> errorCode = nullptr;
	DWORD shaderFlags = D3DCOMPILE_SKIP_OPTIMIZATION;
#if defined( _DEBUG ) 
	shaderFlags |= D3DCOMPILE_DEBUG;
#endif

	result = D3DCompileFromFile(
		fileName.c_str(),
		NULL,
		NULL,
		funcName.c_str(),
		"ps_5_0",
		shaderFlags,
		0,
		_PSCode.GetAddressOf(),
		errorCode.GetAddressOf()
	);
	if (FAILED(result))
	{
		if (errorCode)
		{
			OutputDebugStringA((char*)errorCode->GetBufferPointer());
		}
		return E_FAIL;
	}

	result = _device->CreatePixelShader(
		_PSCode->GetBufferPointer(),
		_PSCode->GetBufferSize(),
		NULL,
		_pixelShader.GetAddressOf()
	);
	if (FAILED(result)) return E_FAIL;

	return result;
}