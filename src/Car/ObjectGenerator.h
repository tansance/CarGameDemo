#pragma once
#include <d3dx10math.h>

class ObjectGenerator {
private:
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
		D3DXVECTOR3 normal;
	};
public:
	ObjectGenerator();
	ObjectGenerator(const ObjectGenerator&);
	~ObjectGenerator();

	
	bool GenerateSides(float, float, VertexType**, int*);
	bool GenerateBody(float, float);
	bool GenerateGround();
private:
	bool GenerateCircle(float, VertexType**, unsigned long**, int*, int*);
	bool OutputToFile(VertexType*, unsigned long*, int, char*);
};