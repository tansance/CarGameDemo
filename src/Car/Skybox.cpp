#include "Skybox.h"

Skybox::Skybox()
{
	box = 0;
}

Skybox::Skybox(const Skybox &)
{
}

Skybox::~Skybox()
{
}

bool Skybox::Initialize(ID3D11Device * device)
{
	bool result;
	D3DXVECTOR3 scale(10000.0f, 10000.0f, 10000.0f);
	box = new ModelClass[6];
	result = box[0].Initialize(device, (char*)"../Car/data/bottom.txt", (WCHAR*)L"../Car/data/bottom.dds", &scale);
	if (!result) {
		return false;
	}
	result = box[1].Initialize(device, (char*)"../Car/data/front.txt", (WCHAR*)L"../Car/data/front.dds", &scale);
	if (!result) {
		return false;
	}
	result = box[2].Initialize(device, (char*)"../Car/data/back.txt", (WCHAR*)L"../Car/data/back.dds", &scale);
	if (!result) {
		return false;
	}
	result = box[3].Initialize(device, (char*)"../Car/data/left.txt", (WCHAR*)L"../Car/data/left.dds", &scale);
	if (!result) {
		return false;
	}
	result = box[4].Initialize(device, (char*)"../Car/data/right.txt", (WCHAR*)L"../Car/data/right.dds", &scale);
	if (!result) {
		return false;
	}
	result = box[5].Initialize(device, (char*)"../Car/data/top.txt", (WCHAR*)L"../Car/data/top.dds", &scale);
	if (!result) {
		return false;
	}
	return true;
}

void Skybox::Shutdown()
{
	for (int i = 0; i < 6; ++i) {
		box[i].Shutdown();
	}
}

void Skybox::Render(ID3D11DeviceContext *deviceContext, int i)
{
	box[i].Render(deviceContext);
}

int Skybox::GetIndexCount(int i)
{
	return box[i].GetIndexCount();
}

ID3D11ShaderResourceView * Skybox::GetTexture(int i)
{
	return box[i].GetTexture();
}

