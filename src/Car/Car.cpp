#include "Car.h"
#include <fstream>
#include<iomanip>

Car::Car()
{
	wheel = 0;
	wheelTransMatrix = 0;
	wheelTransVector = 0;
}

Car::Car(const Car &)
{
}

Car::~Car()
{
}

bool Car::Initialize(ID3D11Device * device, D3DXVECTOR3 * scale)
{
	bool result;

	position.x = 0.0f;
	position.y = 2.0f;
	position.z = 0.0f;

	rotation.x = 0.0f;
	rotation.y = 0.0f;
	rotation.z = 0.0f;

	direction.x = 0.0f;
	direction.y = 0.0f;
	direction.z = 0.05f;

	// Initial wheel translation vector
	wheelTransVector = new D3DXVECTOR3[4]{
		D3DXVECTOR3(0.75f,-1.5f,0.75f),
		D3DXVECTOR3(-0.75f,-1.5f,0.75f),
		D3DXVECTOR3(0.75f, -1.5f, -0.75f),
		D3DXVECTOR3(-0.75f, -1.5f, -0.75f),
	};
	// transform matrix to tramsform wheel from local position to world position
	wheelTransMatrix = new D3DXMATRIX[4];

	setWheelTransMatrix(0.0f);

	wheel = new Wheel[4];
	for (int i = 0; i < 4; ++i) {
		result = wheel[i].Initialize(device, &D3DXVECTOR3(0.5f,0.5f,0.5f));
		if (!result) {
			return false;
		}
	}

	result = body.Initialize(device, (char*)"../Car/data/cube.txt", (WCHAR*)L"../Car/data/seafloor.dds");
	if (!result) {
		return false;
	}

	return true;
}

void Car::Shutdown()
{
	for (int i = 0; i < 4; ++i) {
		wheel[i].Shutdown();
	}
	delete[] wheel;
	wheel = 0;

	delete[] wheelTransVector;
	delete[] wheelTransMatrix;

	body.Shutdown();
}

bool Car::Render(ID3D11DeviceContext *deviceContext, LightShaderClass * lightShader, LightClass * light, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix)
{
	D3DXMATRIX worldMatrix, rotationMatrix;
	bool result;

	D3DXMatrixTranslation(&worldMatrix, position.x, position.y, position.z);
	D3DXMatrixRotationYawPitchRoll(&rotationMatrix, rotation.y, rotation.x, rotation.z);
	worldMatrix = rotationMatrix * worldMatrix;
	
	for (int i = 0; i < 4; ++i) {
		result = wheel[i].Render(deviceContext, lightShader, light, viewMatrix, projectionMatrix);
		if (!result)
		{
			return false;
		}
	}

	body.Render(deviceContext);
	result = lightShader->Render(deviceContext, body.GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		body.GetTexture(), light->GetDirection(), light->GetDiffuseColor());
	if (!result)
	{
		return false;
	}
	return true;
}

bool Car::RenderDepth(ID3D11DeviceContext *deviceContext, DepthShaderClass * depthShader, D3DXMATRIX lightViewMatrix, D3DXMATRIX lightprojectionMatrix)
{
	D3DXMATRIX worldMatrix, rotationMatrix;
	bool result;

	D3DXMatrixTranslation(&worldMatrix, position.x, position.y, position.z);
	D3DXMatrixRotationYawPitchRoll(&rotationMatrix, rotation.y, rotation.x, rotation.z);
	worldMatrix = rotationMatrix * worldMatrix;

	for (int i = 0; i < 4; ++i) {
		result = wheel[i].RenderDepth(deviceContext, depthShader, lightViewMatrix, lightprojectionMatrix);
		if (!result)
		{
			return false;
		}
	}

	body.Render(deviceContext);
	result = depthShader->Render(deviceContext, body.GetIndexCount(), worldMatrix, lightViewMatrix, lightprojectionMatrix);
	if (!result)
	{
		return false;
	}
	return true;
}

bool Car::RenderWithShadow(ID3D11DeviceContext * deviceContext, ShadowShaderClass * shadowShader, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, 
	D3DXMATRIX lightViewMatrix, D3DXMATRIX lightProjectionMatrix, ID3D11ShaderResourceView * depthMapTexture, 
	D3DXVECTOR3 lightPosition, D3DXVECTOR4 ambientColor, D3DXVECTOR4 diffuseColor)
{
	D3DXMATRIX worldMatrix, rotationMatrix;
	bool result;

	D3DXMatrixTranslation(&worldMatrix, position.x, position.y, position.z);
	D3DXMatrixRotationYawPitchRoll(&rotationMatrix, rotation.y, rotation.x, rotation.z);
	worldMatrix = rotationMatrix * worldMatrix;

	for (int i = 0; i < 4; ++i) {
		result = wheel[i].RenderWithShadow(deviceContext, shadowShader, viewMatrix, projectionMatrix, lightViewMatrix, lightProjectionMatrix, depthMapTexture, lightPosition, ambientColor, diffuseColor);
		if (!result)
		{
			return false;
		}
	}

	body.Render(deviceContext);
	result = shadowShader->Render(deviceContext, body.GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, lightViewMatrix, lightProjectionMatrix, body.GetTexture(), depthMapTexture, lightPosition, ambientColor, diffuseColor);
	if (!result)
	{
		return false;
	}
	return true;

	return false;
}

