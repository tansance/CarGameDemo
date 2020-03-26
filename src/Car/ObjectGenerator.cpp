#include "ObjectGenerator.h"
#include <fstream>
#include<iomanip>


using namespace std;

ObjectGenerator::ObjectGenerator()
{
}

ObjectGenerator::ObjectGenerator(const ObjectGenerator &)
{
}

ObjectGenerator::~ObjectGenerator()
{
}

bool ObjectGenerator::GenerateCircle(float r, VertexType** vertices_, unsigned long** indices_, int *vertexCount, int *indexCount)
{
	D3DXMATRIX trans, rotation;
	D3DXVECTOR3 pos(0.0f, r, 0.0), temptext(0.0f, 0.0f, 0.0f), texture(0.0f, 0.5f, 0.0f);
	VertexType *vertices;
	unsigned long * indices;
	ofstream fout;

	*vertexCount = 181;
	*indexCount = 180 * 3;

	D3DXMatrixTranslation(&trans, 0.0f, -0.5f, 0.5f);
	D3DXMatrixRotationYawPitchRoll(&rotation, 0.0f, D3DX_PI / 90.0f, 0.0f);

	vertices = new VertexType[181];
	indices = new unsigned long[180 * 3];

	vertices[0].position.x = 0.0f; vertices[0].position.y = 0.0f; vertices[0].position.z = 0.0f;
	vertices[0].texture.x = 0.5f; vertices[0].texture.y = 0.5f;
	vertices[0].normal.x = 1.0f; vertices[0].normal.y = 0.0f; vertices[0].normal.z = 0.0f;

	for (int i = 1; i < 181; ++i) {
		D3DXVec3TransformCoord(&temptext, &texture, &trans);
		vertices[i].position.x = pos.x; vertices[i].position.y = pos.y; vertices[i].position.z = pos.z;
		vertices[i].texture.x = temptext.z; vertices[i].texture.y = -temptext.y;
		vertices[i].normal.x = 1.0f; vertices[i].normal.y = 0.0f; vertices[i].normal.z = 0.0f;
		D3DXVec3TransformCoord(&pos, &pos, &rotation);
		D3DXVec3TransformCoord(&texture, &texture, &rotation);
	}

	for (int i = 0; i < 179; ++i) {
		indices[i * 3] = 0;
		indices[i * 3 + 1] = i + 1;
		indices[i * 3 + 2] = i + 2;
	}
	indices[179 * 3] = 0;
	indices[179 * 3 + 1] = 180;
	indices[179 * 3 + 2] = 1;

	*vertices_ = vertices;
	*indices_ = indices;

	fout.open("../Car/data/circle.txt");

	if (fout.fail()) {
		return false;
	}

	fout << "Vertex Count: 540\n";
	fout << "\n";
	fout << "Data:\n\n";
	fout << setiosflags(ios::fixed) << setprecision(4);
	for (int i = 0; i < 540; ++i) {
		fout << vertices[indices[i]].position.x << ' ' << vertices[indices[i]].position.y << ' ' << vertices[indices[i]].position.z << ' ';
		fout << vertices[indices[i]].texture.x << ' ' << vertices[indices[i]].texture.y << ' ';
		fout << vertices[indices[i]].normal.x << ' ' << vertices[indices[i]].normal.y << ' ' << vertices[indices[i]].normal.z << '\n';
	}

	fout.close();

	/*delete[] vertices;
	vertices = 0;
	delete[] indices;
	indices = 0;*/
	return false;
}

