#include "Mesh.h"

Mesh::Mesh(){}

Mesh::Mesh(string name, Model model, string textureName1, string textureName2):name(name),modelType(model.name)
{
	this->pos = glm::vec3(0.0f, 0.0f, 0.0f);				// 0,0,0 (origin)
	this->colour = glm::vec4(1.0f, 1.0f, 1.0f,1.0f);		// 0,0,0,1 (black)
	this->orientation = glm::vec3(0.0f, 0.0f, 0.0f);
	this->scale = 1.0f;
	this->isWireframe = false;
	if(textureName1!="")
	this->textureName.push_back(textureName1);
	if(textureName2!="")
	this->textureName.push_back(textureName2);
}