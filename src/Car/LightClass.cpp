#include "LightClass.h"

#include "lightclass.h"


LightClass::LightClass()
{
}


LightClass::LightClass(const LightClass& other)
{
}


LightClass::~LightClass()
{
}

void LightClass::SetAmbientColor(float red, float green, float blue, float alpha)
{
	ambientColor = D3DXVECTOR4(red, green, blue, alpha);
	return;
}

void LightClass::SetDiffuseColor(float red, float green, float blue, float alpha)
{
	diffuseColor_ = D3DXVECTOR4(red, green, blue, alpha);
	return;
}


void LightClass::SetDirection(float x, float y, float z)
{
	direction_ = D3DXVECTOR3(x, y, z);
	return;
}

void LightClass::SetPosition(float x, float y, float z)
{
	position = D3DXVECTOR3(x, y, z);
	return;
}

void LightClass::SetLookAt(float x, float y, float z)
{
	lookAt.x = x;
	lookAt.y = y;
	lookAt.z = z;
	return;
}

D3DXVECTOR4 LightClass::GetAmbientColor()
{
	return ambientColor;
}

D3DXVECTOR4 LightClass::GetDiffuseColor()
{
	return diffuseColor_;
}


D3DXVECTOR3 LightClass::GetDirection()
{
	return direction_;
}

D3DXVECTOR3 LightClass::GetPosition()
{
	return position;
}

void LightClass::GenerateViewMatrix()
{
	D3DXVECTOR3 up;

	// Setup the vector that points upwards.
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	// Create the view matrix from the three vectors.
	D3DXMatrixLookAtLH(&viewMatrix, &position, &lookAt, &up);

	return;
}

void LightClass::GenerateProjectionMatrix(float screenDepth, float screenNear)
{
	float fieldOfView, screenAspect;

	// Setup field of view and screen aspect for a square light source.
	fieldOfView = (float)D3DX_PI / 2.0f;
	screenAspect = 1.0f;

	// Create the projection matrix for the light.
	D3DXMatrixPerspectiveFovLH(&projectionMatrix, fieldOfView, screenAspect, screenNear, screenDepth);

	return;
}

void LightClass::GetViewMatrix(D3DXMATRIX& viewMatrix)
{
	viewMatrix = this->viewMatrix;
	return;
}


void LightClass::GetProjectionMatrix(D3DXMATRIX& projectionMatrix)
{
	projectionMatrix = this->projectionMatrix;
	return;
}