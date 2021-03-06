#define STB_IMAGE_IMPLEMENTATION

#include "CommonValues.h"

#include "GLWindow.h"
#include "Camera.h"
#include "Shader.h"

#include "Mesh.h"
#include "Texture.h"
#include "Material.h"
#include "Model.h"

#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"

#include<stdio.h>
#include <string.h>
#include<cmath>
#include<vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>


const float toRadians = 3.14169265f/ 180.0f;

GLWindow mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader*> shaderList;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;

Material shinyMaterial;
Material dullMaterial;

Model xWing;

DirectionalLight mainLight;
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;


static const char* fShader = "Shaders/shader.frag";
static const char * vShader = "Shaders/shader.vert";

void CalcAverageNormals(unsigned int * indices, unsigned int indiceCount,
						GLfloat * vertices, unsigned int verticesCount, 
						unsigned int vLength, unsigned int normalOffset)
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
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
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
	//   x	     y	   z	   u	   v		 normals
	   -1.0f, -1.0f, -0.6f,   0.0f,	  0.0f,   0.0f, 0.0f, 0.0f,	//bottom left
		0.0f, -1.0f,  1.0f,   0.5f,   0.0f,	 0.0f, 0.0f, 0.0f,	//back side
		1.0f, -1.0f, -0.6f,   1.0f,   0.0f,   0.0f, 0.0f, 0.0f,	// flat side
		0.0f,  1.0f,  0.0f,	  0.5f,   1.0f,	 0.0f, 0.0f, 0.0f   // top of triangle
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};
	GLfloat floorVertices[] = {
		-10.0f ,0.0f, -10.0f,   0.0f, 0.0f,      0.0f, -1.0f, 0.0f, 
		 10.0f, 0.0f, -10.0f,   10.0f, 0.0f,     0.0f, -1.0f, 0.0f,
		 -10.0f, 0.0f, 10.0f,   0.0f, 10.0f,     0.0f, -1.0f, 0.0f,
		 10.0f, 0.0f, 10.0f,    10.0f, 10.0f,    0.0f, -1.0f, 0.0f
	};
	CalcAverageNormals(indices, 12, vertices, 32, 8, 5);
	meshList.emplace_back(new Mesh());
	meshList[meshList.size() -1]->createMesh(vertices, indices, 32, 12);

	meshList.emplace_back(new Mesh());
	meshList[meshList.size() - 1]->createMesh(vertices, indices, 32, 12);

	meshList.emplace_back(new Mesh());
	meshList[meshList.size() - 1]->createMesh(floorVertices, floorIndices, 32, 6);

}


void CreateShaders()
{
	shaderList.emplace_back(new Shader());
	shaderList[shaderList.size() - 1]->createFromFiles(vShader, fShader);
}

int main()
{
	mainWindow = GLWindow(800, 600);
	mainWindow.initialize();
	CreateObject();
	CreateShaders();

	Camera camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 5.0f, 0.5f);

	//creating textures
	brickTexture = Texture("Textures/brick.png");
	brickTexture.loadTextureAlpha();

	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.loadTextureAlpha();

	plainTexture = Texture("Textures/plain.png");
	plainTexture.loadTextureAlpha();

	shinyMaterial = Material(4.0f, 256.0f);

	dullMaterial = Material(0.3f, 4.0f);

	xWing = Model();
	xWing.loadModel("Models/x-wing.obj");

	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
								 0.7f, 0.4f,
								 0.0f, 0.0f, -1.0f);

	unsigned int pointLightCount = 0;
	pointLights[0] = PointLight(0.0f, 0.0f, 1.0f,
								0.0f, 0.1f,
							    0.0f, 0.0f, 0.0f,
								0.3f, 0.2f, 0.1f);
	pointLightCount++;

	pointLights[1] = PointLight(0.0f, 1.0f, 0.0f,
							    0.0f, 0.1f,
							    -4.0f, 2.0f, 0.0f,
							    0.3f, 0.1f, 0.1f);
	pointLightCount++;
	
	unsigned int spotLightCount = 0;
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
							  0.0f, 2.0f,
							  0.0f, 0.0f, 0.0f,
		                      0.0f, -1.0f, 0.0f,
							  1.0f, 0.0f, 0.0f,
							  20.0f);

	spotLightCount++;

	spotLights[1] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, -1.5f, 0.0f,
	   -100.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		20.0f
	);

	spotLightCount++;

	GLuint uniformModel = 0, uniformProjection = 0, uniformView = 0,
		uniformEyePosition = 0, uniformSpecularIntensity =0, uniformShininess=0
		;
	glm::mat4 projection = glm::perspective(45.0f, mainWindow.getBufferWidth()/mainWindow.getBufferHeight(),0.1f, 100.0f);


	//loop until window closed
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = static_cast<GLfloat>(glfwGetTime()); //SDL_GetPerformanceCounter();
		deltaTime = now - lastTime; // (now -lastTime) *1000 / SDL_GetPerformanceFrequency();
		lastTime = now;


		// Get and handle user input events
		glfwPollEvents();

		camera.keyControl(mainWindow.getKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// clear window
		// rgb values clears out entire screen
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f );
		//clears color and depth buffer
		glClear(GL_COLOR_BUFFER_BIT |GL_DEPTH_BUFFER_BIT) ;

		shaderList[0]->useShader();
		uniformModel = shaderList[0]->getModelLocation();
		uniformProjection = shaderList[0]->getProjectionLocation();
		uniformView = shaderList[0]->getViewLocation();
	
		uniformSpecularIntensity = shaderList[0]->getSpecularIntensityLocation();
		uniformShininess = shaderList[0]->getShininessLocation();
		uniformEyePosition = shaderList[0]->getEyePositionLocation();

		glm::vec3 flashLightPos = camera.getCameraPosition();
		flashLightPos.y -= 0.5f;
		//spotLights[0].setFlash(flashLightPos, camera.getCameraDirection());

		shaderList[0]->setDirectionalLight(&mainLight);
		shaderList[0]->setPointLights(pointLights, pointLightCount);
		shaderList[0]->setSpotLights(spotLights, spotLightCount);

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().x);



		glm::mat4 model(1.0f);

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
		//model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		brickTexture.useTexture();
		shinyMaterial.useMaterial(uniformSpecularIntensity, uniformShininess);

		meshList[0]->renderMesh();

		/*model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 4.0f, -2.5f));
		//model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dirtTexture.useTexture();
		dullMaterial.useMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->renderMesh();*/

		model = glm::mat4(1.0f);

		model = glm::translate(model, glm::vec3(-7.0f, 0.0f, 4.0f));
		model = glm::scale(model, glm::vec3(0.004f, 0.004f, 0.004f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		shinyMaterial.useMaterial(uniformSpecularIntensity, uniformShininess);
		xWing.renderModel();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.1f, 0.f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dirtTexture.useTexture();
		shinyMaterial.useMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->renderMesh();

		//unassign shader
		glUseProgram(0);
		mainWindow.swapBuffers();
		
	}
	int len = meshList.size();
	for (int i = 0; i < len; i++) delete meshList[i];
	len = shaderList.size();
	for (int i = 0; i < len; i++) delete shaderList[i];
	
	return 0;
}