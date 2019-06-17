#pragma once
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include<string>
#include"Model.h"
#include"Texture.h"
using namespace std;

class Mesh {
public:	
	string modelType;
	string textureName;
	string name;
	glm::vec3 pos;			
	glm::vec4 colour;		
	glm::vec3 orientation;	 
	float scale;		
	bool isWireframe;	
	Mesh();
	Mesh(string name,Model model,string textureName="");
};