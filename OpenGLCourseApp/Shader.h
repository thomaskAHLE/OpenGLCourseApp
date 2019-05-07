#pragma once

#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"

#include <stdio.h>
#include <string>

#include <iostream>
#include<fstream>

#include<GL/glew.h>



class Shader
{
public:
	Shader();
	//todo CreateFromFile
	void  createFromString(const char* vertexCode, const char* fragmentCode);
	void createFromFiles(const char * vertexFile, const char* fragmentFile);
    GLuint getProjectionLocation()  const { return m_uniformProjection;}
	GLuint getModelLocation() const {return m_uniformModel;}
	GLuint getViewLocation() const {return m_uniformView;}
	GLuint getAmbientIntensityLocation() const {return m_uniformDirectionalLight.uniformAmbientIntensity;}
	GLuint getAmbientColorLocation() const { return m_uniformDirectionalLight.uniformColor;}
	GLuint getDiffuseIntensityLocation() const {return m_uniformDirectionalLight.uniformDiffuseIntensity;}
	GLuint getDirectionLocation() const { return m_uniformDirectionalLight.uniformDirection;}
	GLuint getShininessLocation() const { return m_uniformShininess; }
	GLuint getSpecularIntensityLocation() const { return m_uniformSpecularIntensity; }
	GLuint getEyePositionLocation()  const { return m_uniformEyePosition; }

	void setDirectionalLight(DirectionalLight* dLight);
	void setPointLights(PointLight* pLights, unsigned int lightCount);
	void setSpotLights(SpotLight* sLights, unsigned int lightCount);
	void useShader();
	void clearShader();
	
	~Shader();
private:

	int m_pointLightCount;
	int m_spotLightCount;

	GLuint m_shaderID, m_uniformProjection, m_uniformModel, m_uniformView, m_uniformEyePosition,
		m_uniformSpecularIntensity, m_uniformShininess;

	struct {
		GLuint uniformColor;
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffuseIntensity;
		GLuint uniformDirection;
	} m_uniformDirectionalLight ;

	
	GLuint uniformPointLightCount;
	struct {
		GLuint uniformColor;
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffuseIntensity;
		GLuint uniformPosition;
		GLuint uniformConstant;
		GLuint uniformLinear;
		GLuint uniformQuadratic;
	} m_uniformPointLights[MAX_POINT_LIGHTS];
	
	GLuint m_uniformSpotLightCount;

	struct {
		GLuint uniformColor;
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffuseIntensity;
		GLuint uniformPosition;
		GLuint uniformConstant;
		GLuint uniformLinear;
		GLuint uniformQuadratic;
		GLuint uniformDirection;
		GLuint uniformEdge;
	}m_uniformSpotLights[MAX_SPOT_LIGHTS];


	void compileShader(const char* vertexCode, const char* fragmentCode);
	void addShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);
	std::string readFile(const char * file);
};

