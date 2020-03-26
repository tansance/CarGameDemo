#pragma once
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")

#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>
#include <d3dx10math.h>

class D3DClass
{
public:
	D3DClass();
	D3DClass(const D3DClass&);
	~D3DClass();

	bool Initialize(int, int, bool, HWND, bool, float, float);
	void Shutdown();

	void BeginScene(float, float, float, float);
	void EndScene();

	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();

	void GetProjectionMatrix(D3DXMATRIX&);
	void GetWorldMatrix(D3DXMATRIX&);
	void GetOrthoMatrix(D3DXMATRIX&);

	ID3D11DepthStencilView* GetDepthStencilView();
	void SetBackBufferRenderTarget();
	void ResetViewport();

private:
	bool vsync_enabled;
	/*int videoCardMemory;
	char videoCardDescription[128];*/
	
	ID3D11Device* d3dDevice;
	ID3D11DeviceContext* d3dDeviceContext;
	IDXGISwapChain* swapChain;

	ID3D11RenderTargetView* backBufferTargetView;
	ID3D11Texture2D* depthStencilBuffer;
	ID3D11DepthStencilState* depthStencilState;
	ID3D11DepthStencilView* depthStencilView;

	ID3D11RasterizerState* rasterState;

	D3DXMATRIX projectionMatrix;
	D3DXMATRIX worldMatrix;
	D3DXMATRIX orthoMatrix;

	ID3D11DepthStencilState* depthDisabledStencilState;
	D3D11_VIEWPORT viewport;
};