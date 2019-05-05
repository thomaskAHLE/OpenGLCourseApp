#pragma once

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
	void  CreateFromString(const char* vertexCode, const char* fragmentCode);
	void CreateFromFiles(const char * vertexFile, const char* fragmentFile);
    GLuint GetProjectionLocation()  const { return uniformProjection;}
	GLuint GetModelLocation() const {return uniformModel;}
	GLuint GetViewLocation() const {return uniformView;}
	GLuint GetAmbientIntensityLocation() const {return uniformAmbientIntensity;}
	GLuint GetAmbientColorLocation() const { return uniformAmbientColor;}
	GLuint GetDiffuseIntensityLocation() const {return uniformDiffuseIntensity;}
	GLuint GetDirectionLocation() const { return uniformDirection;}
	GLuint GetShininessLocation() const { return uniformShininess; }
	GLuint GetSpecularIntensityLocation() const { return uniformSpecularIntensity; }
	GLuint GetEyePositionLocation()  const { return uniformEyePosition; }

	void UseShader();
	void ClearShader();
	
	~Shader();
private:
	GLuint shaderID, uniformProjection, uniformModel, uniformView, uniformEyePosition,
		uniformAmbientIntensity, uniformAmbientColor, uniformDiffuseIntensity, uniformDirection,
		uniformSpecularIntensity, uniformShininess
		;
	void compileShader(const char* vertexCode, const char* fragmentCode);
	void addShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);
	std::string readFile(const char * file);
};

