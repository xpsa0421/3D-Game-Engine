#pragma once
#include "Texture.h"
#define S_TexManager TextureManager::GetInstance()

class TextureManager : public Singleton<TextureManager>
{
private:
	friend class Singleton<TextureManager>;
	std::map<W_STR, Texture*> _textures;
public:
	ComPtr<ID3D11Device>		_device				= nullptr;
	ComPtr<ID3D11DeviceContext>	_immediateContext	= nullptr;
public:
	void		SetDevice(ID3D11Device* device, ID3D11DeviceContext* context);
	Texture*	LoadTexture(W_STR fileName);
	Texture*	FindTexture(W_STR fileName);
	bool		Release();
private:
	TextureManager();
public:
	~TextureManager();
};

