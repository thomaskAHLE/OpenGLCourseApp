#include "Texture.h"



Texture::Texture()
{
	m_textureID = 0;
	m_width = m_height = m_bitDepth = 0;
	m_fileLocation ="";
}

Texture::Texture(const char * fileLoc)
{
	m_textureID = 0;
	m_width = m_height = m_bitDepth = 0;
	m_fileLocation = fileLoc;
}

bool Texture::loadTexture()
{
	unsigned char * texData = stbi_load(m_fileLocation, &m_width, &m_height, &m_bitDepth, 0);
	if (!texData)
	{
		printf("Faild to find: %s \n", m_fileLocation);
		return false;
	}
	glGenTextures(1, &m_textureID);
	glBindTexture(GL_TEXTURE_2D, m_textureID);


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, texData);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);
	
	//frees imagedata
	stbi_image_free(texData);
	return true;
}

bool Texture::loadTextureAlpha()
{
	unsigned char * texData = stbi_load(m_fileLocation, &m_width, &m_height, &m_bitDepth, 0);
	if (!texData)
	{
		printf("Faild to find: %s \n", m_fileLocation);
		return false;
	}
	glGenTextures(1, &m_textureID);
	glBindTexture(GL_TEXTURE_2D, m_textureID);


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	//frees imagedata
	stbi_image_free(texData);
	return true;
}

void Texture::useTexture()
{
	//GL_TEXTURE0,texture unit minimum texture units 16 usually 32 on modern graphics cards
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_textureID);
}

void Texture::clearTexture()
{
	if (m_textureID)
	{
		glDeleteTextures(1, &m_textureID);
		m_textureID = 0;
		m_width = m_height = m_bitDepth = 0;
		m_fileLocation = "";
	}
}


Texture::~Texture()
{
	clearTexture();
}
