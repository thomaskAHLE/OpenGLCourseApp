#pragma once
#include <glm/glm.hpp>
#include <GL/glew.h>
class Light
{
public:
	Light();

	Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity);

	virtual void UseLight(GLuint ambientIntensityLocation, GLuint ambientColorIntensity);

	

private:
	glm::vec3 color;
	GLfloat ambientIntensity;
};

