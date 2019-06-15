#pragma once
#include<string>
using namespace std;

struct sVert
{
	float x, y, z ;		// added "z"
	float r, g, b;
	//float nx, ny, nz;
};



class Model {
public:
	string name;
	string fileName;
	unsigned int VAOId;
	unsigned int numberOfIndices;
	unsigned int numberOfVertices;
	//
	Model();
	Model(string name,string fileName);
	sVert* pVertices;
	unsigned int* pIndices;
};