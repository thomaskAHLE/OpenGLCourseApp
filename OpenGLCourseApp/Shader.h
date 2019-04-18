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
	void UseShader();
	void ClearShader();
	
	~Shader();
private:
	GLuint shaderID, uniformProjection, uniformModel;
	void CompileShader(const char* vertexCode, const char* fragmentCode);
	void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);
	std::string ReadFile(const char * file);
};

