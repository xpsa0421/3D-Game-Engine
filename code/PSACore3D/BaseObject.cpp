#include "BaseObject.h"

/*----------------------------------------------------------
|	OBJECT CREATION                                         |
 -----------------------------------------------------------*/

void BaseObject::CreateConstantData()
{
	_constantData.worldMat.Transpose();
	_constantData.viewMat.Transpose();
	_constantData.projMat.Transpose();
	_constantData.colour	= { 1,1,1,1 };
	_constantData.light		= { 0,0,0,0 };
}

void BaseObject::CreateVertexData()
{
	if (_vertices.size() == 0)
	{
		_vertices.resize(4);

		_vertices[0].p = { -1.0f, 1.0f, 0.0f };
		_vertices[0].n = { 0.0f, 0.0f, -1.0f };
		_vertices[0].c = { 1.0f, 1.0f, 1.0f, 1.0f };
		_vertices[0].t = { 0.0f, 0.0f };

		_vertices[1].p = { 1.0f, 1.0f, 0.0f };
		_vertices[1].n = { 0.0f, 0.0f, -1.0f };
		_vertices[1].c = { 1.0f, 1.0f, 1.0f, 1.0f };
		_vertices[1].t = { 1.0f, 0.0f };

		_vertices[2].p = { -1.0f, -1.0f, 0.0f };
		_vertices[2].n = { 0.0f, 0.0f, -1.0f };
		_vertices[2].c = { 1.0f, 1.0f, 1.0f, 1.0f };
		_vertices[2].t = { 0.0f, 1.0f };

		_vertices[3].p = { 1.0f, -1.0f, 0.0f };
		_vertices[3].n = { 0.0f, 0.0f, -1.0f };
		_vertices[3].c = { 1.0f, 1.0f, 1.0f, 1.0f };
		_vertices[3].t = { 1.0f, 1.0f };
	}
	
	_initVertices = _vertices;
}

void BaseObject::CreateIndexData()
{
	if (_indices.size() == 0)
	{
		_indices.resize(6);

		_indices[0] = 0;
		_indices[1] = 1;
		_indices[2] = 2;
		_indices[3] = 2;
		_indices[4] = 1;
		_indices[5] = 3;
	}
}

HRESULT	BaseObject::CreateConstantBuffer()
{
	CreateConstantData();

	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.ByteWidth	= sizeof(VSConstantData) * 1;
	bufferDesc.Usage		= D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags	= D3D11_BIND_CONSTANT_BUFFER;

	D3D11_SUBRESOURCE_DATA subRsrcData;
	ZeroMemory(&subRsrcData, sizeof(subRsrcData));
	subRsrcData.pSysMem = &_constantData;

	HRESULT result = _device->CreateBuffer(
		&bufferDesc,
		&subRsrcData,
		&_constantBuffer
	);
	return result;
}

HRESULT	BaseObject::CreateVertexBuffer()
{
	CreateVertexData();

	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.ByteWidth	= sizeof(Vertex) * UINT(_vertices.size());
	bufferDesc.Usage		= D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags	= D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA subRsrcData;
	ZeroMemory(&subRsrcData, sizeof(subRsrcData));
	subRsrcData.pSysMem = &_vertices.at(0);

	HRESULT result = _device->CreateBuffer(
		&bufferDesc,
		&subRsrcData,
		&_vertexBuffer
	);
	return result;
}

HRESULT	BaseObject::CreateIndexBuffer()
{
	CreateIndexData();

	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.ByteWidth = sizeof(DWORD) * UINT(_indices.size());
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA subRsrcData;
	ZeroMemory(&subRsrcData, sizeof(subRsrcData));
	subRsrcData.pSysMem = &_indices.at(0);

	HRESULT result = _device->CreateBuffer(
		&bufferDesc,
		&subRsrcData,
		&_indexBuffer
	);
	return result;
}

