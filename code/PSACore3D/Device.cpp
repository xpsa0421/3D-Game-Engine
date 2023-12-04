#include "Device.h"

bool Device::Init()
{
    if (FAILED(CreateDXGIDevice()))         return false;
    if (FAILED(CreateDevice()))             return false;
    if (FAILED(CreateSwapChain()))          return false;
    if (FAILED(CreateRenderTargetView()))   return false;
    if (FAILED(CreateDepthStencilView()))   return false;

    CreateViewport();
    return true;
}

bool Device::Frame()
{
    return true;
}

bool Device::Render()
{
    return true;
}

bool Device::Release()
{
    _device             = nullptr;
    _immediateContext   = nullptr;
    _factory            = nullptr;
    _swapChain          = nullptr;
    _rtv                = nullptr;
    return true;
}

HRESULT Device::CreateDevice()
{
    HRESULT hr;
    UINT createDeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#ifdef _DEBUG
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    D3D_FEATURE_LEVEL featureLevel;
    D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_0,
    };
    UINT featureLevelCount = 1;
    hr = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, NULL,
        createDeviceFlags, featureLevels, featureLevelCount, D3D11_SDK_VERSION,
        &_device, &featureLevel, &_immediateContext);
    return hr;
}

HRESULT Device::CreateDXGIDevice()
{
    HRESULT hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&_factory);
    return hr;
}

HRESULT Device::CreateSwapChain()
{
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));

    sd.BufferCount = 1;
    sd.BufferDesc.Width = g_rectClient.right;
    sd.BufferDesc.Height = g_rectClient.bottom;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = g_wnd;
    sd.Windowed = true;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

    return _factory->CreateSwapChain(_device.Get(), &sd, &_swapChain);
}

HRESULT Device::CreateRenderTargetView()
{
    HRESULT hr = 0;
    ID3D11Texture2D* backBuffer = nullptr;
    _swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
    if (backBuffer != nullptr)
    {
        hr = _device->CreateRenderTargetView(backBuffer, nullptr, &_rtv);
        backBuffer->Release();
    }
    return hr;
}

HRESULT Device::CreateDepthStencilView()
{
    HRESULT hr;
    D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
    DXGI_SWAP_CHAIN_DESC scDesc;
    _rtv->GetDesc(&rtvDesc);
    _swapChain->GetDesc(&scDesc);

    // create texture
    ID3D11Texture2D* dsTexture;
    D3D11_TEXTURE2D_DESC tDesc;
    ZeroMemory(&tDesc, sizeof(tDesc));
    tDesc.Width             = scDesc.BufferDesc.Width;
    tDesc.Height            = scDesc.BufferDesc.Height;
    tDesc.MipLevels         = 1;
    tDesc.ArraySize         = 1;
    tDesc.Format            = DXGI_FORMAT_R24G8_TYPELESS;
    tDesc.SampleDesc.Count  = 1;
    tDesc.Usage             = D3D11_USAGE_DEFAULT;
    tDesc.CPUAccessFlags    = 0;
    tDesc.MiscFlags         = 0;
    tDesc.BindFlags         = D3D11_BIND_DEPTH_STENCIL;
    hr = _device->CreateTexture2D(&tDesc, nullptr, &dsTexture);

    // create depth stencil view
    D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
    ZeroMemory(&dsvDesc, sizeof(dsvDesc));
    dsvDesc.Format          = DXGI_FORMAT_D24_UNORM_S8_UINT;
    dsvDesc.ViewDimension   = D3D11_DSV_DIMENSION_TEXTURE2D;
    if (dsTexture)
    {
        hr = _device->CreateDepthStencilView(dsTexture, &dsvDesc, &_dsv);
    }
    dsTexture->Release();
    return hr;
}

void Device::CreateViewport()
{
    _vp.Width = FLOAT(g_rectClient.right);
    _vp.Height = FLOAT(g_rectClient.bottom);
    _vp.TopLeftX = 0;
    _vp.TopLeftY = 0;
    _vp.MinDepth = 0.0f;
    _vp.MaxDepth = 1.0f;
    _immediateContext->RSSetViewports(1, &_vp);
}

HRESULT Device::CreateDXResource()
{
    return S_OK;
}

HRESULT Device::DeleteDXResource()
{
    return S_OK;
}

HRESULT Device::ResizeDevice(UINT width, UINT height)
{
    HRESULT hr;
    if (!_device)   return E_FAIL;

    // unset and release current render target
    if (FAILED(hr = DeleteDXResource())) return hr;
    _immediateContext->OMSetRenderTargets(0, nullptr, NULL);
    _rtv.ReleaseAndGetAddressOf();
    _dsv.ReleaseAndGetAddressOf();

    // resize back buffer size
    DXGI_SWAP_CHAIN_DESC scDesc;
    _swapChain->GetDesc(&scDesc);
    if (FAILED(hr = _swapChain->ResizeBuffers(
        scDesc.BufferCount, width, height, scDesc.BufferDesc.Format, 0)
    )) return hr;

    // recreate render target view
    if (FAILED(hr = CreateRenderTargetView()))  return hr;
    if (FAILED(hr = CreateDepthStencilView()))  return hr;

    CreateViewport();
    return CreateDXResource();
}