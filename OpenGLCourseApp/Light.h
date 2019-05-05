#pragma once
#include <glm/glm.hpp>
#include <GL/glew.h>
class Light
{
public:
	Light();

	Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat dIntensity);


	

protected:
	//for ambient lighting 
	glm::vec3 color;
	GLfloat ambientIntensity;

	//for diffuse lighting
	GLfloat diffuseIntensity;
};

