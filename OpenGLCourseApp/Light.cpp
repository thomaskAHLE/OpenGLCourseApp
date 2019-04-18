#include "Light.h"



Light::Light()
{
	color = glm::vec3(1.0f, 1.0f, 1.0f);
	ambientIntensity = 1.0f;
}

Light::Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity)
{
	color = glm::vec3(glm::clamp(red,0.0f, 1.0f), glm::clamp(green,0.0f, 1.0f), glm::clamp(blue, 0.0f, 1.0f));
	ambientIntensity = aIntensity;
}

void Light::UseLight(GLuint ambientIntensityLocation, GLuint ambientColorIntensity)
{
	glUniform3f(ambientColorIntensity, color.x, color.y, color.z);
	glUniform1f(ambientIntensityLocation, ambientIntensity);
}



