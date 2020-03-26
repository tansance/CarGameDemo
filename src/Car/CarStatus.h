#pragma once
#include <d3dx10math.h>

class CarStatus {
private:
	struct mousePos{
		int X;
		int Y;
		int Z;
	};
	struct moveStatus {
		int turn;
		int direction;
	};
public:
	CarStatus();
	~CarStatus();
	void setMousePos(int , int, int);
	void setTurn(int);
	void setDirection(int);
	void setPosition(float, float, float);
	void setCameraMode(bool);
	int getMouseX();
	int getMouseY();
	int getMouseZ();
	int getMoveTurn();
	int getMoveDirection();
	D3DXVECTOR3 getPosition();
	bool getCameraMode();
private:
	mousePos mouseStatus_;
	moveStatus moveStatus_;
	D3DXVECTOR3 pos;
	bool cameraMode;
};