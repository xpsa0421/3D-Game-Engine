#pragma once
#include "Shader.h"
#define S_ShaderManager ShaderManager::GetInstance()

class ShaderManager : public Singleton<ShaderManager>
{
private:
	friend class Singleton<ShaderManager>;
	std::map<C_STR, Shader*> _shaders;
public:
	ComPtr<ID3D11Device>		_device				= nullptr;
	ComPtr<ID3D11DeviceContext>	_immediateContext	= nullptr;
public:
	void		SetDevice(ID3D11Device* device, ID3D11DeviceContext* context);
	Shader*		LoadShader(W_STR shaderFileName, C_STR VSFuncName, C_STR PSFuncName);
	Shader*		FindShader(C_STR shaderName);
	bool		Release();
private:
	ShaderManager();
public:
	~ShaderManager();
};

