#pragma once
#include<string>

using namespace std;

struct sVert
{
	float x, y, z ;		// added "z"
	
	float nx, ny, nz;
	float u0, v0, u1,v1;
};



class Model {
public:
	string name;
	string fileName;
	unsigned int VAOId;
	unsigned int numberOfIndices;
	unsigned int numberOfVertices;

	bool GenerateSphericalUVs; //uv
	//
	Model();
	Model(string name,string fileName);
	sVert* pVertices;
	unsigned int* pIndices;
};