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
	GLuint GetProjectionLocation() const;
	GLuint GetModelLocation() const;
	GLuint GetViewLocation() const;
	GLuint GetAmbientIntensityLocation() const;
	GLuint GetAmbientColorLocation() const;
	GLuint GetDiffuseIntensityLocation() const;
	GLuint GetDirectionLocation() const;
	void UseShader();
	void ClearShader();
	
	~Shader();
private:
	GLuint shaderID, uniformProjection, uniformModel, uniformView;
	GLuint uniformAmbientIntensity, uniformAmbientColor, uniformDiffuseIntensity, uniformDirection ;
	void compileShader(const char* vertexCode, const char* fragmentCode);
	void addShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);
	std::string readFile(const char * file);
};

