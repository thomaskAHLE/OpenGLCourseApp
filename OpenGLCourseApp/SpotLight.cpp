#include "SpotLight.h"



SpotLight::SpotLight(): PointLight()
{
	m_direction = glm::vec3(0.0f, -1.0f, 0.0f);
	m_edge = 0.0f;
	m_procEdge = cosf(glm::radians(m_edge));
}

SpotLight::SpotLight(GLfloat red, GLfloat green, GLfloat blue,
					 GLfloat aIntensity, GLfloat dIntensity,
					 GLfloat xPos, GLfloat yPos, GLfloat zPos, 
					 GLfloat xDir, GLfloat yDir, GLfloat zDir, 
					 GLfloat con, GLfloat lin, GLfloat quad,
					 GLfloat edg):PointLight(red, green,blue, aIntensity, dIntensity, xPos,yPos,zPos, con, lin, quad)
{
	m_direction = glm::normalize(glm::vec3(xDir, yDir, zDir));
	m_edge = edg;
	m_procEdge = cosf(glm::radians(m_edge));
}

void SpotLight::useLight(GLuint ambientIntensityLocation, GLuint colorLocation, GLuint diffuseIntensityLocation, 
						 GLuint positionLocation, GLuint directionLocation, 
						 GLuint constantLocation, GLuint linearLocation, GLuint quadraticLocation,
						 GLuint edgeLocation)
{
	glUniform3f(colorLocation, m_color.x, m_color.y, m_color.z);
	glUniform1f(ambientIntensityLocation, m_ambientIntensity);
	glUniform1f(diffuseIntensityLocation, m_diffuseIntensity);

	glUniform3f(positionLocation, m_position.x, m_position.y, m_position.z);
	glUniform1f(constantLocation, m_constant);
	glUniform1f(linearLocation, m_linear);
	glUniform1f(quadraticLocation, m_quadratic);

	glUniform3f(directionLocation, m_direction.x, m_direction.y, m_direction.z);
	glUniform1f(edgeLocation, m_procEdge);
}

void SpotLight::setFlash(glm::vec3 pos, glm::vec3 dir)
{
	m_position = pos;
	m_direction = dir;
}
