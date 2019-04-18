#include "Shader.h"



Shader::Shader()
{
	shaderID = 0;
	uniformModel = uniformProjection = 0;
}

void Shader::CreateFromString(const char * vertexCode, const char * fragmentCode)
{
	CompileShader(vertexCode, fragmentCode);
}

void Shader::CreateFromFiles(const char * vertexFile, const char * fragmentFile)
{
	std::string vertexStr = ReadFile(vertexFile);
	std::string fragmentStr = ReadFile(fragmentFile);
	const char * vertexCode = vertexStr.c_str();
	const char * fragmentCode = fragmentStr.c_str();
	CompileShader(vertexCode, fragmentCode);
}

GLuint Shader::GetProjectionLocation() const
{
	return uniformProjection;
}

GLuint Shader::GetModelLocation() const
{
	return uniformModel;
}

void Shader::UseShader()
{
	if (shaderID)
	{
		glUseProgram(shaderID);
	}
}

void Shader::ClearShader()
{
	if (shaderID)
	{
		glDeleteProgram(shaderID);
		shaderID = 0;
	}
	uniformModel = uniformProjection = 0;
}


Shader::~Shader()
{
	ClearShader();
}

void Shader::CompileShader(const char * vertexCode, const char * fragmentCode)
{
//store shader as id
	shaderID = glCreateProgram();
	if (!shaderID)
	{
		printf("Error creating shader program \n");
		return;
	}
	AddShader(shaderID, vertexCode, GL_VERTEX_SHADER);
	AddShader(shaderID, fragmentCode, GL_FRAGMENT_SHADER);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	//creates exe on graphics card
	glLinkProgram(shaderID);
	glGetProgramiv(shaderID, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
		printf("Error linking program %s \n", eLog);
		return;
	}

	//checking if shader is valid in current context of openGL
	glValidateProgram(shaderID);
	glGetProgramiv(shaderID, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
		printf("Error validating program %s \n", eLog);
		return;
	}
    
	uniformModel = glGetUniformLocation(shaderID, "model");
	uniformProjection = glGetUniformLocation(shaderID, "projection");

}

void Shader::AddShader(GLuint theProgram, const char * shaderCode, GLenum shaderType)
{
	GLuint theShader = glCreateShader(shaderType);

	const GLchar* theCode[1];
	theCode[0] = shaderCode;

	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);

	glShaderSource(theShader, 1, theCode, codeLength);
	glCompileShader(theShader);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(theShader, sizeof(eLog), NULL, eLog);
		printf("Error compiling the %d shader  %s \n", shaderType, eLog);
		return;
	}

	glAttachShader(theProgram, theShader);


}

std::string Shader::ReadFile(const char * file)
{
	std::string content;
	std::ifstream fileStream(file, std::ios::in);
	if (!fileStream.is_open())
	{
		printf("Failed to read %s. File doesnt exist \n", file);
		return "";
	}
	std::string line = "";
	while (!fileStream.eof())
	{
		std::getline(fileStream, line);
		content.append(line + "\n");
	}
	fileStream.close();
	return content;
}