bool ObjectGenerator::GenerateSides(float r, float height, VertexType** vertices, int * vertexCount)
{
	VertexType* sideRight, *sides, *cylinder;
	unsigned long* rightIndices,  *sidesIndices, *cylinderIndices;
	int sideIndexCount, sideVertexCount;
	D3DXMATRIX transRight, transLeft;

	D3DXMatrixTranslation(&transRight, height/2.0f, -0.0f, 0.0f);
	D3DXMatrixTranslation(&transLeft, -height/2.0f, -0.0f, 0.0f);

	rightIndices = 0;
	sideRight = 0;
	GenerateCircle(r, &sideRight, &rightIndices, &sideVertexCount, &sideIndexCount);

	sides = new VertexType[sideVertexCount*2];
	sidesIndices = new unsigned long[sideIndexCount * 2];
	
	// transform circles and store them in one array
	for (int i = 0; i < sideVertexCount; ++i) {
		D3DXVec3TransformCoord(&sides[i + sideVertexCount].position, &sideRight[i].position, &transLeft);
		D3DXVec3TransformCoord(&sides[i].position, &sideRight[i].position, &transRight);

		sides[i].texture.x = sideRight[i].texture.x; 
		sides[i].texture.y = sideRight[i].texture.y;
		sides[i + sideVertexCount].texture.x = sideRight[i].texture.x;
		sides[i + sideVertexCount].texture.y = sideRight[i].texture.y;

		sides[i].normal.x = sideRight[i].normal.x; 
		sides[i].normal.y = sideRight[i].normal.y; 
		sides[i].normal.z = sideRight[i].normal.z;

		sides[i + sideVertexCount].normal.x = -sideRight[i].normal.x;
		sides[i + sideVertexCount].normal.y = sideRight[i].normal.y;
		sides[i + sideVertexCount].normal.z = sideRight[i].normal.z;
	}

	for (int i = 0; i < sideIndexCount; ++i) {
		sidesIndices[i] = rightIndices[i];
	}

	for (int i = 0; i < sideVertexCount - 2; ++i) {
		int j = i + sideVertexCount - 1;
		sidesIndices[j * 3] = sideVertexCount;
		sidesIndices[j * 3 + 1] = j + 3;
		sidesIndices[j * 3 + 2] = j + 2;
	}
	sidesIndices[((sideVertexCount - 1) * 2 - 1) * 3] = sideVertexCount;
	sidesIndices[((sideVertexCount - 1) * 2 - 1) * 3 + 1] = sideVertexCount + 1;
	sidesIndices[((sideVertexCount - 1) * 2 - 1) * 3 + 2] = sideVertexCount * 2 - 1;

	OutputToFile(sides, sidesIndices, sideIndexCount * 2, (char*)"../Car/data/tireSides.txt");

	delete[] sideRight;
	delete[] rightIndices;
	delete[] sidesIndices;

	*vertices = sides;
	*vertexCount = sideVertexCount * 2;

	return false;
}

bool ObjectGenerator::GenerateBody(float r, float height)
{
	VertexType * vertices;
	unsigned long * indices;
	int vertexCount, sideVertexCount;

	vertices = 0;
	indices = 0;

	GenerateSides(r, height, &vertices, &sideVertexCount);

	//translate side vertices into body vertices
	vertexCount = sideVertexCount / 2;

	vertices[0] = vertices[1];
	vertices[0].normal.x = 0;
	vertices[0].normal.y = vertices[0].position.y;
	vertices[0].normal.z = vertices[0].position.z;
	vertices[0].texture.x = 1.0f;
	vertices[0].texture.y = 1.0f;

	vertices[vertexCount] = vertices[vertexCount + 1];
	vertices[vertexCount].normal.x = 0;
	vertices[vertexCount].normal.y = vertices[vertexCount].position.y;
	vertices[vertexCount].normal.z = vertices[vertexCount].position.z;
	vertices[vertexCount].texture.x = 0.0f;
	vertices[vertexCount].texture.y = 1.0f;

	for (int i = 1; i < vertexCount; ++i) {
		vertices[i].normal.x = 0;
		vertices[i].normal.y = vertices[i].position.y;
		vertices[i].normal.z = vertices[i].position.z;
		vertices[i].texture.x = 1.0f;
		vertices[i].texture.y = (i - 1) / 180.0f;

		vertices[i + vertexCount].normal.x = 0;
		vertices[i + vertexCount].normal.y = vertices[i + vertexCount].position.y;
		vertices[i + vertexCount].normal.z = vertices[i + vertexCount].position.z;
		vertices[i + vertexCount].texture.x = 0.0f;
		vertices[i + vertexCount].texture.y = (i - 1) / 180.0f;
	}

	// form indices array
	indices = new unsigned long[180 * 6];
	
	for (int i = 0; i < 179; ++i) {
		indices[i * 6] = i + 1;
		indices[i * 6 + 1] = i + 1 + vertexCount;
		indices[i * 6 + 2] = i + 2 + vertexCount;
		indices[i * 6 + 3] = i + 1;
		indices[i * 6 + 4] = i + 2 + vertexCount;
		indices[i * 6 + 5] = i + 2;
	}
	indices[179 * 6] = 180;
	indices[179 * 6 + 1] = 361;
	indices[179 * 6 + 2] = 181;
	indices[179 * 6 + 3] = 180;
	indices[179 * 6 + 4] = 181;
	indices[179 * 6 + 5] = 0;

	OutputToFile(vertices, indices, 180 * 6, (char*)"../Car/data/body.txt");

	delete[] vertices;
	delete[] indices;
	return false;
}

