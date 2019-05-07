#include "Shader.h"



Shader::Shader()
{
	m_shaderID = 0;
	m_uniformModel = m_uniformProjection = m_uniformView = 0;

	m_pointLightCount = 0;
	m_spotLightCount = 0;
}

void Shader::createFromString(const char * vertexCode, const char * fragmentCode)
{
	compileShader(vertexCode, fragmentCode);
}

void Shader::createFromFiles(const char * vertexFile, const char * fragmentFile)
{
	std::string vertexStr = readFile(vertexFile);
	std::string fragmentStr = readFile(fragmentFile);
	const char * vertexCode = vertexStr.c_str();
	const char * fragmentCode = fragmentStr.c_str();
	compileShader(vertexCode, fragmentCode);
}




void Shader::setDirectionalLight(DirectionalLight * dLight)
{
	dLight->useLight(m_uniformDirectionalLight.uniformAmbientIntensity, m_uniformDirectionalLight.uniformColor, 
		m_uniformDirectionalLight.uniformDiffuseIntensity, m_uniformDirectionalLight.uniformDirection);
}

void Shader::setPointLights(PointLight * pLights, unsigned int lightCount)
{
	if (lightCount > MAX_POINT_LIGHTS) lightCount = MAX_POINT_LIGHTS;

	glUniform1i(uniformPointLightCount, lightCount);

	for (size_t i = 0; i < lightCount; i++)
	{
		pLights[i].useLight(m_uniformPointLights[i].uniformAmbientIntensity, m_uniformPointLights[i].uniformColor, 
			m_uniformPointLights[i].uniformDiffuseIntensity, m_uniformPointLights[i].uniformPosition,
			m_uniformPointLights[i].uniformConstant, m_uniformPointLights[i].uniformLinear, m_uniformPointLights[i].uniformQuadratic);
	}
}

void Shader::setSpotLights(SpotLight * sLights, unsigned int lightCount)
{
	if (lightCount > MAX_SPOT_LIGHTS) lightCount = MAX_SPOT_LIGHTS;

	glUniform1i(m_uniformSpotLightCount, lightCount);

	for (size_t i = 0; i < lightCount; i++)
	{
		sLights[i].useLight(m_uniformSpotLights[i].uniformAmbientIntensity, m_uniformSpotLights[i].uniformColor,
			m_uniformSpotLights[i].uniformDiffuseIntensity, m_uniformSpotLights[i].uniformPosition, m_uniformSpotLights[i].uniformDirection,
			m_uniformSpotLights[i].uniformConstant, m_uniformSpotLights[i].uniformLinear, m_uniformSpotLights[i].uniformQuadratic,
			m_uniformSpotLights[i].uniformEdge);
	}
}

void Shader::useShader()
{
	if (m_shaderID)
	{
		glUseProgram(m_shaderID);
	}
}

void Shader::clearShader()
{
	if (m_shaderID)
	{
		glDeleteProgram(m_shaderID);
		m_shaderID = 0;
	}
	m_uniformModel = m_uniformProjection = m_uniformView = 0;
}


Shader::~Shader()
{
	clearShader();
}

void Shader::compileShader(const char * vertexCode, const char * fragmentCode)
{
//store shader as id
	m_shaderID = glCreateProgram();
	if (!m_shaderID)
	{
		printf("Error creating shader program \n");
		return;
	}
	addShader(m_shaderID, vertexCode, GL_VERTEX_SHADER);
	addShader(m_shaderID, fragmentCode, GL_FRAGMENT_SHADER);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	//creates exe on graphics card
	glLinkProgram(m_shaderID);
	glGetProgramiv(m_shaderID, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(m_shaderID, sizeof(eLog), NULL, eLog);
		printf("Error linking program %s \n", eLog);
		return;
	}

	//checking if shader is valid in current context of openGL
	glValidateProgram(m_shaderID);
	glGetProgramiv(m_shaderID, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(m_shaderID, sizeof(eLog), NULL, eLog);
		printf("Error validating program %s \n", eLog);
		return;
	}
    
	m_uniformModel = glGetUniformLocation(m_shaderID, "model");
	m_uniformProjection = glGetUniformLocation(m_shaderID, "projection");
	m_uniformView = glGetUniformLocation(m_shaderID, "view");
	
	//for specular 
	m_uniformSpecularIntensity = glGetUniformLocation(m_shaderID, "material.specularIntensity");
	m_uniformShininess = glGetUniformLocation(m_shaderID, "material.shininess");
	m_uniformEyePosition = glGetUniformLocation(m_shaderID, "eyePosition");

	//directionalLight uniforms
	m_uniformDirectionalLight.uniformColor = glGetUniformLocation(m_shaderID, "directionalLight.base.color");
	m_uniformDirectionalLight.uniformAmbientIntensity = glGetUniformLocation(m_shaderID, "directionalLight.base.ambientIntensity");
	m_uniformDirectionalLight.uniformDiffuseIntensity = glGetUniformLocation(m_shaderID, "directionalLight.base.diffuseIntensity");
	m_uniformDirectionalLight.uniformDirection = glGetUniformLocation(m_shaderID, "directionalLight.direction");
	
	
	uniformPointLightCount = glGetUniformLocation(m_shaderID, "pointLightCount");

	//set all uniform locations for pointlights 
	for (size_t i = 0; i < MAX_POINT_LIGHTS; i++)
	{
		char locBuff[100] = { '\0' };

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.color", i);
		m_uniformPointLights[i].uniformColor = glGetUniformLocation(m_shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.ambientIntensity", i);
		m_uniformPointLights[i].uniformAmbientIntensity = glGetUniformLocation(m_shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.diffuseIntensity", i);
		m_uniformPointLights[i].uniformDiffuseIntensity = glGetUniformLocation(m_shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].position", i);
		m_uniformPointLights[i].uniformPosition = glGetUniformLocation(m_shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].constant", i);
		m_uniformPointLights[i].uniformConstant = glGetUniformLocation(m_shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].linear", i);
		m_uniformPointLights[i].uniformLinear = glGetUniformLocation(m_shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].quadratic", i);
		m_uniformPointLights[i].uniformQuadratic = glGetUniformLocation(m_shaderID, locBuff);

	}


	m_uniformSpotLightCount = glGetUniformLocation(m_shaderID, "spotLightCount");

	//set all uniform locations for spotlights 
	for (size_t i = 0; i < MAX_SPOT_LIGHTS; i++)
	{
		char locBuff[100] = { '\0' };

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.base.color", i);
		m_uniformSpotLights[i].uniformColor = glGetUniformLocation(m_shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.base.ambientIntensity", i);
		m_uniformSpotLights[i].uniformAmbientIntensity = glGetUniformLocation(m_shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.base.diffuseIntensity", i);
		m_uniformSpotLights[i].uniformDiffuseIntensity = glGetUniformLocation(m_shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.position", i);
		m_uniformSpotLights[i].uniformPosition = glGetUniformLocation(m_shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.constant", i);
		m_uniformSpotLights[i].uniformConstant = glGetUniformLocation(m_shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.linear", i);
		m_uniformSpotLights[i].uniformLinear = glGetUniformLocation(m_shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.quadratic", i);
		m_uniformSpotLights[i].uniformQuadratic = glGetUniformLocation(m_shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].direction", i);
		m_uniformSpotLights[i].uniformDirection = glGetUniformLocation(m_shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].edge", i);
		m_uniformSpotLights[i].uniformEdge = glGetUniformLocation(m_shaderID, locBuff);

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
