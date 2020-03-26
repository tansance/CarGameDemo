#pragma once

#include <d3d11.h>
#include <d3dx10math.h>
#include <fstream>
#include "TextureClass.h"
#include "DepthShader.h"

using namespace std;

class ModelClass
{
private:
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
		D3DXVECTOR3 normal;
	};
	//used for loading models from file
	struct ModelType
	{
		float x, y, z; //pos
		float tu, tv; //texture
		float nx, ny, nz; //normal
	};

public:
	ModelClass();
	ModelClass(const ModelClass&);
	~ModelClass();

	bool Initialize(ID3D11Device*, char*, WCHAR*, D3DXVECTOR3* = nullptr);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();

	ID3D11ShaderResourceView* GetTexture();
private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTexture(ID3D11Device*, WCHAR*);
	void ReleaseTexture();

	bool LoadModel(char*);
	void ReleaseModel();
private:
	D3DXVECTOR3 scale;
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;
	TextureClass* m_Texture;
	ModelType* model;
};