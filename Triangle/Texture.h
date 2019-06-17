#pragma once
#include<string>
using namespace std;


class Texture {
public:
	string name;
	string fileName;
	unsigned int textureId;

	Texture();
	Texture(string name, string fileName);
};