void Car::setPosition(float x, float y, float z)
{
	D3DXMATRIX rotationMatrix;
	D3DXVECTOR3 wheelPosition;
	
	position.x = x;
	position.y = y;
	position.z = z;
	
	D3DXMatrixRotationY(&rotationMatrix, rotation.y);
	D3DXVec3TransformCoord(&wheelPosition, &position, &wheelTransMatrix[0]);

	wheel[0].setPosition(wheelPosition.x, wheelPosition.y, wheelPosition.z);

	D3DXVec3TransformCoord(&wheelPosition, &position, &wheelTransMatrix[1]);
	wheel[1].setPosition(wheelPosition.x, wheelPosition.y, wheelPosition.z);

	D3DXVec3TransformCoord(&wheelPosition, &position, &wheelTransMatrix[2]);
	wheel[2].setPosition(wheelPosition.x, wheelPosition.y, wheelPosition.z);

	D3DXVec3TransformCoord(&wheelPosition, &position, &wheelTransMatrix[3]);
	wheel[3].setPosition(wheelPosition.x, wheelPosition.y, wheelPosition.z);
}

D3DXVECTOR3 Car::getPosition()
{
	return position;
}

D3DXVECTOR3 Car::getRotation()
{
	return rotation;
}

void Car::increasePositionBy(float x, float y, float z)
{
	setPosition(position.x + x, position.y + y, position.z + z);
}

void Car::increaseWheelRotationBy(float x, float y, float z)
{
	for (int i = 0; i < 4; ++i) {
		wheel[i].increaseRotationBy(x, y, z);
	}
}

void Car::setWheelTransMatrix(float turnAngle)
{
	D3DXMATRIX rotationMatrix;

	D3DXMatrixRotationY(&rotationMatrix, turnAngle);
	// i = 0: right front wheel
	// i = 1: left front wheel
	// i = 2: right back wheel
	// i = 3: left back wheel
	for (int i = 0; i < 4; ++i) {
		D3DXVec3TransformCoord(&wheelTransVector[i], &wheelTransVector[i], &rotationMatrix);
		D3DXMatrixTranslation(&wheelTransMatrix[i], wheelTransVector[i].x, wheelTransVector[i].y, wheelTransVector[i].z);
	}
}

void Car::go(int dire, int turn)
{
	D3DXMATRIX turnDirection;

	for (int i = 0; i < 4; ++i) {
		wheel[i].roll(dire);
	}
	wheel[0].setDirection(turn);
	wheel[1].setDirection(turn);

	if (dire == 1) {
		if (turn == 1) {
			rotation.y += TURN_ANGLE;
			increaseWheelRotationBy(0.0f, TURN_ANGLE, 0.0f);
			setWheelTransMatrix(TURN_ANGLE);
			D3DXMatrixRotationYawPitchRoll(&turnDirection, TURN_ANGLE, 0.0f, 0.0f);
		}
		else if (turn == -1) {
			rotation.y += -TURN_ANGLE;
			increaseWheelRotationBy(0.0f, -TURN_ANGLE, 0.0f);
			setWheelTransMatrix(-TURN_ANGLE);
			D3DXMatrixRotationYawPitchRoll(&turnDirection, -TURN_ANGLE, 0.0f, 0.0f);
		}
		else { 
			rotation.y += 0;
			D3DXMatrixRotationYawPitchRoll(&turnDirection, 0.0f, 0.0f, 0.0f);
		}
		D3DXVec3TransformCoord(&direction, &direction, &turnDirection);
		increasePositionBy(direction.x, direction.y,direction.z);
	}
	if (dire == -1) {
		if (turn == 1) {
			rotation.y += -TURN_ANGLE;
			increaseWheelRotationBy(0.0f, -TURN_ANGLE, 0.0f);
			setWheelTransMatrix(-TURN_ANGLE);
			D3DXMatrixRotationYawPitchRoll(&turnDirection, -TURN_ANGLE, 0.0f, 0.0f);
		}
		else if (turn == -1) {
			rotation.y += TURN_ANGLE;
			increaseWheelRotationBy(0.0f, TURN_ANGLE, 0.0f);
			setWheelTransMatrix(TURN_ANGLE);
			D3DXMatrixRotationYawPitchRoll(&turnDirection, TURN_ANGLE, 0.0f, 0.0f);
		}
		else { 
			rotation.y += 0;
			D3DXMatrixRotationYawPitchRoll(&turnDirection, 0.0f, 0.0f, 0.0f);
		}
		D3DXVec3TransformCoord(&direction, &direction, &turnDirection);
		increasePositionBy(-direction.x, -direction.y, -direction.z);
	}
}
