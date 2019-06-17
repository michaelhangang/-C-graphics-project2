#include "TextureManager.h"
#include<iostream>
#include <sstream>

using namespace std;

bool TextureManager::Create2DTexture(Texture &texture, bool bGenerateMIPMap )
{
	
	glGenTextures(1, &texture.textureId);
	glBindTexture(GL_TEXTURE_2D, texture.textureId); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	
	// The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
	unsigned char* data = stbi_load(texture.fileName.c_str(), &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	
	
	this->TexNameToTexture[ texture.name ] = texture;

	return true;
}




unsigned int TextureManager::getTextureIdFromName( string textureName )
{
	map< std::string, Texture >::iterator itTexture
		= this->TexNameToTexture.find(textureName);


	// Reutrn texture number (from OpenGL genTexture)
	return itTexture->second.textureId;
}


