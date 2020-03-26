#include "D3DClass.h"

D3DClass::D3DClass()
{
	swapChain = 0;
	d3dDevice = 0;
	d3dDeviceContext = 0;
	backBufferTargetView = 0;
	depthStencilBuffer = 0;
	depthStencilState = 0;
	depthStencilView = 0;
	rasterState = 0;
}


D3DClass::D3DClass(const D3DClass& other)
{
}


D3DClass::~D3DClass()
{
}

bool D3DClass::Initialize(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen, float screenDepth, float screenNear)
{
	HRESULT result;
	IDXGIFactory* factory;
	IDXGIAdapter* adapter;
	IDXGIOutput* adapterOutput;
	unsigned int numModes, i, numerator, denominator, stringLength;
	DXGI_MODE_DESC* displayModeList;
	DXGI_ADAPTER_DESC adapterDesc;
	int error;
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	D3D_FEATURE_LEVEL featureLevel;
	ID3D11Texture2D* backBufferPtr;
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	D3D11_RASTERIZER_DESC rasterDesc;
	float fieldOfView, screenAspect;


	// Store the vsync setting.
	vsync_enabled = vsync;

	// Create a DirectX graphics interface factory.
	result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	if (FAILED(result))
	{
		MessageBox(hwnd, L"Could not create IDXGIFactory", L"Error", MB_OK);
		return false;
	}

	// Use the factory to create an adapter for the primary graphics interface (video card).
	result = factory->EnumAdapters(0, &adapter);
	if (FAILED(result))
	{
		MessageBox(hwnd, L"Could not create adapter of primary graphics interface", L"Error", MB_OK);
		return false;
	}

	// Enumerate the primary adapter output (monitor).
	result = adapter->EnumOutputs(0, &adapterOutput);
	if (FAILED(result))
	{
		MessageBox(hwnd, L"Could not enumerate the primary adapter output", L"Error", MB_OK);
		return false;
	}

	// Get the number of modes that fit the DXGI_FORMAT_R8G8B8A8_UNORM display format for the adapter output (monitor).
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
	if (FAILED(result))
	{
		MessageBox(hwnd, L"Could not get the number of modes that fit the DXGI_FORMAT_R8G8B8A8_UNORM display format for the adapter output", L"Error", MB_OK);
		return false;
	}

	// Create a list to hold all the possible display modes for this monitor/video card combination.
	displayModeList = new DXGI_MODE_DESC[numModes];
	if (!displayModeList)
	{
		MessageBox(hwnd, L"Could not create a list to hold all the possible display modes for this monitor/video card combination", L"Error", MB_OK);
		return false;
	}

	// Fill the display mode list structures.
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
	if (FAILED(result))
	{
		return false;
	}

	// Go through all the display modes and find the one that matches the screen width and height.
	// When a match is found store the numerator and denominator of the refresh rate for that monitor.
	for (i = 0; i < numModes; i++)
	{
		if (displayModeList[i].Width == (unsigned int)screenWidth)
		{
			if (displayModeList[i].Height == (unsigned int)screenHeight)
			{
				numerator = displayModeList[i].RefreshRate.Numerator;
				denominator = displayModeList[i].RefreshRate.Denominator;
			}
		}
	}

	// Release the display mode list.
	delete[] displayModeList;
	displayModeList = 0;

	// Release the adapter output.
	adapterOutput->Release();
	adapterOutput = 0;

	// Release the adapter.
	adapter->Release();
	adapter = 0;

	// Release the factory.
	factory->Release();
	factory = 0;

	//Initialize D3D
	// Initialize the swap chain description.
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	// Set to a single back buffer.
	swapChainDesc.BufferCount = 1;

	// Set the width and height of the back buffer.
	swapChainDesc.BufferDesc.Width = screenWidth;
	swapChainDesc.BufferDesc.Height = screenHeight;

	// Set regular 32-bit surface for the back buffer.
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	// Set the refresh rate of the back buffer.
	if (vsync_enabled)
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
	}
	else
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	// Set the usage of the back buffer.
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	// Set the handle for the window to render to.
	swapChainDesc.OutputWindow = hwnd;

	// Turn multisampling off.
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	// Set to full screen or windowed mode.
	if (fullscreen)
	{
		swapChainDesc.Windowed = false;
	}
	else
	{
		swapChainDesc.Windowed = true;
	}

	// Set the scan line ordering and scaling to unspecified.
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// Discard the back buffer contents after presenting.
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	// Don't set the advanced flags.
	swapChainDesc.Flags = 0;

	// Set the feature level to DirectX 11.
	featureLevel = D3D_FEATURE_LEVEL_11_0;

	// Create the swap chain, Direct3D device, and Direct3D device context.
	result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &featureLevel, 1,
			D3D11_SDK_VERSION, &swapChainDesc, &swapChain, &d3dDevice, NULL, &d3dDeviceContext);
	if (FAILED(result))
	{
		return false;
	}

	// Get the pointer to the back buffer.
	result = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
	if (FAILED(result))
	{
		return false;
	}

	// Create the render target view with the back buffer pointer.
	result = d3dDevice->CreateRenderTargetView(backBufferPtr, NULL, &backBufferTargetView);
	if (FAILED(result))
	{
		return false;
	}

	// Release pointer to the back buffer as we no longer need it.
	backBufferPtr->Release();
	backBufferPtr = 0;

	// Initialize the description of the depth buffer.
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	// Set up the description of the depth buffer.
	depthBufferDesc.Width = screenWidth;
	depthBufferDesc.Height = screenHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	// Create the texture for the depth buffer using the filled out description.
		result = d3dDevice->CreateTexture2D(&depthBufferDesc, NULL, &depthStencilBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Initialize the description of the stencil state.
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// Set up the description of the stencil state.
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing.
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create the depth stencil state.
	result = d3dDevice->CreateDepthStencilState(&depthStencilDesc, &depthStencilState);
	if (FAILED(result))
	{
		return false;
	}

	// Set the depth stencil state.
	d3dDeviceContext->OMSetDepthStencilState(depthStencilState, 1);

	// Initailze the depth stencil view.
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	// Set up the depth stencil view description.
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// Create the depth stencil view.
	result = d3dDevice->CreateDepthStencilView(depthStencilBuffer, &depthStencilViewDesc, &depthStencilView);
	if (FAILED(result))
	{
		return false;
	}

	// Bind the render target view and depth stencil buffer to the output render pipeline.
	d3dDeviceContext->OMSetRenderTargets(1, &backBufferTargetView, depthStencilView);

	// Setup the raster description which will determine how and what polygons will be drawn.
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	// Create the rasterizer state from the description we just filled out.
	result = d3dDevice->CreateRasterizerState(&rasterDesc, &rasterState);
	if (FAILED(result))
	{
		return false;
	}

	// Now set the rasterizer state.
	d3dDeviceContext->RSSetState(rasterState);

	// Setup the viewport for rendering.
	viewport.Width = (float)screenWidth;
	viewport.Height = (float)screenHeight;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	// Create the viewport.
	d3dDeviceContext->RSSetViewports(1, &viewport);

	// Setup the projection matrix.
	fieldOfView = (float)D3DX_PI / 4.0f;
	screenAspect = (float)screenWidth / (float)screenHeight;

	// Create the projection matrix for 3D rendering.
	D3DXMatrixPerspectiveFovLH(&projectionMatrix, fieldOfView, screenAspect, screenNear, screenDepth);

	// Initialize the world matrix to the identity matrix.
	D3DXMatrixIdentity(&worldMatrix);

	// Create an orthographic projection matrix for 2D rendering.
	D3DXMatrixOrthoLH(&orthoMatrix, (float)screenWidth, (float)screenHeight, screenNear, screenDepth);

	return true;
}

