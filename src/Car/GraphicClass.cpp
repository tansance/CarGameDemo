#include "GraphicClass.h"

GraphicClass::GraphicClass()
{
	d3d = 0;
	renderTexture = 0;
	lightShader = 0;
	shadowShader = 0;
	depthShader = 0;

	cameraFP = 0;
	cameraTP = 0;
	light = 0;

	skybox = 0;
	car = 0;
	test = 0;
	ground = 0;
	og = 0;
}


GraphicClass::GraphicClass(const GraphicClass& other)
{
}


GraphicClass::~GraphicClass()
{
}


bool GraphicClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;

	//og = new ObjectGenerator();
	//og->GenerateGround();

	// Create the Direct3D object.
	d3d = new D3DClass;
	if (!d3d)
	{
		MessageBox(hwnd, L"Could not Create Direct3D", L"Error", MB_OK);
		return false;
	}

	// Initialize the Direct3D object.
	result = d3d->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
		return false;
	}

	// Create the camera object.
	cameraFP = new CameraClass(FIRST);
	if (!cameraFP)
	{
		MessageBox(hwnd, L"Could not create first person camera", L"Error", MB_OK);
		return false;
	}

	// Set the initial position of the camera.
	cameraFP->SetPosition(-0.0f, 2.0f, -0.0f);

	// Create the camera object.
	cameraTP = new CameraClass(THIRD);
	if (!cameraTP)
	{
		MessageBox(hwnd, L"Could not create third person camera", L"Error", MB_OK);
		return false;
	}

	// Set the initial position of the camera.
	cameraTP->SetPosition(-0.0f, 4.0f, -4.0f);

	// Create the model object.
	car = new Car();
	if (!car)
	{
		MessageBox(hwnd, L"Could not create car", L"Error", MB_OK);
		return false;
	}
	
	// Initialize the model object.
	result = car->Initialize(d3d->GetDevice());
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize car.", L"Error", MB_OK);
		return false;
	}
	car->setPosition(0.0f, 2.0f, 0.0f);


	test = new ModelClass;
	if (!test)
	{
		MessageBox(hwnd, L"Could not create cube", L"Error", MB_OK);
		return false;
	}

	// Initialize the model object.
	result = test->Initialize(d3d->GetDevice(), (char*)"../Car/data/cube.txt", (WCHAR*)L"../Car/data/seafloor.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize cube.", L"Error", MB_OK);
		return false;
	}

	// Create the model object.
	skybox = new Skybox;
	if (!skybox)
	{
		MessageBox(hwnd, L"Could not create skybox", L"Error", MB_OK);
		return false;
	}

	// Initialize the model object.
	result = skybox->Initialize(d3d->GetDevice());
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize skybox.", L"Error", MB_OK);
		return false;
	}

	// Create the model object.
	ground = new Ground();
	if (!ground)
	{
		MessageBox(hwnd, L"Could not create ground", L"Error", MB_OK);
		return false;
	}

	// Initialize the model object.
	result = ground->Initialize(d3d->GetDevice());
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize ground.", L"Error", MB_OK);
		return false;
	}

	// Create the light shader object.
	lightShader = new LightShaderClass;
	if (!lightShader)
	{
		MessageBox(hwnd, L"Could not create light shader object.", L"Error", MB_OK);
		return false;
	}

	// Initialize the light shader object.
	result = lightShader->Initialize(d3d->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the light shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the light object.
	light = new LightClass;
	if (!light)
	{
		MessageBox(hwnd, L"Could not create light.", L"Error", MB_OK);
		return false;
	}

	// Initialize the light object.
	light->SetPosition(5.0f, 15.0f, 20.0f);
	light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	//light->SetDirection(-1.0f, -1.0f, -1.0f);
	light->SetAmbientColor(0.5, 0.5f, 0.5f, 1.0f);
	light->SetLookAt(0.0f, 0.0f, 0.0f);
	light->GenerateProjectionMatrix(100.0f, 1.0f);

	// Create the render to texture object.
	renderTexture = new RenderTextureClass;
	if (!renderTexture)
	{
		MessageBox(hwnd, L"Could not create the render texture object.", L"Error", MB_OK);
		return false;
	}

	// Initialize the render to texture object.
	result = renderTexture->Initialize(d3d->GetDevice(), SHADOWMAP_WIDTH, SHADOWMAP_HEIGHT, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the render texture object.", L"Error", MB_OK);
		return false;
	}

	// Create the depth shader object.
	depthShader = new DepthShaderClass;
	if (!depthShader)
	{
		return false;
	}

	// Initialize the depth shader object.
	result = depthShader->Initialize(d3d->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the depth shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the shadow shader object.
	shadowShader = new ShadowShaderClass;
	if (!shadowShader)
	{
		return false;
	}

	// Initialize the shadow shader object.
	result = shadowShader->Initialize(d3d->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the shadow shader object.", L"Error", MB_OK);
		return false;
	}

	return true;
}


void GraphicClass::Shutdown()
{
	// Release the shadow shader object.
	if (shadowShader)
	{
		shadowShader->Shutdown();
		delete shadowShader;
		shadowShader = 0;
	}

	// Release the depth shader object.
	if (depthShader)
	{
		depthShader->Shutdown();
		delete depthShader;
		depthShader = 0;
	}

	// Release the texture shader object.
	if (renderTexture)
	{
		renderTexture->Shutdown();
		delete renderTexture;
		renderTexture = 0;
	}

	// Release the light object.
	if (light)
	{
		delete light;
		light = 0;
	}

	// Release the color shader object.
	if (lightShader)
	{
		lightShader->Shutdown();
		delete lightShader;
		lightShader = 0;
	}

	// Release the model object.
	if (car)
	{
		car->Shutdown();
		delete car;
		car = 0;
	}

	// Release the skybox object.
	if (skybox)
	{
		skybox->Shutdown();
		delete skybox;
		skybox = 0;
	}

	if (ground) {
		ground->Shutdown();
		delete ground;
		ground = 0;
	}

	// Release the camera object.
	if (cameraFP)
	{
		delete cameraFP;
		cameraFP = 0;
	}

	if (cameraTP)
	{
		delete cameraTP;
		cameraTP = 0;
	}

	// Release the D3D object
	if (d3d)
	{
		d3d->Shutdown();
		delete d3d;
		d3d = 0;
	}
	return;
}


bool GraphicClass::Frame(HWND hwnd, CarStatus* carStatus)
{
	bool result;

	// Render the graphics scene.
	result = Render(hwnd, carStatus);

	if (!result)
	{
		return false;
	}
	return true;
}


bool GraphicClass::Render(HWND hwnd, CarStatus* carStatus)
{
	D3DXMATRIX viewMatrix, projectionMatrix, worldMatrix;
	bool result;
	int mouseX, mouseY;

	result = RenderToTexture(carStatus);
	if (!result) {
		return false;
	}

	// Clear the buffers to begin the scene.
	d3d->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	result = RenderScene(carStatus);
	if (!result) {
		return false;
	}

	// Present the rendered scene to the screen.
	d3d->EndScene();
	return true;
}

bool GraphicClass::RenderToTexture(CarStatus* carStatus)
{
	D3DXMATRIX worldMatrix, lightViewMatrix, lightProjectionMatrix, translateMatrix;
	bool result;

	// Set the render target to be the render to texture.
	renderTexture->SetRenderTarget(d3d->GetDeviceContext());

	// Clear the render to texture.
	renderTexture->ClearRenderTarget(d3d->GetDeviceContext(), 0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the light view matrix based on the light's position.
	light->GenerateViewMatrix();

	// Get the world matrix from the d3d object.
	d3d->GetWorldMatrix(worldMatrix);

	// Get the view and orthographic matrices from the light object.
	light->GetViewMatrix(lightViewMatrix);
	light->GetProjectionMatrix(lightProjectionMatrix);

	// Render Car to depth stencil buffer
	car->go(carStatus->getMoveDirection(), carStatus->getMoveTurn());

	result = car->RenderDepth(d3d->GetDeviceContext(), depthShader, lightViewMatrix, lightProjectionMatrix);
	if (!result) {
		return false;
	}

	// Render ground to depth stencil buffer
	result = ground->RenderDepth(d3d->GetDeviceContext(), depthShader, lightViewMatrix, lightProjectionMatrix);
	if (!result) {
		return false;
	}
	
	// Render test object to depth stencil buffer
	D3DXMatrixTranslation(&worldMatrix, 3.0f, 1.0f, 0.0f);
	test->Render(d3d->GetDeviceContext());
	result = depthShader->Render(d3d->GetDeviceContext(), test->GetIndexCount(), worldMatrix, lightViewMatrix, lightProjectionMatrix);
	if (!result) {
		return false;
	}

	// Reset the render target back to the original back buffer and not the render to texture anymore.
	d3d->SetBackBufferRenderTarget();
	
	// Reset the viewport back to the original.
	d3d->ResetViewport();

	return true;
}

bool GraphicClass::RenderScene(CarStatus* carStatus)
{
	D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix;
	D3DXMATRIX lightViewMatrix, lightProjectionMatrix;
	bool result;
	D3DXVECTOR3 cameraPosition;

	light->GenerateViewMatrix();
	// Get the world, view, and projection matrices from the camera and d3d objects.
	d3d->GetWorldMatrix(worldMatrix);
	d3d->GetProjectionMatrix(projectionMatrix);

	light->GetViewMatrix(lightViewMatrix);
	light->GetProjectionMatrix(lightProjectionMatrix);

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	// Render the model using the light shader.
	car->go(carStatus->getMoveDirection(), carStatus->getMoveTurn());

	// Generate the view matrix based on the camera's position.
	if (carStatus->getCameraMode() == FIRST) {
		cameraFP->SetLookAt(0.0f, 0.0f, 1.0f);
		cameraFP->SetPosition(car->getPosition().x, car->getPosition().y, car->getPosition().z);
		cameraFP->SetRotation(carStatus->getMouseY(), carStatus->getMouseX(), 0.0f);
		cameraFP->IncreaseRotationBy(0.0f, car->getRotation().y, 0.0f);
		cameraFP->Render();
		cameraFP->GetViewMatrix(viewMatrix);
		cameraPosition = cameraFP->GetPosition();
	}
	else {
		cameraTP->SetLookAt(car->getPosition().x, car->getPosition().y, car->getPosition().z);
		cameraTP->SetPosition(0.0f, 0.0f, -10.0f + carStatus->getMouseZ());
		cameraTP->SetRotation(carStatus->getMouseY(), carStatus->getMouseX(), 0.0f);
		cameraTP->Render();
		cameraTP->GetViewMatrix(viewMatrix);
		cameraPosition = cameraTP->GetPosition();
	}

	// Render Car
	result = car->RenderWithShadow(d3d->GetDeviceContext(), shadowShader, viewMatrix, projectionMatrix, lightViewMatrix, lightProjectionMatrix, 
		renderTexture->GetShaderResourceView(), light->GetPosition(), light->GetAmbientColor(), light->GetDiffuseColor());
	if (!result)
	{
		return false;
	}

	// Render ground
	result = ground->RenderWithShadow(d3d->GetDeviceContext(), shadowShader, viewMatrix, projectionMatrix, lightViewMatrix, lightProjectionMatrix,
		renderTexture->GetShaderResourceView(), light->GetPosition(), light->GetAmbientColor(), light->GetDiffuseColor());

	// Render test object
	test->Render(d3d->GetDeviceContext());
	D3DXMATRIX temp;
	D3DXMatrixTranslation(&temp, 3.0f, 1.0f, 0.0f);
	result = shadowShader->Render(d3d->GetDeviceContext(), test->GetIndexCount(), temp, viewMatrix, projectionMatrix, lightViewMatrix,lightProjectionMatrix,
		test->GetTexture(), renderTexture->GetShaderResourceView(), light->GetPosition(), light->GetAmbientColor(), light->GetDiffuseColor());
	if (!result)
	{
		return false;
	}

	// Render skybox
	D3DXMatrixTranslation(&worldMatrix, cameraPosition.x, cameraPosition.y, cameraPosition.z);
	for (int i = 0; i < 6; ++i) {
		skybox->Render(d3d->GetDeviceContext(), i);
		result = lightShader->Render(d3d->GetDeviceContext(), skybox->GetIndexCount(i), worldMatrix, viewMatrix, projectionMatrix,
			skybox->GetTexture(i), light->GetDirection(), light->GetDiffuseColor());
		if (!result)
		{
			return false;
		}
	}

	return true;
}