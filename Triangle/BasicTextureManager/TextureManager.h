#ifndef _TextureManager_HG_
#define _TextureManager_HG_

#include <string>
#include <map>
#include "../Texture.h"
#include"../globe.h"
#include "../stb_image.h"
class TextureManager
{
public:
	bool Create2DTexture(Texture& texture, bool bGenerateMIPMap );

	// returns 0 on error
	unsigned int getTextureIdFromName(string textureName);

	



	std::map< std::string, Texture> TexNameToTexture;


};

#endif
