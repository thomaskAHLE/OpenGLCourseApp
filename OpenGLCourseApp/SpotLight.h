#pragma once
#include "PointLight.h"
class SpotLight :
	public PointLight
{
public:
	SpotLight();
	SpotLight(GLfloat red, GLfloat green, GLfloat blue,
		GLfloat aIntensity, GLfloat dIntensity,
		GLfloat xPos, GLfloat yPos, GLfloat zPos,
		GLfloat xDir, GLfloat yDir, GLfloat zDir,
		GLfloat con, GLfloat lin, GLfloat quad,
		GLfloat edg
		);

	void UseLight(GLuint ambientIntensityLocation, GLuint colorLocation,
		GLuint diffuseIntensityLocation, GLuint positionLocation, GLuint directionLocation,
		GLuint constantLocation, GLuint linearLocation, GLuint quadraticLocation,
		GLuint edgeLocation
		);

	void SetFlash(glm::vec3 pos, glm::vec3 dir);
private: 
	glm::vec3 direction;

	GLfloat edge, procEdge;

};

