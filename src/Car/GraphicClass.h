#pragma once
#include <windows.h>
#include "D3DClass.h"
#include "InputClass.h"
#include "ObjectGenerator.h"
#include "CarStatus.h"

#include "LightShaderClass.h"
#include "ShadowShader.h"
#include "lightclass.h"
#include "RenderTexture.h"
#include "DepthShader.h"

#include "CameraClass.h"
#include "Skybox.h"
#include "Car.h"
#include "Ground.h"


const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 100000.0f;
const float SCREEN_NEAR = 0.1f;

const int SHADOWMAP_WIDTH = 1024;
const int SHADOWMAP_HEIGHT = 1024;

class GraphicClass
{
public:
	GraphicClass();
	GraphicClass(const GraphicClass&);
	~GraphicClass();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame(HWND, CarStatus*);

private:
	bool RenderToTexture(CarStatus*);
	bool RenderScene(CarStatus*);
	bool Render(HWND, CarStatus*);

private:
	D3DClass* d3d;
	CameraClass* cameraFP,* cameraTP;
	LightShaderClass* lightShader;

	Car* car;
	Skybox* skybox;
	Ground* ground;
	LightClass* light;
	ModelClass* test;

	ObjectGenerator* og;

	RenderTextureClass* renderTexture;
	DepthShaderClass* depthShader;
	ShadowShaderClass* shadowShader;
};