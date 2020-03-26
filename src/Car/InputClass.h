#pragma once
#define DIRECTINPUT_VERSION 0x0800
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#include <dinput.h>

class InputClass
{
public:
	InputClass();
	InputClass(const InputClass&);
	~InputClass();

	bool Initialize(HINSTANCE, HWND, int, int);
	void Shutdown();
	bool Frame();

	bool IsKeyDown(unsigned char*, int);
	bool IsEscapePressed();
	bool IsSwitchCamera();
	bool IsGoForward();
	bool IsGoBackword();
	bool IsStop();
	bool IsTurnRight();
	bool IsTurnLeft();
	bool IsStopTurn();
	void GetMouseLocation(int&, int&, int&);

	void refreshKeyboardState();

private:
	bool ReadKeyboard();
	bool ReadMouse();
	void ProcessInput();

private:
	IDirectInput8* directInput;
	IDirectInputDevice8* keyboard;
	IDirectInputDevice8* mouse;

	unsigned char keyboardState[256];
	unsigned char prevKeyboardState[256];
	DIMOUSESTATE mouseState;

	int screenWidth, screenHeight;
	int mouseX, mouseY, mouseZ;
};