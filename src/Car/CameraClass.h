#pragma once
#include <d3dx10math.h>

#define THIRD true
#define FIRST false
#define XLIMIT D3DX_PI/2
#define YLIMIT D3DX_PI

class CameraClass
{
public:
	CameraClass();
	CameraClass(bool);
	CameraClass(const CameraClass&);
	~CameraClass();

	void SetPosition(float, float, float);
	void SetLookAt(float, float, float);
	void SetRotation(float, float, float);
	void IncreaseRotationBy(float, float, float);

	D3DXVECTOR3 GetPosition();
	D3DXVECTOR3 GetRotation();

	void Render();
	void GetViewMatrix(D3DXMATRIX&);

private:
	void RenderFirst(D3DXMATRIX, D3DXVECTOR3*);
	void RenderThrid(D3DXMATRIX,  D3DXVECTOR3*);

private:
	bool cameraMode;
	D3DXVECTOR3 position, lookAt;
	float rotationX, rotationY, rotationZ;
	D3DXMATRIX viewMatrix;
};