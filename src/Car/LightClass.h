#pragma once
#include <d3dx10math.h>

////////////////////////////////////////////////////////////////////////////////
// Class name: LightClass
////////////////////////////////////////////////////////////////////////////////
class LightClass
{
public:
	LightClass();
	LightClass(const LightClass&);
	~LightClass();

	void SetAmbientColor(float, float, float, float);
	void SetDiffuseColor(float, float, float, float);
	void SetDirection(float, float, float);
	void SetPosition(float, float, float);
	void SetLookAt(float, float, float);

	D3DXVECTOR4 GetAmbientColor();
	D3DXVECTOR4 GetDiffuseColor();
	D3DXVECTOR3 GetDirection();
	D3DXVECTOR3 GetPosition();

	void GenerateViewMatrix();
	void GenerateProjectionMatrix(float, float);

	void GetViewMatrix(D3DXMATRIX&);
	void GetProjectionMatrix(D3DXMATRIX&);
private:
	D3DXVECTOR4 ambientColor;
	D3DXVECTOR4 diffuseColor_;
	D3DXVECTOR3 direction_;
	D3DXVECTOR3 position;
	D3DXVECTOR3 lookAt;
	D3DXMATRIX viewMatrix;
	D3DXMATRIX projectionMatrix;
};