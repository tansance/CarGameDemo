#include "Wheel.h"

Wheel::Wheel()
{
}

Wheel::Wheel(const Wheel &)
{
}

Wheel::~Wheel()
{
}

bool Wheel::Initialize(ID3D11Device* device, D3DXVECTOR3* scaleInput)
{
	position.x = 0;
	position.y = 0;
	position.z = 0;

	rotation.x = 0;
	rotation.y = 0;
	rotation.z = 0;

	direction = 0.0f;

	bool result;
	result = sides.Initialize(device, (char*)"../Car/data/tireSides.txt",(WCHAR*) L"../Car/data/tireside.dds", scaleInput);
	if (!result)
	{
		return false;
	}
	result = tire.Initialize(device, (char*)"../Car/data/body.txt", (WCHAR*)L"../Car/data/tirefront.dds", scaleInput);
	if (!result)
	{
		return false;
	}
	return true;
}

void Wheel::Shutdown()
{
	sides.Shutdown();
	tire.Shutdown();
}

bool Wheel::Render(ID3D11DeviceContext *deviceContext, LightShaderClass* lightShader, LightClass* light, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix)
{
	D3DXMATRIX worldMatrix, rotationMatrix;
	D3DXMatrixTranslation(&worldMatrix, position.x, position.y, position.z);
	D3DXMatrixRotationYawPitchRoll(&rotationMatrix, rotation.y + direction, rotation.x, rotation.z);
	worldMatrix = rotationMatrix * worldMatrix;
	bool result;
	sides.Render(deviceContext);
	result = lightShader->Render(deviceContext, GetSideIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		GetSideTexture(), light->GetDirection(), light->GetDiffuseColor());
	if (!result)
	{
		return false;
	}
	tire.Render(deviceContext);
	result = lightShader->Render(deviceContext, GetTireIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		GetTireTexture(), light->GetDirection(), light->GetDiffuseColor());
	if (!result)
	{
		return false;
	}
	return true;
}

bool Wheel::RenderDepth(ID3D11DeviceContext *deviceContext, DepthShaderClass * depthShader, D3DXMATRIX lightViewMatrix, D3DXMATRIX lightProjectionMatrix)
{
	bool result;
	D3DXMATRIX worldMatrix, rotationMatrix;
	D3DXMatrixTranslation(&worldMatrix, position.x, position.y, position.z);
	D3DXMatrixRotationYawPitchRoll(&rotationMatrix, rotation.y + direction, rotation.x, rotation.z);
	worldMatrix = rotationMatrix * worldMatrix;
		
	sides.Render(deviceContext);
	result = depthShader->Render(deviceContext, GetSideIndexCount(), worldMatrix, lightViewMatrix, lightProjectionMatrix);
	if (!result)
	{
		return false;
	}
	tire.Render(deviceContext);
	result = depthShader->Render(deviceContext, GetTireIndexCount(), worldMatrix, lightViewMatrix, lightProjectionMatrix);
	if (!result)
	{
		return false;
	}
	return true;
}

bool Wheel::RenderWithShadow(ID3D11DeviceContext * deviceContext, ShadowShaderClass * shadowShader, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix,
	D3DXMATRIX lightViewMatrix, D3DXMATRIX lightProjectionMatrix, ID3D11ShaderResourceView * depthMapTexture,
	D3DXVECTOR3 lightPosition, D3DXVECTOR4 ambientColor, D3DXVECTOR4 diffuseColor) 
{
	bool result;
	D3DXMATRIX worldMatrix, rotationMatrix;
	D3DXMatrixTranslation(&worldMatrix, position.x, position.y, position.z);
	D3DXMatrixRotationYawPitchRoll(&rotationMatrix, rotation.y + direction, rotation.x, rotation.z);
	worldMatrix = rotationMatrix * worldMatrix;

	sides.Render(deviceContext);
	result = shadowShader->Render(deviceContext, GetSideIndexCount(), worldMatrix, viewMatrix, projectionMatrix, lightViewMatrix, lightProjectionMatrix, sides.GetTexture(), depthMapTexture, lightPosition, ambientColor, diffuseColor);
	if (!result)
	{
		return false;
	}

	tire.Render(deviceContext);
	result = shadowShader->Render(deviceContext, GetTireIndexCount(), worldMatrix, viewMatrix, projectionMatrix, lightViewMatrix, lightProjectionMatrix, tire.GetTexture(), depthMapTexture, lightPosition, ambientColor, diffuseColor);
	if (!result)
	{
		return false;
	}
	return true;
}

int Wheel::GetSideIndexCount()
{
	return sides.GetIndexCount();
}

int Wheel::GetTireIndexCount()
{
	return tire.GetIndexCount();
}

ID3D11ShaderResourceView * Wheel::GetSideTexture()
{
	return sides.GetTexture();
}

ID3D11ShaderResourceView * Wheel::GetTireTexture()
{
	return tire.GetTexture();
}

void Wheel::setPosition(float x, float y, float z)
{
	position.x = x;
	position.y = y;
	position.z = z;
}

void Wheel::increasePositionBy(float x, float y, float z)
{
	position.x += x;
	position.y += y;
	position.z += z;
}

void Wheel::setDirection(int turn)
{
	if (turn == 1) {
		direction = D3DX_PI / 4;
	}
	else if (turn == -1) {
		direction = -D3DX_PI / 4;
	}
	else if(turn == 0){
		direction = 0.0f;
	}
}

void Wheel::increaseRotationBy(float pitch, float yaw, float roll)
{
	rotation.x += pitch;
	rotation.y += yaw;
	rotation.z += roll;
}

void Wheel::roll(int direction)
{
	if (direction == 1) {
		//increasePositionBy(0.0f, 0.0f, 0.2f);
		rotation.x += D3DX_PI / 90;
		if (rotation.x > 2 * D3DX_PI)
			rotation.x -= 2 * D3DX_PI;
	}
	if (direction == -1) {
		//increasePositionBy(0.0f, 0.0f, -0.2f);
		rotation.x -= D3DX_PI / 90;
		if (rotation.x < - 2 * D3DX_PI)
			rotation.x += 2 * D3DX_PI;
	}
}
