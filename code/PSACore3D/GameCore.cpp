#include "GameCore.h"

/*
Unbind all objects from the immediate context
*/
void GameCore::ClearD3D11DeviceContext()
{
	if (_immediateContext == NULL) return;

	ID3D11ShaderResourceView* pSRVs[16] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
	/*ID3D11RenderTargetView* pRTVs[16] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
	ID3D11DepthStencilView* pDSV = NULL;
	ID3D11Buffer* pBuffers[16] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
	ID3D11SamplerState* pSamplers[16] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
	UINT StrideOffset[16] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };*/

	// Shaders
	//pd3dDeviceContext->VSSetShader(NULL, NULL, 0);
	//pd3dDeviceContext->HSSetShader(NULL, NULL, 0);
	//pd3dDeviceContext->DSSetShader(NULL, NULL, 0);
	//pd3dDeviceContext->GSSetShader(NULL, NULL, 0);
	//pd3dDeviceContext->PSSetShader(NULL, NULL, 0);

	//// IA clear
	//pd3dDeviceContext->IASetVertexBuffers(0, 16, pBuffers, StrideOffset, StrideOffset);
	//pd3dDeviceContext->IASetIndexBuffer(NULL, DXGI_FORMAT_R16_UINT, 0);
	//pd3dDeviceContext->IASetInputLayout(NULL);

	//// Constant buffers
	//pd3dDeviceContext->VSSetConstantBuffers(0, 14, pBuffers);
	//pd3dDeviceContext->HSSetConstantBuffers(0, 14, pBuffers);
	//pd3dDeviceContext->DSSetConstantBuffers(0, 14, pBuffers);
	//pd3dDeviceContext->GSSetConstantBuffers(0, 14, pBuffers);
	//pd3dDeviceContext->PSSetConstantBuffers(0, 14, pBuffers);

	// Resources
	_immediateContext->VSSetShaderResources(0, 16, pSRVs);
	//pd3dDeviceContext->HSSetShaderResources(0, 16, pSRVs);
	//pd3dDeviceContext->DSSetShaderResources(0, 16, pSRVs);
	//pd3dDeviceContext->GSSetShaderResources(0, 16, pSRVs);
	_immediateContext->PSSetShaderResources(0, 16, pSRVs);

	//// Samplers
	//pd3dDeviceContext->VSSetSamplers(0, 16, pSamplers);
	//pd3dDeviceContext->HSSetSamplers(0, 16, pSamplers);
	//pd3dDeviceContext->DSSetSamplers(0, 16, pSamplers);
	//pd3dDeviceContext->GSSetSamplers(0, 16, pSamplers);
	//pd3dDeviceContext->PSSetSamplers(0, 16, pSamplers);

	//// Render targets
	//pd3dDeviceContext->OMSetRenderTargets(8, pRTVs, pDSV);

	//// States
	//FLOAT blendFactor[4] = { 0,0,0,0 };
	//pd3dDeviceContext->OMSetBlendState(NULL, blendFactor, 0xFFFFFFFF);
	//pd3dDeviceContext->OMSetDepthStencilState(NULL, 0);
	//pd3dDeviceContext->RSSetState(NULL);
}

HRESULT GameCore::CreateDXResource()
{
	_writer.Init();
	IDXGISurface1* backBuffer;
	_swapChain->GetBuffer(0, __uuidof(IDXGISurface1),
		(void**)&backBuffer);
	_writer.Set(backBuffer);
	backBuffer->Release();	
	return S_OK;
}

HRESULT GameCore::DeleteDXResource()
{
	HRESULT hr;
	if (FAILED(hr = _writer.DeleteDXResource())) return hr;
	return hr;
}

bool GameCore::CoreInit()
{
	Device::Init();
	DxState::SetState(_device.Get());
	S_SoundManager.Init();
	S_TexManager.SetDevice(_device.Get(), _immediateContext.Get());
	S_ShaderManager.SetDevice(_device.Get(), _immediateContext.Get());
	S_GameTimer.Init();
	S_Input.Init();

	_writer.Init();
	IDXGISurface1* backBuffer;
	_swapChain->GetBuffer(0, __uuidof(IDXGISurface1),
		(void**)&backBuffer);
	_writer.Set(backBuffer);
	backBuffer->Release();

	//W_STR shaderfilename = L"../../resource/shader/defaultShader.txt";
	//_background.Create(_device.Get(), _immediateContext.Get(), shaderfilename);
	//_renderTarget.Create(_device.Get(), _immediateContext.Get(), 2048, 2048);

	return Init();
}

bool GameCore::CoreFrame()
{
	S_SoundManager.Frame();
	S_Input.Frame();
	S_GameTimer.Frame();
	_writer.Frame();
	ClearD3D11DeviceContext();
	return Frame();
}

bool GameCore::CorePreRender()
{
	float color[4] = { 1, 1, 1, 1.0f };
	_immediateContext->OMSetRenderTargets(1, _rtv.GetAddressOf(), _dsv.Get());
	_immediateContext->ClearRenderTargetView(_rtv.Get(), color);
	_immediateContext->ClearDepthStencilView(_dsv.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	_immediateContext->PSSetSamplers(0, 1, &DxState::g_SSWrap);
	_immediateContext->RSSetViewports(1, &_vp);
	_immediateContext->RSSetState(DxState::g_RSSolid);
	_immediateContext->OMSetBlendState(DxState::g_BSAlpha, 0, -1);
	_immediateContext->OMSetDepthStencilState(DxState::g_DSDepthEnable, 0xff);
	return true;
}

bool GameCore::CoreRender()
{
	CorePreRender();
	/*_renderTarget._oldRtv	= _rtv.Get();
	_renderTarget._oldDsv	= _dsv.Get();
	_renderTarget._oldVp[0] = _vp;*/
	if (S_Input.GetKey(VK_F1) == KEY_HOLD)
	{
		_isWireFrame = true;
		_immediateContext->RSSetState(DxState::g_RSWireFrame);
		
	}
	/*else
	{
		_isWireFrame = false;
		if (_renderTarget.Begin())
		{
			Render();
			_renderTarget.End();
		}
		if (_renderTarget._srv)
		{
			_background._textureSRV = _renderTarget._srv.Get();
		}
	}*/
	Render();
	CorePostRender();
	return true;
}

bool GameCore::CorePostRender()
{
	/*if (!_isWireFrame)
	{
		_background.SetTransformationMatrix(nullptr, nullptr, nullptr);
		_background.Render();
	}*/
	
	S_Input.Render();
	S_GameTimer.Render();
	_writer.Render();
	_swapChain->Present(0, 0);
	return true;
}

bool GameCore::CoreRelease()
{
	_renderTarget.Release();
	_background.Release();
	Release();
	S_Input.Release();
	S_GameTimer.Release();
	DxState::Release();
	Device::Release();
	_writer.Release();
	return true;
}

bool GameCore::Run()
{
	CoreInit();
	while (_isGameRun)
	{
		if (Window::Run() == false)
		{
			_isGameRun = false;
		}
		else
		{
			if (!CoreFrame() || !CoreRender())
			{
				_isGameRun = false;
			}

		}
	}
	CoreRelease();
	//CheckLiveObjects();
	return true;
}