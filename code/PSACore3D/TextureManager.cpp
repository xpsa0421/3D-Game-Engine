#include "TextureManager.h"

void TextureManager::SetDevice(ID3D11Device* device, ID3D11DeviceContext* context)
{
	_device = device;
	_immediateContext = context;
}

Texture* TextureManager::LoadTexture(W_STR fileName)
{
	HRESULT result;
	Texture* texture = FindTexture(fileName);

	if (texture != nullptr)
	{
		return texture;
	}
	else
	{
		texture = new Texture;
		result = texture->Create(_device.Get(), _immediateContext.Get(), fileName);
		if (SUCCEEDED(result))
		{
			_textures.insert(std::make_pair(fileName, texture));
			return texture;
		}
		delete texture;
	}
	return nullptr;
}

Texture* TextureManager::FindTexture(W_STR fileName)
{
	auto texture = _textures.find(fileName);
	if (texture != _textures.end())
		return texture->second;
	else 
		return nullptr;
}

bool TextureManager::Release()
{
	for (auto texture : _textures)
	{
		Texture* t = texture.second;
		if (t) t->Release();
		delete t;
	}
	_textures.clear();
	return true;
}

TextureManager::TextureManager()
{
}

TextureManager::~TextureManager()
{
	Release(); 
}