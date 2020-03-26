#include "InputClass.h"


InputClass::InputClass()
{
	directInput = 0;
	keyboard = 0;
	mouse = 0;
}

InputClass::InputClass(const InputClass& other)
{
}

InputClass::~InputClass()
{
}


bool InputClass::Initialize(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight)
{
	HRESULT result;
	
	// Store the screen size which will be used for positioning the mouse cursor.
	this->screenWidth = screenWidth;
	this->screenHeight = screenHeight;

	// Initialize the location of the mouse on the screen.
	mouseX = screenWidth /2.0f;
	mouseY = screenHeight/2.0f;
	mouseZ = 3;

	// Initialize the main direct input interface.
	result = DirectInput8Create(hinstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput, NULL);
	if (FAILED(result))
	{
		return false;
	}

	// Initialize the direct input interface for the keyboard.
		result = directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
	if (FAILED(result))
	{
		return false;
	}

	// Set the data format.  In this case since it is a keyboard we can use the predefined data format.
	result = keyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(result))
	{
		return false;
	}

	// Set the cooperative level of the keyboard to not share with other programs.
	result = keyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
	if (FAILED(result))
	{
		return false;
	}

	// Now acquire the keyboard.
	result = keyboard->Acquire();
	if (FAILED(result))
	{
		return false;
	}

	// Initialize the direct input interface for the mouse.
	result = directInput->CreateDevice(GUID_SysMouse, &mouse, NULL);
	if (FAILED(result))
	{
		return false;
	}

	// Set the data format for the mouse using the pre-defined mouse data format.
	result = mouse->SetDataFormat(&c_dfDIMouse);
	if (FAILED(result))
	{
		return false;
	}

	// Set the cooperative level of the mouse to share with other programs.
	result = mouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(result))
	{
		return false;
	}
	
	// Acquire the mouse.
	result = mouse->Acquire();
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void InputClass::Shutdown()
{
	// Release the mouse.
	if (mouse)
	{
		mouse->Unacquire();
		mouse->Release();
		mouse = 0;
	}

	// Release the keyboard.
	if (keyboard)
	{
		keyboard->Unacquire();
		keyboard->Release();
		keyboard = 0;
	}

	// Release the main interface to direct input.
	if (directInput)
	{
		directInput->Release();
		directInput = 0;
	}

	return;
}

bool InputClass::Frame()
{
	bool result;

	// Read the current state of the keyboard.
	result = ReadKeyboard();
	if (!result)
	{
		return false;
	}

	// Read the current state of the mouse.
	result = ReadMouse();
	if (!result)
	{
		return false;
	}

	// Process the changes in the mouse and keyboard.
	ProcessInput();

	return true;
}

bool InputClass::ReadKeyboard()
{
	HRESULT result;
	
	// Read the keyboard device.
	result = keyboard->GetDeviceState(sizeof(keyboardState), (LPVOID)&keyboardState);
	if (FAILED(result))
	{
		// If the keyboard lost focus or was not acquired then try to get control back.
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			keyboard->Acquire();
		}
		else
		{
			return false;
		}
	}

	return true;
}

bool InputClass::ReadMouse()
{
	HRESULT result;
	
	// Read the mouse device.
	result = mouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&mouseState);
	if (FAILED(result))
	{
		// If the mouse lost focus or was not acquired then try to get control back.
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			mouse->Acquire();
		}
		else
		{
			return false;
		}
	}

	return true;
}

void InputClass::ProcessInput()
{
	// Update the location of the mouse cursor based on the change of the mouse location during the frame.
	mouseX += mouseState.lX;
	mouseY += mouseState.lY;
	if (mouseState.lZ > 0)
		mouseZ++;
	if (mouseState.lZ < 0)
		mouseZ--;

	// Ensure the mouse location doesn't exceed the screen width or height.
	if (mouseX < 0) { mouseX = 0; }
	if (mouseY < 0) { mouseY = 0; }
	if (mouseZ < 0) { mouseZ = 0; }

	if (mouseX > screenWidth) { mouseX = screenWidth; }
	if (mouseY > screenHeight) { mouseY = screenHeight; }
	if (mouseZ > 6) { mouseZ = 6; }

	return;
}

// Check whether is key pressed
bool InputClass::IsKeyDown(unsigned char * keyboard, int key)
{
	return keyboard[key] & 0x80;
}

// Check whether ESC is pressed
bool InputClass::IsEscapePressed()
{
	return IsKeyDown(keyboardState, DIK_ESCAPE);
}

// Check whether V is pressed
bool InputClass::IsSwitchCamera()
{
	return IsKeyDown(keyboardState, DIK_V) && !IsKeyDown(prevKeyboardState, DIK_V);
}

// Check whether W is pressed
bool InputClass::IsGoForward()
{
	return IsKeyDown(prevKeyboardState,DIK_W) && IsKeyDown(keyboardState,DIK_W);
}

// Check whether S is pressed
bool InputClass::IsGoBackword()
{
	return IsKeyDown(prevKeyboardState, DIK_S) && IsKeyDown(keyboardState, DIK_S);
}

// Check whether W or S is upped
bool InputClass::IsStop()
{
	if (IsKeyDown(prevKeyboardState, DIK_W) && !IsKeyDown(keyboardState, DIK_W))
		return true;
	if (IsKeyDown(prevKeyboardState, DIK_S) && !IsKeyDown(keyboardState, DIK_S))
		return true;
	return false;
}

// Check whether D is pressed last
bool InputClass::IsTurnRight()
{
	if(!IsKeyDown(prevKeyboardState, DIK_D) && IsKeyDown(keyboardState, DIK_D))
		return true;
	if(!IsKeyDown(keyboardState, DIK_A) && IsKeyDown(keyboardState, DIK_D))
		return true;
	return false;
}

// Check whether A is pressed last
bool InputClass::IsTurnLeft()
{
	if (!IsKeyDown(prevKeyboardState, DIK_A) && IsKeyDown(keyboardState, DIK_A))
		return true;
	if (!IsKeyDown(keyboardState, DIK_D) && IsKeyDown(keyboardState, DIK_A))
		return true;
	return false;
}

// Check whether A or D is upped
bool InputClass::IsStopTurn()
{
	if (IsKeyDown(prevKeyboardState, DIK_D) && !IsKeyDown(keyboardState, DIK_D))
		return true;
	if (IsKeyDown(prevKeyboardState, DIK_A) && !IsKeyDown(keyboardState, DIK_A))
		return true;
	return false;
}

// Get mouse location
void InputClass::GetMouseLocation(int& mouseX, int& mouseY, int& mouseZ)
{
	mouseX = this->mouseX - screenWidth/2;
	mouseY = this->mouseY - screenHeight/2;
	mouseZ = this->mouseZ - 3;
	return;
}

// Refresh keyboard State
void InputClass::refreshKeyboardState()
{
	memcpy(prevKeyboardState, keyboardState, sizeof(keyboardState));
}
