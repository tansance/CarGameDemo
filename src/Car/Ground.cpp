#include "Ground.h"

Ground::Ground()
{
}

Ground::Ground(const Ground &)
{
}

Ground::~Ground()
{
}

bool Ground::Initialize(ID3D11Device * device)
{
	bool result;
	D3DXVECTOR3 scale(100.0f, 0.0f, 100.0f);
	result = ground.Initialize(device, (char*)"../Car/data/ground.txt", (WCHAR*)L"../Car/data/ground.dds");
	if (!result) {
		return false;
	}
	return true;
}

void Ground::Shutdown()
{
	ground.Shutdown();
}

void Ground::Render(ID3D11DeviceContext * deviceContext)
{
	ground.Render(deviceContext);
}

bool Ground::RenderDepth(ID3D11DeviceContext *deviceContext, DepthShaderClass * depthShader, D3DXMATRIX lightViewMatrix, D3DXMATRIX lightProjectionMatrix)
{
	bool result;
	D3DXMATRIX wordMatrix;
	D3DXMatrixTranslation(&wordMatrix, 0.0f, 0.0f, 0.0f);
	ground.Render(deviceContext);
	result = depthShader->Render(deviceContext, ground.GetIndexCount(), wordMatrix, lightViewMatrix, lightProjectionMatrix);
	if (!result) {
		return false;
	}
	return true;
}

bool Ground::RenderWithShadow(ID3D11DeviceContext *deviceContext, ShadowShaderClass *shadowShader, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix,
	D3DXMATRIX lightViewMatrix, D3DXMATRIX lightProjectionMatrix, ID3D11ShaderResourceView * depthMapTexture,
	D3DXVECTOR3 lightPosition, D3DXVECTOR4 ambientColor, D3DXVECTOR4 diffuseColor)
{
	bool result;
	D3DXMATRIX wordMatrix;
	D3DXMatrixTranslation(&wordMatrix, 0.0f, 0.0f, 0.0f);
	ground.Render(deviceContext);
	result = shadowShader->Render(deviceContext, ground.GetIndexCount(), wordMatrix, viewMatrix, projectionMatrix, lightViewMatrix, lightProjectionMatrix, ground.GetTexture(), depthMapTexture, lightPosition, ambientColor,diffuseColor);
	if (!result) {
		return false;
	}
	return true;
}

int Ground::GetIndexCount()
{
	return ground.GetIndexCount();
}

ID3D11ShaderResourceView * Ground::GetTexture()
{
	return ground.GetTexture();
}
