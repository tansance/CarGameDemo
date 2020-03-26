#include "GameDemo.h"

GameDemo::GameDemo()
{
	Input_ = 0;
	Graphics_ = 0;
	carStatus_ = 0;
}

GameDemo::GameDemo(const GameDemo& other)
{
}


GameDemo::~GameDemo()
{
}

bool GameDemo::Initialize()
{
	bool result;
	
	// Initialize the width and height of the screen to zero before sending the variables into the function.
	screenWidth = 0;
	screenHeight = 0;

	// Initialize the windows api.
	InitializeWindows(screenWidth, screenHeight);

	// Create the Input_ object.  This object will be used to handle reading the keyboard Input_ from the user.
	Input_ = new InputClass;
	if (!Input_)	{
		return false;
	}

	// Initialize the Input_ object.
	result = Input_->Initialize(hinstance_, hwnd_, screenWidth, screenHeight);
	if (!result) {
		MessageBox(hwnd_, L"Could not initialize the input device.", L"Error", MB_OK);
		return false;
	}

	// Create the Graphics_ object.  This object will handle rendering all the Graphics_ for this application.
	Graphics_ = new GraphicClass;
	if (!Graphics_)	{
		return false;
	}

	// Initialize the Graphics_ object.
	result = Graphics_->Initialize(screenWidth, screenHeight, hwnd_);
	if (!result)	{
		return false;
	}

	// Initialize the CarStatus object.
	carStatus_ = new CarStatus;

	return true;
}

void GameDemo::Shutdown()
{
	// Release the Graphics_ object.
	if (Graphics_)
	{
		Graphics_->Shutdown();
		delete Graphics_;
		Graphics_ = 0;
	}

	// Release the Input_ object.
	if (Input_)
	{
		Input_->Shutdown();
		delete Input_;
		Input_ = 0;
	}

	if (carStatus_) {
		delete carStatus_;
		carStatus_ = 0;
	}

	// Shutdown the window.
	ShutdownWindows();

	return;
}

void GameDemo::Run()
{
	MSG msg;
	bool done, result;


	// Initialize the message structure.
	ZeroMemory(&msg, sizeof(MSG));

	// Loop until there is a quit message from the window or the user.
	done = false;
	while (!done)
	{
		// Handle the windows messages.
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// If windows signals to end the application then exit out.
		if (msg.message == WM_QUIT) {
			done = true;
		}
		else {
			// Otherwise do the frame processing.
			result = Frame();
			if (!result)
			{
				MessageBox(hwnd_, L"Frame Processing Failed", L"Error", MB_OK);
				done = true;
			}
		}
		if (Input_->IsEscapePressed()) {
			done = true;
		}
	}

	return;
}

bool GameDemo::Frame()
{
	bool result;

	result = Input_->Frame();
	if (!result) {
		return false;
	}

	updateCarStatus();

	// Do the frame processing for the Graphics_ object.
	result = Graphics_->Frame(hwnd_, carStatus_);
	if (!result) {
		return false;
	}

	Input_->refreshKeyboardState();

	return true;
}

void GameDemo::updateCarStatus()
{
	int mouseX, mouseY, mouseZ;
	Input_->GetMouseLocation(mouseX, mouseY, mouseZ);
	carStatus_->setMousePos(mouseX, mouseY, mouseZ);

	if (Input_->IsGoForward()) {
		carStatus_->setDirection(1);
	}
	if (Input_->IsGoBackword()) {
		carStatus_->setDirection(-1);
	}
	if (Input_->IsStop()) {
		carStatus_->setDirection(0);
	}
	if (Input_->IsStopTurn()) {
		carStatus_->setTurn(0);
	}
	if (Input_->IsTurnLeft()) {
		carStatus_->setTurn(-1);
	}
	if (Input_->IsTurnRight()) {
		carStatus_->setTurn(1);
	}
	if (Input_->IsSwitchCamera()) {
		if (carStatus_->getCameraMode() == FIRST) {
			carStatus_->setCameraMode(THIRD);
		}
		else {
			carStatus_->setCameraMode(FIRST);
		}
	}
	
	return;
}

LRESULT CALLBACK GameDemo::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	return DefWindowProc(hwnd, umsg, wparam, lparam);
}

void GameDemo::InitializeWindows(int& screenWidth, int& screenHeight)
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;


	// Get an external pointer to this object.
	ApplicationHandle = this;

	// Get the instance of this application.
	hinstance_ = GetModuleHandle(NULL);

	// Give the application a name.
	appName = L"Engine";

	// Setup the windows class with default settings.
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hinstance_;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = appName;
	wc.cbSize = sizeof(WNDCLASSEX);

	// Register the window class.
	RegisterClassEx(&wc);

	// Determine the resolution of the clients desktop screen.
	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	// Setup the screen settings depending on whether it is running in full screen or in windowed mode.
	if (FULL_SCREEN)
	{
		// If full screen set the screen to maximum size of the users desktop and 32bit.
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Change the display settings to full screen.
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// Set the position of the window to the top left corner.
		posX = posY = 0;
	}
	else
	{
		// If windowed then set it to 800x600 resolution.
		screenWidth = 800;
		screenHeight = 600;

		// Place the window in the middle of the screen.
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	// Create the window with the screen settings and get the handle to it.
	hwnd_ = CreateWindowEx(WS_EX_APPWINDOW, appName, appName,
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
		posX, posY, screenWidth, screenHeight, NULL, NULL, hinstance_, NULL);

	// Bring the window up on the screen and set it as main focus.
	ShowWindow(hwnd_, SW_SHOW);
	SetForegroundWindow(hwnd_);
	SetFocus(hwnd_);

	// Hide the mouse cursor.
	ShowCursor(false);

	return;
}

void GameDemo::ShutdownWindows()
{
	// Show the mouse cursor.
	ShowCursor(true);

	// Fix the display settings if leaving full screen mode.
	if (FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	// Remove the window.
	DestroyWindow(hwnd_);
	hwnd_ = NULL;

	// Remove the application instance.
	UnregisterClass(appName, hinstance_);
	hinstance_ = NULL;

	// Release the pointer to this class.
	ApplicationHandle = NULL;

	return;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
		// Check if the window is being destroyed.
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}

	// Check if the window is being closed.
	case WM_CLOSE:
	{
		PostQuitMessage(0);
		return 0;
	}

	// All other messages pass to the message handler in the system class.
	default:
	{
		return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
	}
	}
}
