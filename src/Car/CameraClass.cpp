#include "CameraClass.h"

CameraClass::CameraClass()
{
	position.x = 0.0f;
	position.y = 0.0f;
	position.z = 0.0f;

	rotationX = 0.0f;
	rotationY = 0.0f;
	rotationZ = 0.0f;

	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;

	cameraMode = FIRST;
}

CameraClass::CameraClass(bool mode)
{
	position.x = 0.0f;
	position.y = 0.0f;
	position.z = 0.0f;

	rotationX = 0.0f;
	rotationY = 0.0f;
	rotationZ = 0.0f;

	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;

	cameraMode = mode;
}


CameraClass::CameraClass(const CameraClass& other)
{
}


CameraClass::~CameraClass()
{
}

//Setters
void CameraClass::SetPosition(float x, float y, float z)
{
	position.x = x;
	position.y = y;
	position.z = z;
	return;
}

void CameraClass::SetLookAt(float x, float y, float z)
{
	lookAt.x = x;
	lookAt.y = y;
	lookAt.z = z;
}

void CameraClass::SetRotation(float x, float y, float z)
{
	rotationX = x * 0.0174532925f / 2.0f;
	
	rotationY = y * 0.0174532925f / 2.0f;
	rotationZ = z * 0.0174532925f;
	if (cameraMode == FIRST) {
		if (rotationX > XLIMIT)
			rotationX = XLIMIT;
		if (rotationX < -XLIMIT)
			rotationX = -XLIMIT;
		if (rotationY > YLIMIT)
			rotationY = YLIMIT;
		if (rotationY < -YLIMIT)
			rotationY = -YLIMIT;
	}
	
	return;
}

void CameraClass::IncreaseRotationBy(float x, float y, float z)
{
	rotationX += x;
	rotationY += y;
	rotationZ += z;
}

//Getters
D3DXVECTOR3 CameraClass::GetPosition()
{
	return position;
}


D3DXVECTOR3 CameraClass::GetRotation()
{
	return D3DXVECTOR3(rotationX, rotationY, rotationZ);
}

void CameraClass::Render()
{
	D3DXVECTOR3 up, lookAt;
	float yaw, pitch, roll;
	D3DXMATRIX rotationMatrix;

	// Setup the vector that points upwards.
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	// Create the rotation matrix from the yaw, pitch, and roll values.
	D3DXMatrixRotationYawPitchRoll(&rotationMatrix, rotationY, rotationX, rotationZ);

	if (cameraMode == FIRST) {
		RenderFirst(rotationMatrix, &up);
	}
	if (cameraMode == THIRD) {
		RenderThrid(rotationMatrix, &up);
	}
	return;
}

void CameraClass::GetViewMatrix(D3DXMATRIX& viewMatrix)
{
	viewMatrix = this->viewMatrix;
	return;
}

void CameraClass::RenderFirst(D3DXMATRIX rotationMatrix, D3DXVECTOR3 *up)
{


	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	D3DXMATRIX rotateY, transMatrix;
	// Fix camera on object
	D3DXMatrixTranslation(&transMatrix, 0.0f, 1.0f, 0.0f);
	D3DXVec3TransformCoord(&position, &position, &transMatrix);
	
	D3DXVec3TransformCoord(&lookAt, &lookAt, &rotationMatrix);
	//D3DXVec3Normalize(&lookAt, &lookAt);
	D3DXVec3TransformCoord(up, up, &rotationMatrix);
	//D3DXVec3Normalize(up, up);

		
	/*D3DXMatrixRotationY(&rotateY, yaw);
	D3DXVec3TransformNormal(lookAt, lookAt, &rotateY);
	D3DXVec3TransformNormal(up, up, &rotateY);*/

	lookAt = position + lookAt;

	// Finally create the view matrix from the three updated vectors.
	D3DXMatrixLookAtLH(&viewMatrix, &position, &lookAt, up);

	return;
}

void CameraClass::RenderThrid(D3DXMATRIX rotationMatrix, D3DXVECTOR3 *up)
{
	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	D3DXVec3TransformCoord(up, up, &rotationMatrix);
	D3DXVec3TransformCoord(&position, &position, &rotationMatrix);

	position = position + lookAt;

	// Finally create the view matrix from the three updated vectors.
	D3DXMatrixLookAtLH(&viewMatrix, &position, &lookAt, up);

	return;
}
