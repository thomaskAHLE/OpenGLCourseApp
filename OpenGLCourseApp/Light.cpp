#include "Light.h"



Light::Light()
{
	m_color = glm::vec3(1.0f, 1.0f, 1.0f);
	m_ambientIntensity = 1.0f;
	m_diffuseIntensity = 0.0f;
}

Light::Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat dIntensity)
{
	m_color = glm::vec3(glm::clamp(red,0.0f, 1.0f), glm::clamp(green,0.0f, 1.0f), glm::clamp(blue, 0.0f, 1.0f));
	m_ambientIntensity = aIntensity;
	m_diffuseIntensity = dIntensity;
}






