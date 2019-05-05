#pragma once

#include <stdio.h>
#include <string>
#include <iostream>
#include<fstream>

#include<GL/glew.h>

#include "CommonValues.h"

#include "DirectionalLight.h"
#include "PointLight.h"

class Shader
{
public:
	Shader();
	//todo CreateFromFile
	void  CreateFromString(const char* vertexCode, const char* fragmentCode);
	void CreateFromFiles(const char * vertexFile, const char* fragmentFile);
    GLuint GetProjectionLocation()  const { return uniformProjection;}
	GLuint GetModelLocation() const {return uniformModel;}
	GLuint GetViewLocation() const {return uniformView;}
	GLuint GetAmbientIntensityLocation() const {return uniformDirectionalLight.uniformAmbientIntensity;}
	GLuint GetAmbientColorLocation() const { return uniformDirectionalLight.uniformColor;}
	GLuint GetDiffuseIntensityLocation() const {return uniformDirectionalLight.uniformDiffuseIntensity;}
	GLuint GetDirectionLocation() const { return uniformDirectionalLight.uniformDirection;}
	GLuint GetShininessLocation() const { return uniformShininess; }
	GLuint GetSpecularIntensityLocation() const { return uniformSpecularIntensity; }
	GLuint GetEyePositionLocation()  const { return uniformEyePosition; }

	void SetDirectionalLight(DirectionalLight* dLight);
	void SetPointLights(PointLight* pLights, unsigned int lightCount);
	void UseShader();
	void ClearShader();
	
	~Shader();
private:

	int pointLightCount;

	GLuint shaderID, uniformProjection, uniformModel, uniformView, uniformEyePosition,
		uniformSpecularIntensity, uniformShininess;

	struct {
		GLuint uniformColor;
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffuseIntensity;
		GLuint uniformDirection;
	} uniformDirectionalLight ;

	
	GLuint uniformPointLightCount;
	struct {
		GLuint uniformColor;
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffuseIntensity;
		GLuint uniformPosition;
		GLuint uniformConstant;
		GLuint uniformLinear;
		GLuint uniformQuadratic;
	} uniformPointLights[MAX_POINT_LIGHTS];



	void compileShader(const char* vertexCode, const char* fragmentCode);
	void addShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);
	std::string readFile(const char * file);
};

