#include "DirectionalLight.h"



DirectionalLight::DirectionalLight(): Light()
{
	m_direction = glm::vec3(0.0f, -1.0f, 0.0f);
}

DirectionalLight::DirectionalLight(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity,  GLfloat dIntensity,
								   GLfloat xDir, GLfloat yDir, GLfloat zDir)
								 : Light(red, green , blue, aIntensity, dIntensity)
{
	
	m_color = glm::vec3(glm::clamp(red, 0.0f, 1.0f), glm::clamp(green, 0.0f, 1.0f), glm::clamp(blue, 0.0f, 1.0f));
	m_ambientIntensity = aIntensity;
	m_direction = glm::normalize(glm::vec3(xDir, yDir, zDir));
	m_diffuseIntensity = dIntensity;
}

void DirectionalLight::useLight(GLuint ambientIntensityLocation, GLuint colorLocation,
	GLuint diffuseIntensityLocation, GLuint directionLocation) 
{
	glUniform3f(colorLocation, m_color.x, m_color.y, m_color.z);
	glUniform1f(ambientIntensityLocation, m_ambientIntensity);
	glUniform1f(diffuseIntensityLocation, m_diffuseIntensity);

	glUniform3f(directionLocation, m_direction.x, m_direction.y, m_direction.z);
	
}