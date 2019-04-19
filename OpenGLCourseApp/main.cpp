#define STB_IMAGE_IMPLEMENTATION

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
#include "Texture.h"
#include "Light.h"


const float toRadians = 3.14169265f/ 180.0f;

GLWindow mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader*> shaderList;

Texture brickTexture;
Texture dirtTexture;

Light mainLight;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;


static const char* fShader = "Shaders/shader.frag";
static const char * vShader = "Shaders/shader.vert";

void CalcAverageNormals(unsigned int * indices, unsigned int indiceCount,
						GLfloat * vertices, unsigned int verticesCount, unsigned int vLength,
						unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);
		
		in0 += normalOffset;
		in1 += normalOffset;
		in2 += normalOffset;

		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;

		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;

		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticesCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 1]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}

void CreateObject()
{
	unsigned int indices[] = {
		0, 3 ,1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2

	};
	GLfloat vertices[] = {
		// x	  y	     z		u	   v		normals
		-1.0f,  -1.0f, 0.0f,  0.0f,	 0.0f,   0.0f, 0.0f, 0.0f,	//bottom left
		0.0f, -1.0f, 1.0f,    0.5f , 0.0f,	 0.0f, 0.0f, 0.0f,	//back side
		1.0f, -1.0f, 0.0f,   1.0f,   0.0f,   0.0f, 0.0f, 0.0f,	// flat side
		0.0f, 1.0f ,0.0f,	 0.5f,   1.0f,	 0.0f, 0.0f, 0.0f   // top of triangle
	};
	CalcAverageNormals(indices, 12, vertices, 32, 8, 5);
	meshList.emplace_back(new Mesh());
	meshList[meshList.size() -1]->CreateMesh(vertices, indices, 32, 12);
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

	//creating textures
	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTexture();

	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTexture();

	mainLight = Light(1.0f, 1.0f, 1.0f, 0.0f, 
					 2.0f, -1.0f, -2.0f, 1.0f);

	GLuint uniformModel = 0, uniformProjection = 0, uniformView = 0,
		uniformAmbientColor = 0, uniformAmbientIntensity = 0, uniformDirection = 0, uniformDiffuseIntensity = 0;
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
		uniformAmbientColor = shaderList[0]->GetAmbientColorLocation();
		uniformAmbientIntensity = shaderList[0]->GetAmbientIntensityLocation();
		uniformDiffuseIntensity = shaderList[0]->GetDiffuseIntensityLocation();
		uniformDirection = shaderList[0]->GetDirectionLocation();

		mainLight.UseLight(uniformAmbientIntensity, uniformAmbientColor, uniformDiffuseIntensity, uniformDirection);

		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.CalculateViewMatrix()));

		brickTexture.UseTexture();

		meshList[0]->RenderMesh();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, -2.5f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		dirtTexture.UseTexture();

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