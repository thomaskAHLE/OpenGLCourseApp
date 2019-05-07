#pragma once


#include<GL\glew.h>

#include"stb_image.h"

class Texture
{
public:
	Texture();
	Texture(const char* fileLoc);
	bool loadTexture();
	bool loadTextureAlpha();
	void useTexture();
	void clearTexture();
	~Texture();
private:
	GLuint m_textureID;
	int m_width, m_height, m_bitDepth;
	const char* m_fileLocation;
};

