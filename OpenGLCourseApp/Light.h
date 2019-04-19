#pragma once
#include <glm/glm.hpp>
#include <GL/glew.h>
class Light
{
public:
	Light();

	Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity,
		  GLfloat xDir, GLfloat yDir, GLfloat zDir, GLfloat dIntensity
		);

	virtual void UseLight(GLuint ambientIntensityLocation, GLuint colorIntensity,
						  GLuint diffuseIntensityLocation, GLuint directionLocation);

	

private:
	//for ambient lighting 
	glm::vec3 color;
	GLfloat ambientIntensity;

	//for diffuse lighting
	glm::vec3 direction;
	GLfloat diffuseIntensity;
};

