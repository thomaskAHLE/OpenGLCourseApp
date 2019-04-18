#include "GLWindow.h"



GLWindow::GLWindow()
{
	width = 800;
	height = 600;
}

GLWindow::GLWindow(GLint windowWidth, GLint windowHeight)
{
	width = windowWidth;
	height = windowHeight;
}

int GLWindow::Initialize()
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
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//allowing forward compatibility
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	mainWindow = glfwCreateWindow(width, height, "Test Window", NULL, NULL);
	if (!mainWindow)
	{
		printf("GLFW window creation failed!");
		glfwTerminate();
		return 1;
	}


	//Get buffer size information
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

	//depth buffer
	glEnable(GL_DEPTH_TEST);

	//setup viewport size

	glViewport(0, 0, bufferWidth, bufferHeight);
	return 0;
}


GLWindow::~GLWindow()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();
}