void D3DClass::Shutdown()
{
	// Before shutting down set to windowed mode or when you release the swap chain it will throw an exception.
	if (swapChain)
	{
		swapChain->SetFullscreenState(false, NULL);
	}

	if (rasterState)
	{
		rasterState->Release();
		rasterState = 0;
	}

	if (depthStencilView)
	{
		depthStencilView->Release();
		depthStencilView = 0;
	}

	if (depthStencilState)
	{
		depthStencilState->Release();
		depthStencilState = 0;
	}

	if (depthStencilBuffer)
	{
		depthStencilBuffer->Release();
		depthStencilBuffer = 0;
	}

	if (backBufferTargetView)
	{
		backBufferTargetView->Release();
		backBufferTargetView = 0;
	}

	if (d3dDeviceContext)
	{
		d3dDeviceContext->Release();
		d3dDeviceContext = 0;
	}

	if (d3dDevice)
	{
		d3dDevice->Release();
		d3dDevice = 0;
	}

	if (swapChain)
	{
		swapChain->Release();
		swapChain = 0;
	}

	return;
}

void D3DClass::BeginScene(float red, float green, float blue, float alpha)
{
	float color[4];
	
	// Setup the color to clear the buffer to.
	color[0] = red;
	color[1] = green;
	color[2] = blue;
	color[3] = alpha;

	// Clear the back buffer.
	d3dDeviceContext->ClearRenderTargetView(backBufferTargetView, color);

	// Clear the depth buffer.
	d3dDeviceContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	return;
}

void D3DClass::EndScene()
{
	// Present the back buffer to the screen since rendering is complete.
	if (vsync_enabled)
	{
		// Lock to screen refresh rate.
		swapChain->Present(1, 0);
	}
	else
	{
		// Present as fast as possible.
		swapChain->Present(0, 0);
	}

	return;
}

//Getters
ID3D11Device* D3DClass::GetDevice()
{
	return d3dDevice;
}


ID3D11DeviceContext* D3DClass::GetDeviceContext()
{
	return d3dDeviceContext;
}

void D3DClass::GetProjectionMatrix(D3DXMATRIX& projectionMatrix)
{
	projectionMatrix = this->projectionMatrix;
	return;
}


void D3DClass::GetWorldMatrix(D3DXMATRIX& worldMatrix)
{
	worldMatrix = this->worldMatrix;
	return;
}


void D3DClass::GetOrthoMatrix(D3DXMATRIX& orthoMatrix)
{
	orthoMatrix = this->orthoMatrix;
	return;
}

ID3D11DepthStencilView* D3DClass::GetDepthStencilView()
{
	return depthStencilView;
}

void D3DClass::SetBackBufferRenderTarget()
{
	// Bind the render target view and depth stencil buffer to the output render pipeline.
	d3dDeviceContext->OMSetRenderTargets(1, &backBufferTargetView, depthStencilView);
	return;
}

void D3DClass::ResetViewport()
{
	// Set the viewport.
	d3dDeviceContext->RSSetViewports(1, &viewport);

	return;
}