#include "CarStatus.h"

CarStatus::CarStatus()
{
	mouseStatus_.X = 0;
	mouseStatus_.Y = 0;
	mouseStatus_.Z = 0;
	moveStatus_.direction = 0;
	moveStatus_.turn = 0;
	pos.x = 0;
	pos.y = 0;
	pos.z = 0;
	cameraMode = false;
}

CarStatus::~CarStatus()
{
}

void CarStatus::setMousePos(int x, int y, int z)
{
	mouseStatus_.X = x;
	mouseStatus_.Y = y;
	mouseStatus_.Z = z;
	return;
}

void CarStatus::setTurn(int t)
{
	moveStatus_.turn = t;
	return;
}

void CarStatus::setDirection(int direction)
{
	moveStatus_.direction = direction;
	return;
}

void CarStatus::setPosition(float x, float y, float z)
{
	pos.x = x;
	pos.y = y;
	pos.z = z;
	return;
}

void CarStatus::setCameraMode(bool mode)
{
	cameraMode = mode;
	return;
}

int CarStatus::getMouseX()
{
	return mouseStatus_.X;
}

int CarStatus::getMouseY()
{
	return mouseStatus_.Y;
}

int CarStatus::getMouseZ()
{
	return mouseStatus_.Z;
}

int CarStatus::getMoveTurn()
{
	return moveStatus_.turn;
}

int CarStatus::getMoveDirection()
{
	return moveStatus_.direction;
}

D3DXVECTOR3 CarStatus::getPosition()
{
	return pos;
}

bool CarStatus::getCameraMode()
{
	return cameraMode;
}
