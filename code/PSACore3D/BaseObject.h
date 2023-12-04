#pragma once
#include "TextureManager.h"
#include "ShaderManager.h"
#include "Collision.h"

struct Vertex
{
	Vector	p;
	Vector	n;
	Vector4 c;
	Vector2 t;
	Vertex() {};
	Vertex(Vector point, Vector normal, Vector4 colour, Vector2 textureUV)
	{
		p = point;
		n = normal;
		c = colour;
		t = textureUV;
	}
};

struct VertexIW
{
	Vector4 i;
	Vector4 w;
	VertexIW() {};
	VertexIW(Vector4 index, Vector4 weight)
	{
		i = index;
		w = weight;
	}
};

struct VSConstantData
{
	Matrix	worldMat;
	Matrix	viewMat;
	Matrix	projMat;
	Vector4 colour;
	Vector4 light;
};

struct VSBoneConstantData
{
	Matrix	animMat[255];
};

class BaseObject
{
public:
	ComPtr<ID3D11Device>				_device				= nullptr;
	ComPtr<ID3D11DeviceContext>			_immediateContext	= nullptr;
	ComPtr<ID3D11Buffer>				_constantBuffer		= nullptr;
	ComPtr<ID3D11Buffer>				_indexBuffer		= nullptr;
	ComPtr<ID3D11Buffer>				_vertexBuffer		= nullptr;
	ComPtr<ID3D11InputLayout>			_vertexLayout		= nullptr;
	Texture*							_texture			= nullptr;
	ID3D11ShaderResourceView*			_textureSRV			= nullptr;
	Shader*								_shader				= nullptr;
	BoundingRect						_collider;
	VSConstantData						_constantData;
	std::vector<Vertex>					_initVertices;
	std::vector<Vertex>					_vertices;
	std::vector<DWORD>					_indices;
public:
	Matrix	_worldMat;
	Matrix	_viewMat;
	Matrix	_projMat;
public:
	float	_yaw	= 0.0f;	// µµ¸®µµ¸®
	float	_pitch	= 0.0f;	// ²ô´ö²ô´ö
	float	_roll	= 0.0f;	// °¼¿ì¶×
public:
	bool	_showDirLine	= false;
	W_STR	_name			= L"";
	DWORD	_numFaces		= 0;
public:;
	virtual void			CreateConstantData();
	virtual void			CreateVertexData();
	virtual void			CreateIndexData();
	virtual HRESULT			CreateConstantBuffer();
	virtual HRESULT			CreateVertexBuffer();
	virtual HRESULT			CreateIndexBuffer();
	virtual HRESULT			CreateVertexLayout();
	static ID3D11Buffer*	CreateVertexBuffer(ID3D11Device* device, void* dataAddress, UINT numVertices, UINT vertexSize);
public:
	virtual bool	LoadShader(W_STR shaderFileName, C_STR VSFuncName, C_STR PSFuncName);
	virtual bool	LoadTexture(W_STR texFileName);
public:
	virtual bool	Create( ID3D11Device* device, ID3D11DeviceContext* immediateContext,
					const W_STR shaderFileName, const W_STR texFileName = L"",
					const C_STR VSFuncName = "VSMain", const C_STR PSFuncName = "PSMain");
	virtual void	UpdateConstantBuffer();
	virtual void	UpdateVertexBuffer();
	virtual void	SetTransformationMatrix(Matrix* worldMat, Matrix* viewMat, Matrix* projMat);
	virtual void	GetMinMaxV(Vector& objMinV, Vector& objMaxV);
public:
	virtual bool	Init();
	virtual bool	Frame();
	virtual bool	PreRender();
	virtual bool	PostRender();
	virtual bool	Render();
	virtual bool	Release();
};