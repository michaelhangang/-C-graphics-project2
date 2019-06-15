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
				>> modelName.pVertices[i].z>> modelName.pVertices[i].r
				>>modelName.pVertices[i].g>> modelName.pVertices[i].b;
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	//enable vertext attribte , it is disable by default
	glEnableVertexAttribArray(0);

	// 
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//unbind VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//unbind VAO
	glBindVertexArray(0);

	this->dateVAO[modelName.name] = modelName;

}