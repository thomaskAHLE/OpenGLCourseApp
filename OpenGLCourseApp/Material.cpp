#include "Material.h"



Material::Material()
{
	m_specularIntensity = m_shininess = 0;
}

Material::Material(GLfloat sIntensity, GLfloat shine)
{
	m_specularIntensity = sIntensity;
	m_shininess = shine;
}


void Material::useMaterial(GLuint specularIntensityLocation, GLuint shininessLocation)
{
	glUniform1f(specularIntensityLocation, m_specularIntensity);
	glUniform1f(shininessLocation, m_shininess);
}

Material::~Material()
{
}
