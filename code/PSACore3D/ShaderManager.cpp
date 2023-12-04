#include "ShaderManager.h"

void ShaderManager::SetDevice(ID3D11Device* device, ID3D11DeviceContext* context)
{
	_device = device;
	_immediateContext = context;
}

Shader* ShaderManager::LoadShader(W_STR shaderFileName, C_STR VSFuncName, C_STR PSFuncName)
{
	HRESULT result;
	C_STR shaderName = wtm(shaderFileName) + VSFuncName + PSFuncName;
	Shader* shader = FindShader(shaderName);

	if (shader != nullptr)
	{
		return shader;
	}
	else
	{
		shader = new Shader;
		result = shader->Create(_device.Get(), _immediateContext.Get(), shaderFileName, VSFuncName, PSFuncName);
		if (SUCCEEDED(result))
		{
			_shaders.insert(std::make_pair(shaderName, shader));
		}
		return shader;
	}
}

Shader* ShaderManager::FindShader(C_STR shaderName)
{
	auto shader = _shaders.find(shaderName);
	if (shader != _shaders.end())
		return shader->second;
	else
		return nullptr;
}

bool ShaderManager::Release()
{
	for (auto shader : _shaders)
	{
		Shader* s = shader.second;
		if (s) s->Release();
		delete s;
		s = nullptr;
	}
	_shaders.clear();
	return true;
}

ShaderManager::ShaderManager()
{
}

ShaderManager::~ShaderManager()
{
	Release();
}