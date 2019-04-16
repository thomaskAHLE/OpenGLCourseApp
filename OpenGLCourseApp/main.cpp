#include<stdio.h>
#include <string.h>
#include<cmath>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

//Drawing Triangle to Screen
const GLint WIDTH = 800, HEIGHT = 600;
const float toRadians = 3.14169265f/ 180.0f;
GLuint VAO, VBO, shader, uniformModel;

//direction right = true, left = false
bool direction = true;
float triOffset = 0.0f;
float triMaxOffset = 0.7f;
float triIncrement = 0.0005f;
float currAngle = 0.0f;

bool sizeDirection = true;
float curSize = 0.4f;
float maxSize = 0.8f;
float minSize = 0.1f;
//Vertex Shader

static const char * vShader = "			\n\
	#version 330						\n\
										\n\
	layout(location = 0) in vec3 pos;		\n\
	uniform mat4 model;										\n\
	void main(){									\n\
	gl_Position = model * vec4(  pos, 1.0f); 	\n\
	}";

static const char* fShader = "			\n\
	#version 330						\n\
										\n\
	out vec4 color;	\n\
											\n\
	void main(){									\n\
	color = vec4( 1.0f, 0.0f , 0.0f , 1.0f); 	\n\
	}";


void CreateTriangle()
{
	GLfloat vertices[] = {
		-1.0f,  -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		0.0f, 1.0f ,0.0f
	};
	//num of arrays, where to store id 
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//static draw values not changing after put in buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices),vertices, GL_STATIC_DRAW);


	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);


	//unbinding buffer and array
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType)
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
		printf("Error compiling the %d shader  %s \n",shaderType, eLog);
		return;
	}

	glAttachShader(theProgram, theShader);


	return;
}

void CompileShaders()
{
	//store shader as id
	shader = glCreateProgram();
	if (!shader)
	{
		printf("Error creating shader program \n");
		return;
	}
	AddShader(shader, vShader, GL_VERTEX_SHADER);
	AddShader(shader, fShader, GL_FRAGMENT_SHADER);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	//creates exe on graphics card
	glLinkProgram(shader);
	glGetProgramiv(shader, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("Error linking program %s \n", eLog);
		return;
	}

	//checking if shader is valid in current context of openGL
	glValidateProgram(shader);
	glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("Error validating program %s \n", eLog);
		return;
	}
    
	uniformModel = glGetUniformLocation(shader, "model");
}



int main()
{
	//initialize glfw
	if (!glfwInit())
	{
		printf("GLFW init failed");
		glfwTerminate();
		return 1;
	}

	// setup glfw window properties
	//opengl version
	//using Opengl version 3.x
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	//using opengl version x.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//causes depracated functions to throw errors
	glfwWindowHint(GLFW_OPENGL_PROFILE,  GLFW_OPENGL_CORE_PROFILE);
	//allowing forward compatibility
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow * mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Test Window", NULL, NULL);
	if (!mainWindow)
	{
		printf("GLFW window creation failed!");
		glfwTerminate();
		return 1;
	}


	//Get buffer size information
	int bufferWidth, bufferHeight;
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	//set context for Glew to use
	glfwMakeContextCurrent(mainWindow);

	//allow modern extension features
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("GLeW init failed");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	//setup viewport size

	glViewport(0, 0, bufferWidth, bufferHeight);

	CreateTriangle();
	CompileShaders();

	//loop until window closed
	while (!glfwWindowShouldClose(mainWindow))
	{
		// Get and handle user input events
		glfwPollEvents();

		if (direction)
		{
			triOffset += triIncrement;
		}
		else 
		{
			triOffset -= triIncrement;
		}
	
		if (abs(triOffset) >= triMaxOffset)
		{
			direction = !direction;
		}

		if (sizeDirection)
		{
			curSize += 0.0001f;
		}
		else
		{
			curSize -= 0.0001f;
		}
		if (curSize >= maxSize || curSize <= minSize)
		{
			sizeDirection = !sizeDirection;
		}
		currAngle += 0.01f;
		if (currAngle >= 360.0f)
		{
			currAngle -= 360.0f;
		}

		// clear window
		// rgb values glears out entire screen
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f );
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(shader);

		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(triOffset, 0.0f, 0.0f));
		model = glm::rotate(model, currAngle * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(curSize, curSize, 1.0f));


		//value ptr to turn into version of data that will work for shader
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES,0, 3);
		glBindVertexArray(0);
		//unassign shader
		glUseProgram(0);
		glfwSwapBuffers(mainWindow);
		
	}

	return 0;
}