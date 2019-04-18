#include "Texture.h"



Texture::Texture()
{
	textureID = 0;
	width = height = bitDepth = 0;
	fileLocation ="";
}

Texture::Texture(const char * fileLoc)
{
	textureID = 0;
	width = height = bitDepth = 0;
	fileLocation = fileLoc;
}

void Texture::LoadTexture()
{
	unsigned char * texData = stbi_load(fileLocation, &width, &height, &bitDepth, 0);
	if (!texData)
	{
		printf("Faild to find: %s \n", fileLocation);
		return;
	}
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);
	
	//frees imagedata
	stbi_image_free(texData);
}

void Texture::UseTexture()
{
	//GL_TEXTURE0,texture unit minimum texture units 16 usually 32 on modern graphics cards
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);
}

void Texture::ClearTexture()
{
	if (textureID)
	{
		glDeleteTextures(1, &textureID);
		textureID = 0;
		width = height = bitDepth = 0;
		fileLocation = "";
	}
}


Texture::~Texture()
{
	ClearTexture();
}