HRESULT	BaseObject::CreateVertexLayout()
{
	D3D11_INPUT_ELEMENT_DESC elementDesc[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "TEXTURE", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 40, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	HRESULT result = _device->CreateInputLayout(
		elementDesc,
		sizeof(elementDesc) / sizeof(elementDesc[0]),
		_shader->_VSCode->GetBufferPointer(),
		_shader->_VSCode->GetBufferSize(),
		&_vertexLayout
	);
	return result;
}

bool BaseObject::LoadShader(W_STR shaderFileName, C_STR VSFuncName, C_STR PSFuncName)
{
	_shader = S_ShaderManager.LoadShader(shaderFileName, VSFuncName, PSFuncName);
	if (_shader) return true;
	else return false;
}

bool BaseObject::LoadTexture(W_STR texFileName)
{
	if (texFileName == L"") return false;
	_texture = S_TexManager.LoadTexture(texFileName);
	if (_texture)
	{
		_textureSRV = _texture->_textureSRV;
		return true;
	}
	else return false;
}

bool BaseObject::Create(
	ID3D11Device* device, ID3D11DeviceContext* immediateContext,
	const W_STR shaderFileName, const W_STR texFileName, const C_STR VSFuncName, const C_STR PSFuncName)
{
	_device = device;
	_immediateContext = immediateContext;

	if (FAILED(CreateConstantBuffer()))							return false;
	if (FAILED(CreateVertexBuffer()))							return false;
	if (FAILED(CreateIndexBuffer()))							return false;
	if (!LoadShader(shaderFileName, VSFuncName, PSFuncName))	return false;
	if (FAILED(CreateVertexLayout()))							return false;
	if (LoadTexture(texFileName))
	{
		_textureSRV = _texture->_textureSRV;
	};

	Init();
	return true;
}

void BaseObject::UpdateConstantBuffer()
{
	_constantData.worldMat	= _worldMat.GetTransposed();
	_constantData.viewMat	= _viewMat.GetTransposed();
	_constantData.projMat	= _projMat.GetTransposed();
	_immediateContext->UpdateSubresource(
		_constantBuffer.Get(), NULL, nullptr, &_constantData, 0, 0);
}

void BaseObject::UpdateVertexBuffer()
{
	_immediateContext->UpdateSubresource(
		_vertexBuffer.Get(), NULL, nullptr, &_vertices.at(0), 0, 0);
}

void BaseObject::SetTransformationMatrix(Matrix* worldMat, Matrix* viewMat, Matrix* projMat)
{
	if (worldMat)	_worldMat	= *worldMat;
	if (viewMat)	_viewMat	= *viewMat;
	if (projMat)	_projMat	= *projMat;
	UpdateConstantBuffer();
}

void BaseObject::GetMinMaxV(Vector& minVertex, Vector& maxVertex)
{
	minVertex = maxVertex = _vertices[0].p;

	for (int i = 0; i < _vertices.size(); i++)
	{
		if (_vertices[i].p.x > maxVertex.x) maxVertex.x = _vertices[i].p.x;
		if (_vertices[i].p.y > maxVertex.y) maxVertex.y = _vertices[i].p.y;
		if (_vertices[i].p.z > maxVertex.z) maxVertex.z = _vertices[i].p.z;
							
		if (_vertices[i].p.x < minVertex.x) minVertex.x = _vertices[i].p.x;
		if (_vertices[i].p.y < minVertex.y) minVertex.y = _vertices[i].p.y;
		if (_vertices[i].p.z < minVertex.z) minVertex.z = _vertices[i].p.z;
	}
}

/*
Static function that creates and returns a vertex buffer
*/
ID3D11Buffer* BaseObject::CreateVertexBuffer(ID3D11Device* device, void* dataAddress, UINT numVertices, UINT vertexSize)
{
	ID3D11Buffer* vertexBuffer;
	D3D11_BUFFER_DESC vbDesc;
	ZeroMemory(&vbDesc, sizeof(vbDesc));
	vbDesc.ByteWidth = vertexSize * numVertices;
	vbDesc.Usage = D3D11_USAGE_DEFAULT;
	vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA subRsrcData;
	ZeroMemory(&subRsrcData, sizeof(subRsrcData));
	subRsrcData.pSysMem = dataAddress;

	HRESULT result = device->CreateBuffer(
		&vbDesc,
		&subRsrcData,
		&vertexBuffer
	);

	return vertexBuffer;
}

/*----------------------------------------------------------
|	OBJECT RENDERING		                                |
 -----------------------------------------------------------*/

bool BaseObject::Init()
{
	return true;
}

bool BaseObject::Frame()
{
	return true;
}

bool BaseObject::PreRender()
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	_immediateContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	_immediateContext->IASetVertexBuffers(0, 1, _vertexBuffer.GetAddressOf(), &stride, &offset);
	_immediateContext->IASetIndexBuffer(_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, offset);
	_immediateContext->IASetInputLayout(_vertexLayout.Get());
	_immediateContext->VSSetConstantBuffers(0, 1, _constantBuffer.GetAddressOf());
	_immediateContext->VSSetShader(_shader->_vertexShader.Get(), NULL, 0);
	_immediateContext->PSSetShader(_shader->_pixelShader.Get(), NULL, 0);
	_immediateContext->PSSetShaderResources(0, 1, &_textureSRV);
	
	return true;
}

bool BaseObject::PostRender()
{
	if (_indexBuffer == nullptr)
		_immediateContext->Draw(UINT(_vertices.size()), 0);
	else
		_immediateContext->DrawIndexed(UINT(_indices.size()), 0, 0);

	return true;
}

bool BaseObject::Render()
{
	PreRender();
	PostRender();
	return true;
}

bool BaseObject::Release()
{
	return true;
}
