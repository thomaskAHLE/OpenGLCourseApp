#include<stdio.h>
#include <string.h>
#include<cmath>
#include<vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"
#include "GLWindow.h"


const float toRadians = 3.14169265f/ 180.0f;

GLWindow mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader*> shaderList;


GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;


static const char* fShader = "Shaders/shader.frag";
static const char * vShader = "Shaders/shader.vert";

void CreateObject()
{
	unsigned int indices[] = {
		0, 3 ,1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2

	};
	GLfloat vertices[] = {
		-1.0f,  -1.0f, 0.0f,
		0.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 0.0f,
		0.0f, 1.0f ,0.0f
	};
	meshList.emplace_back(new Mesh());
	meshList[meshList.size() -1]->CreateMesh(vertices, indices, 12, 12);
}


void CreateShaders()
{
	shaderList.emplace_back(new Shader());
	shaderList[shaderList.size() - 1]->CreateFromFiles(vShader, fShader);
}

int main()
{
	mainWindow = GLWindow(800, 600);
	mainWindow.Initialize();
	CreateObject();
	CreateObject();
	CreateShaders();

	Camera camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 0.5f);
	GLuint uniformModel = 0, uniformProjection = 0, uniformView = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.GetBufferWidth()/(GLfloat)mainWindow.GetBufferHeight(),0.1f, 100.0f);
	//loop until window closed
	while (!mainWindow.GetShouldClose())
	{
		GLfloat now = glfwGetTime(); //SDL_GetPerformanceCounter();
		deltaTime = now - lastTime; // (now -lastTime) *1000 / SDL_GetPerformanceFrequency();
		lastTime = now;


		// Get and handle user input events
		glfwPollEvents();

		camera.KeyControl(mainWindow.GetKeys(), deltaTime);
		camera.MouseControl(mainWindow.GetXChange(), mainWindow.GetYChange());

		// clear window
		// rgb values clears out entire screen
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f );
		//clears color and depth buffer
		glClear(GL_COLOR_BUFFER_BIT |GL_DEPTH_BUFFER_BIT) ;

		shaderList[0]->UseShader();
		uniformModel = shaderList[0]->GetModelLocation();
		uniformProjection = shaderList[0]->GetProjectionLocation();
		uniformView = shaderList[0]->GetViewLocation();

		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.CalculateViewMatrix()));

		meshList[0]->RenderMesh();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, -2.5f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();

		//unassign shader
		glUseProgram(0);
		mainWindow.SwapBuffers();
		
	}
	int len = meshList.size();
	for (int i = 0; i < len; i++)
		delete meshList[i];
	len = shaderList.size();
	for (int i = 0; i < len; i++)
	{
		delete shaderList[i];
	}
	return 0;
}