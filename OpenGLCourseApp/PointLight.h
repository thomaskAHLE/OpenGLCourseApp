#pragma once
#include "Light.h"
class PointLight :
	public Light
{
public:
	PointLight();

	PointLight(GLfloat red, GLfloat green, GLfloat blue,
		GLfloat aIntensity, GLfloat dIntensity,
		GLfloat xPos, GLfloat yPos, GLfloat zPos,
		GLfloat con, GLfloat lin, GLfloat quad);

	void UseLight(GLuint ambientIntensityLocation, GLuint colorLocation,
		GLuint diffuseIntensityLocation, GLuint positionLocation, 
		GLuint constantLocation, GLuint linearLocation, GLuint quadraticLocation);
	~PointLight();

protected:
	glm::vec3 position;
	
	GLfloat constant, linear, quadratic;
};

