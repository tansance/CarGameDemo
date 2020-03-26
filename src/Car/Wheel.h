#pragma once
#include "Modelclass.h"
#include "LightShaderClass.h"
#include "DepthShader.h"
#include "ShadowShader.h"
#include "LightClass.h"
#include <D3DX10math.h>

class Wheel {
public:
	Wheel();
	Wheel(const Wheel&);
	~Wheel();

	bool Initialize(ID3D11Device*, D3DXVECTOR3* = nullptr);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, LightShaderClass*,LightClass*, D3DXMATRIX, D3DXMATRIX);
	bool RenderDepth(ID3D11DeviceContext*, DepthShaderClass*, D3DXMATRIX, D3DXMATRIX);
	bool RenderWithShadow(ID3D11DeviceContext*, ShadowShaderClass*, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX,
		ID3D11ShaderResourceView*, D3DXVECTOR3, D3DXVECTOR4, D3DXVECTOR4);

	int GetSideIndexCount();
	int GetTireIndexCount();

	ID3D11ShaderResourceView* GetSideTexture();
	ID3D11ShaderResourceView* GetTireTexture();

	void setPosition(float, float, float);
	void increasePositionBy(float, float, float);
	void setDirection(int);
	void increaseRotationBy(float, float, float);
	void roll(int);
private:
	ModelClass sides;
	ModelClass tire;
	D3DXVECTOR3 position;
	D3DXVECTOR3 rotation;
	float direction;
};