bool ObjectGenerator::GenerateGround()
{
	VertexType * vertices;
	unsigned long * indices;
	int lineVerticesCount = 21;

	vertices = new VertexType[lineVerticesCount*lineVerticesCount];
	indices = new unsigned long[(lineVerticesCount-1)*(lineVerticesCount-1)*6];

	for (int i = 0; i < lineVerticesCount; ++i) {
		for (int j = 0; j < lineVerticesCount; ++j) {
			vertices[i*lineVerticesCount + j].position.x = -5000 + j * 500;
			vertices[i*lineVerticesCount + j].position.y = 0;
			vertices[i*lineVerticesCount + j].position.z = 5000 - i * 500;
			if (j % 2 == 0) vertices[i*lineVerticesCount + j].texture.x = 0;
			else vertices[i*lineVerticesCount + j].texture.x = 1;
			if (i % 2 == 0) vertices[i*lineVerticesCount + j].texture.y = 0;
			else vertices[i*lineVerticesCount + j].texture.y = 1;
			vertices[i*lineVerticesCount + j].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		}
	}

	for (int i = 0; i < lineVerticesCount - 1; ++i) {
			for (int j = 0; j < lineVerticesCount - 1; ++j) {
				indices[(i*(lineVerticesCount - 1) + j) * 6] = i * lineVerticesCount + j;
				indices[(i*(lineVerticesCount - 1) + j) * 6 + 1] = (i + 1) * lineVerticesCount + j + 1;
				indices[(i*(lineVerticesCount - 1) + j) * 6 + 2] = (i + 1) * lineVerticesCount + j;
				indices[(i*(lineVerticesCount - 1) + j) * 6 + 3] = i * lineVerticesCount + j;
				indices[(i*(lineVerticesCount - 1) + j) * 6 + 4] = i * lineVerticesCount + j + 1;
				indices[(i*(lineVerticesCount - 1) + j) * 6 + 5] = (i + 1) * lineVerticesCount + j + 1;
		}
	}

	OutputToFile(vertices, indices, 2400, (char*)"../Car/data/ground.txt");

	delete[] vertices;
	delete[] indices;

	return false;
}

bool ObjectGenerator::OutputToFile(VertexType * vertices, unsigned long *indices, int indexCount, char * filename)
{
	ofstream fout;

	fout.open(filename);

	if (fout.fail()) {
		return false;
	}

	fout << "Vertex Count:"<<' '<< indexCount <<'\n';
	fout << "\n";
	fout << "Data:\n\n";
	fout << setiosflags(ios::fixed) << setprecision(4);
	for (int i = 0; i < indexCount; ++i) {
		fout << vertices[indices[i]].position.x << ' ' << vertices[indices[i]].position.y << ' ' << vertices[indices[i]].position.z << ' ';
		fout << vertices[indices[i]].texture.x << ' ' << vertices[indices[i]].texture.y << ' ';
		fout << vertices[indices[i]].normal.x << ' ' << vertices[indices[i]].normal.y << ' ' << vertices[indices[i]].normal.z << '\n';
	}

	fout.close();

	return true;
}
