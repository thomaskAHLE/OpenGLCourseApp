#include "DirectionalLight.h"



DirectionalLight::DirectionalLight(): Light()
{
	direction = glm::vec3(0.0f, -1.0f, 0.0f);
}

DirectionalLight::DirectionalLight(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity,  GLfloat dIntensity,
								   GLfloat xDir, GLfloat yDir, GLfloat zDir)
								 : Light(red, green , blue, aIntensity, dIntensity)
{
	
	color = glm::vec3(glm::clamp(red, 0.0f, 1.0f), glm::clamp(green, 0.0f, 1.0f), glm::clamp(blue, 0.0f, 1.0f));
	ambientIntensity = aIntensity;
	direction = glm::normalize(glm::vec3(xDir, yDir, zDir));
	diffuseIntensity = dIntensity;
}

void DirectionalLight::UseLight(GLuint ambientIntensityLocation, GLuint colorLocation,
	GLuint diffuseIntensityLocation, GLuint directionLocation) 
{
	glUniform3f(colorLocation, color.x, color.y, color.z);
	glUniform1f(ambientIntensityLocation, ambientIntensity);
	glUniform1f(diffuseIntensityLocation, diffuseIntensity);

	glUniform3f(directionLocation, direction.x, direction.y, direction.z);
	
}