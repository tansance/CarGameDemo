#include "GameDemo.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	GameDemo* Game;
	bool result;


	// Create the system object.
	Game = new GameDemo;
	if (!Game)
	{
		return 0;
	}

	// Initialize and run the system object.
	result = Game->Initialize();
	if (result)
	{
		Game->Run();
	}

	// Shutdown and release the system object.
	Game->Shutdown();
	delete Game;
	Game = 0;

	return 0;
}