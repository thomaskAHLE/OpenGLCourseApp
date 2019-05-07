#include "Shader.h"



Shader::Shader()
{
	shaderID = 0;
	uniformModel = uniformProjection = uniformView = 0;

	pointLightCount = 0;
	spotLightCount = 0;
}

void Shader::CreateFromString(const char * vertexCode, const char * fragmentCode)
{
	compileShader(vertexCode, fragmentCode);
}

void Shader::CreateFromFiles(const char * vertexFile, const char * fragmentFile)
{
	std::string vertexStr = readFile(vertexFile);
	std::string fragmentStr = readFile(fragmentFile);
	const char * vertexCode = vertexStr.c_str();
	const char * fragmentCode = fragmentStr.c_str();
	compileShader(vertexCode, fragmentCode);
}




void Shader::SetDirectionalLight(DirectionalLight * dLight)
{
	dLight->UseLight(uniformDirectionalLight.uniformAmbientIntensity, uniformDirectionalLight.uniformColor, 
		uniformDirectionalLight.uniformDiffuseIntensity, uniformDirectionalLight.uniformDirection);
}

void Shader::SetPointLights(PointLight * pLights, unsigned int lightCount)
{
	if (lightCount > MAX_POINT_LIGHTS) lightCount = MAX_POINT_LIGHTS;

	glUniform1i(uniformPointLightCount, lightCount);

	for (size_t i = 0; i < lightCount; i++)
	{
		pLights[i].UseLight(uniformPointLights[i].uniformAmbientIntensity, uniformPointLights[i].uniformColor, 
			uniformPointLights[i].uniformDiffuseIntensity, uniformPointLights[i].uniformPosition,
			uniformPointLights[i].uniformConstant, uniformPointLights[i].uniformLinear, uniformPointLights[i].uniformQuadratic);
	}
}

void Shader::SetSpotLights(SpotLight * sLights, unsigned int lightCount)
{
	if (lightCount > MAX_SPOT_LIGHTS) lightCount = MAX_SPOT_LIGHTS;

	glUniform1i(uniformSpotLightCount, lightCount);

	for (size_t i = 0; i < lightCount; i++)
	{
		sLights[i].UseLight(uniformSpotLights[i].uniformAmbientIntensity, uniformSpotLights[i].uniformColor,
			uniformSpotLights[i].uniformDiffuseIntensity, uniformSpotLights[i].uniformPosition, uniformSpotLights[i].uniformDirection,
			uniformSpotLights[i].uniformConstant, uniformSpotLights[i].uniformLinear, uniformSpotLights[i].uniformQuadratic,
			uniformSpotLights[i].uniformEdge);
	}
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
	uniformModel = uniformProjection = uniformView = 0;
}


Shader::~Shader()
{
	ClearShader();
}

void Shader::compileShader(const char * vertexCode, const char * fragmentCode)
{
//store shader as id
	shaderID = glCreateProgram();
	if (!shaderID)
	{
		printf("Error creating shader program \n");
		return;
	}
	addShader(shaderID, vertexCode, GL_VERTEX_SHADER);
	addShader(shaderID, fragmentCode, GL_FRAGMENT_SHADER);

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
	uniformView = glGetUniformLocation(shaderID, "view");
	
	//for specular 
	uniformSpecularIntensity = glGetUniformLocation(shaderID, "material.specularIntensity");
	uniformShininess = glGetUniformLocation(shaderID, "material.shininess");
	uniformEyePosition = glGetUniformLocation(shaderID, "eyePosition");

	//directionalLight uniforms
	uniformDirectionalLight.uniformColor = glGetUniformLocation(shaderID, "directionalLight.base.color");
	uniformDirectionalLight.uniformAmbientIntensity = glGetUniformLocation(shaderID, "directionalLight.base.ambientIntensity");
	uniformDirectionalLight.uniformDiffuseIntensity = glGetUniformLocation(shaderID, "directionalLight.base.diffuseIntensity");
	uniformDirectionalLight.uniformDirection = glGetUniformLocation(shaderID, "directionalLight.direction");
	
	
	uniformPointLightCount = glGetUniformLocation(shaderID, "pointLightCount");

	//set all uniform locations for pointlights 
	for (size_t i = 0; i < MAX_POINT_LIGHTS; i++)
	{
		char locBuff[100] = { '\0' };

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.color", i);
		uniformPointLights[i].uniformColor = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.ambientIntensity", i);
		uniformPointLights[i].uniformAmbientIntensity = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.diffuseIntensity", i);
		uniformPointLights[i].uniformDiffuseIntensity = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].position", i);
		uniformPointLights[i].uniformPosition = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].constant", i);
		uniformPointLights[i].uniformConstant = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].linear", i);
		uniformPointLights[i].uniformLinear = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].quadratic", i);
		uniformPointLights[i].uniformQuadratic = glGetUniformLocation(shaderID, locBuff);

	}


	uniformSpotLightCount = glGetUniformLocation(shaderID, "spotLightCount");

	//set all uniform locations for spotlights 
	for (size_t i = 0; i < MAX_SPOT_LIGHTS; i++)
	{
		char locBuff[100] = { '\0' };

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.base.color", i);
		uniformSpotLights[i].uniformColor = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.base.ambientIntensity", i);
		uniformSpotLights[i].uniformAmbientIntensity = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.base.diffuseIntensity", i);
		uniformSpotLights[i].uniformDiffuseIntensity = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.position", i);
		uniformSpotLights[i].uniformPosition = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.constant", i);
		uniformSpotLights[i].uniformConstant = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.linear", i);
		uniformSpotLights[i].uniformLinear = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.quadratic", i);
		uniformSpotLights[i].uniformQuadratic = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].direction", i);
		uniformSpotLights[i].uniformDirection = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].edge", i);
		uniformSpotLights[i].uniformEdge = glGetUniformLocation(shaderID, locBuff);

	}

}

void Shader::addShader(GLuint theProgram, const char * shaderCode, GLenum shaderType)
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

std::string Shader::readFile(const char * file)
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
