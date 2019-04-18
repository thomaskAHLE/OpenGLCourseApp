#include "GLWindow.h"



GLWindow::GLWindow()
{
	width = 800;
	height = 600;

	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}

GLWindow::GLWindow(GLint windowWidth, GLint windowHeight)
{
	width = windowWidth;
	height = windowHeight;

	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
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

	//handle key and mouse call backs
	createCallBacks();

	//lock mouse to window
	glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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
	glfwSetWindowUserPointer(mainWindow, this);
	return 0;
}

GLfloat GLWindow::GetXChange()
{
	GLfloat theChange = xChange;
	xChange = 0.0f;
	return theChange;
}

GLfloat GLWindow::GetYChange()
{
	GLfloat theChange = yChange;
	yChange = 0.0f;
	return theChange;
}


GLWindow::~GLWindow()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();
}

void GLWindow::createCallBacks()
{
	glfwSetKeyCallback(mainWindow, handleKeys);
	glfwSetCursorPosCallback(mainWindow, handleMouse);
}

void GLWindow::handleKeys(GLFWwindow * window, int key, int code, int action, int mode)
{
	GLWindow* theWindow = static_cast<GLWindow*>(glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);

	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			theWindow->keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			theWindow->keys[key] = false;
		}
	}

}

void GLWindow::handleMouse(GLFWwindow * window, double xpos, double ypos)
{
	GLWindow* theWindow = static_cast<GLWindow*>(glfwGetWindowUserPointer(window));
	
	if (theWindow->mouseFirstMoved)
	{
		theWindow->lastX = xpos;
		theWindow->lastY = ypos;
		theWindow->mouseFirstMoved = false;
	}
	theWindow->xChange = xpos - theWindow->lastX;

	//other way to make inverted y movement
	theWindow->yChange = theWindow->lastY - ypos;
	
	theWindow->lastX = xpos;
	theWindow->lastY = ypos;
}
