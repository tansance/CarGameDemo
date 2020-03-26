#pragma once
#include"Modelclass.h"
#include "ShadowShader.h"

class Ground {
public:
	Ground();
	Ground(const Ground&);
	~Ground();

	bool Initialize(ID3D11Device*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);
	bool RenderDepth(ID3D11DeviceContext*, DepthShaderClass*, D3DXMATRIX, D3DXMATRIX);
	bool RenderWithShadow(ID3D11DeviceContext*, ShadowShaderClass*, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX,
		ID3D11ShaderResourceView *, D3DXVECTOR3 , D3DXVECTOR4 , D3DXVECTOR4 );

	int GetIndexCount();

	ID3D11ShaderResourceView* GetTexture();

private:
	ModelClass ground;
};