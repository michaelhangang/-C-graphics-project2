#include"VAOManager.h"
#include<fstream>
#include<iostream>
#include"globe.h"
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <vector>
#include"Model.h"
void VAOManager::loadModelToVAO(Model &modelName) {


	//load ply file 
	ifstream iff(modelName.fileName);
	if (iff.is_open()) {
		string temp;
		while (iff >> temp) {
			if (temp == "vertex")
				break;
		}
		//read the number of vertices
		iff >> modelName.numberOfVertices;

		while (iff >> temp) {
			if (temp == "face")
				break;
		}
		//read the number of faces
		unsigned int faceNum;
		iff >> faceNum;
		modelName.numberOfIndices = 3 * faceNum;
	
		while (iff >> temp) {
			if (temp == "end_header")
				break;
		}
		
		modelName.pVertices = new sVert[modelName.numberOfVertices];

		//load  vertex attributes
		for (unsigned int i = 0; i < modelName.numberOfVertices; i++) {

			iff >> modelName.pVertices[i].x>> modelName.pVertices[i].y
				>> modelName.pVertices[i].z>> modelName.pVertices[i].nx

				>>modelName.pVertices[i].ny>> modelName.pVertices[i].nz;
			modelName.pVertices[i].u0 = 0.0f;
			modelName.pVertices[i].u1 = 0.0f;
			modelName.pVertices[i].v0 = 0.0f;
			modelName.pVertices[i].v1 = 0.0f;
		}
		
		unsigned int tem = 0;
		modelName.pIndices = new unsigned int[modelName.numberOfIndices];
		//load index
		for (unsigned int i = 0, j = 0; i < faceNum; i++, j += 3) {

			iff >> tem;
			iff >> modelName.pIndices[j + 0];
			iff >> modelName.pIndices[j + 1];
			iff >> modelName.pIndices[j + 2];
		}
	}
	else {
		cout << "open modle file failed";
	}
	//create texture coordinator
	if (modelName.GenerateSphericalUVs)
	{
		this->m_GenTextureCoordsSpherical(modelName, POSITIVE_X,
			POSITIVE_Y, false, 1.0f, false);
	}

	//create vertex buffer object 
	unsigned int  VBO, EBO;
	glGenVertexArrays(1, &modelName.VAOId);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	//bind vertex Array Object first 
	glBindVertexArray(modelName.VAOId);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(sVert) * modelName.numberOfVertices,
		(GLvoid*)modelName.pVertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * modelName.numberOfIndices, (GLvoid*)modelName.pIndices, GL_STATIC_DRAW);
	//setup vertex attributes pointer
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(sVert), (void*)0);
	//enable vertext attribte , it is disable by default
	glEnableVertexAttribArray(0);

	// 
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(sVert), (void*)(offsetof(sVert, nx)));
	glEnableVertexAttribArray(1);
	
	//text coordinator
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(sVert), (void*)(offsetof(sVert,u0)));
	glEnableVertexAttribArray(2);

	//unbind VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//unbind VAO
	glBindVertexArray(0);

	this->dateVAO[modelName.name] = modelName;

}

// Generates texture coordinates based on a shperical projection from the 
// origin (0,0,0) location of the model. 
// NOTE: Will overwrite any existing texture coordinates 
// Inspired by: http://www.mvps.org/directx/articles/spheremap.htm
// 
//	basedOnNormals = true --> uses normals for calculation, so good for reflections
//	basedOnNormals = false --> uses model coordinates, so more 'shrink wrapped'
//  fast -> approximate or not (not uses sin() )
//  scale -> normally 1.0, but can be the limits of the texture coord
//
// The 'bias' is what coordinates are used for the generation. 
// For example, if POSITIVE_X is for u and POSITIVE_Y is for v, then:
//	u coords: based on -x to +x
//  v coords: based on -y to +y
//

void VAOManager::m_GenTextureCoordsSpherical(Model& drawInfo, enumTEXCOORDBIAS uBias, enumTEXCOORDBIAS vBias, bool basedOnNormals, float scale, bool fast)
{
	// determine the 'remaining' coordinate...
	bool xUsed = false;
	bool yUsed = false;
	bool zUsed = false;
	if (uBias == POSITIVE_X || vBias == POSITIVE_X)	xUsed = true;
	if (uBias == POSITIVE_Y || vBias == POSITIVE_Y)	yUsed = true;
	if (uBias == POSITIVE_Z || vBias == POSITIVE_Z)	yUsed = true;


	for (unsigned int index = 0; index != drawInfo.numberOfVertices; index++)
	{
		glm::vec3 xyz;
		if (basedOnNormals)
		{
			if (uBias == POSITIVE_X)		xyz.x = drawInfo.pVertices[index].nx;
			else if (uBias == POSITIVE_Y)	xyz.x = drawInfo.pVertices[index].ny;
			else if (uBias == POSITIVE_Z)	xyz.x = drawInfo.pVertices[index].nz;

			if (vBias == POSITIVE_X)		xyz.y = drawInfo.pVertices[index].nx;
			else if (vBias == POSITIVE_Y)	xyz.y = drawInfo.pVertices[index].ny;
			else if (vBias == POSITIVE_Z)	xyz.y = drawInfo.pVertices[index].nz;

			// Fill in the remaining coordinate...
			if (!xUsed)	xyz.z = drawInfo.pVertices[index].nx;
			if (!yUsed)	xyz.z = drawInfo.pVertices[index].ny;
			if (!zUsed)	xyz.z = drawInfo.pVertices[index].nz;
		}
		else
		{
			if (uBias == POSITIVE_X)		xyz.x = drawInfo.pVertices[index].x;
			else if (uBias == POSITIVE_Y)	xyz.x = drawInfo.pVertices[index].y;
			else if (uBias == POSITIVE_Z)	xyz.x = drawInfo.pVertices[index].z;

			if (vBias == POSITIVE_X)		xyz.y = drawInfo.pVertices[index].x;
			else if (vBias == POSITIVE_Y)	xyz.y = drawInfo.pVertices[index].y;
			else if (vBias == POSITIVE_Z)	xyz.y = drawInfo.pVertices[index].z;

			// Fill in the remaining coordinate...
			if (!xUsed)	xyz.z = drawInfo.pVertices[index].x;
			if (!yUsed)	xyz.z = drawInfo.pVertices[index].y;
			if (!zUsed)	xyz.z = drawInfo.pVertices[index].z;
		}

		glm::normalize(xyz);

		if (fast)
		{
			drawInfo.pVertices[index].u0 = ((xyz.x / 2.0f) + 0.5f) * scale;
			drawInfo.pVertices[index].v0 = ((xyz.y / 2.0f) + 0.5f) * scale;
		}
		else
		{
			const float PI = 3.141457f;
			drawInfo.pVertices[index].u0 = ((asin(xyz.x) / PI) + 0.5f) * scale;
			drawInfo.pVertices[index].v0 = ((asin(xyz.y) / PI) + 0.5f) * scale;
		}
		drawInfo.pVertices[index].u1 = drawInfo.pVertices[index].u0;
		drawInfo.pVertices[index].v1 = drawInfo.pVertices[index].v0;
	}
	return;
}
