#pragma once
#include "Modelclass.h"
#include <vector>

class Skybox {
public:
	Skybox();
	Skybox(const Skybox&);
	~Skybox();

	bool Initialize(ID3D11Device*);
	void Shutdown();
	void Render(ID3D11DeviceContext*, int);

	int GetIndexCount(int i);

	ID3D11ShaderResourceView* GetTexture(int i);
private:
	ModelClass* box;
	D3DXVECTOR3 position;
};