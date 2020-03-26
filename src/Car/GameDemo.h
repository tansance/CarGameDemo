#pragma once
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include "InputClass.h"
#include "GraphicClass.h"
#include "CarStatus.h"

class GameDemo
{
public:
	GameDemo();
	GameDemo(const GameDemo&);
	~GameDemo();

	bool Initialize();
	void Shutdown();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	bool Frame();
	void updateCarStatus();
	void InitializeWindows(int&, int&);
	void ShutdownWindows();

private:
	int screenWidth, screenHeight;
	LPCWSTR appName;
	HINSTANCE hinstance_;
	HWND hwnd_;

	InputClass* Input_;
	GraphicClass* Graphics_;
	CarStatus* carStatus_;
};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

static GameDemo* ApplicationHandle = 0;