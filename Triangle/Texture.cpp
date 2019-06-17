#include "Texture.h"


Texture::Texture(){}

Texture::Texture(string name, string fileName) :name(name), fileName("Textures/"+fileName) {
	this->textureId = 0;
}