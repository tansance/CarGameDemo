#pragma once
#include "Wheel.h"
#define TURN_ANGLE D3DX_PI/90.0f

class Car {
public:
	Car();
	Car(const Car&);
	~Car();
	
	bool Initialize(ID3D11Device*, D3DXVECTOR3* = nullptr);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, LightShaderClass*, LightClass*, D3DXMATRIX, D3DXMATRIX);
	bool RenderDepth(ID3D11DeviceContext*, DepthShaderClass*, D3DXMATRIX, D3DXMATRIX);
	bool RenderWithShadow(ID3D11DeviceContext*, ShadowShaderClass*, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX,
		ID3D11ShaderResourceView* , D3DXVECTOR3 , D3DXVECTOR4, D3DXVECTOR4);

	void setPosition(float, float, float);
	D3DXVECTOR3 getPosition();
	D3DXVECTOR3 getRotation();
	void increasePositionBy(float, float, float);

	void increaseWheelRotationBy(float, float, float);
	void setWheelTransMatrix(float);
	void go(int, int);
private:
	ModelClass body;
	Wheel* wheel;
	D3DXVECTOR3 position;
	D3DXVECTOR3 rotation;
	D3DXVECTOR3 direction;
	D3DXVECTOR3* wheelTransVector;
	D3DXMATRIX* wheelTransMatrix;
};