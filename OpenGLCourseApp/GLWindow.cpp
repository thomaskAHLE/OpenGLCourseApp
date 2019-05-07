#include "GLWindow.h"



GLWindow::GLWindow()
{
	m_width = 800;
	m_height = 600;

	for (size_t i = 0; i < 1024; i++)
	{
		m_keys[i] = 0;
	}
	m_xChange = m_yChange = 0.0f;
}

GLWindow::GLWindow(GLint windowWidth, GLint windowHeight)
{
	m_width = windowWidth;
	m_height = windowHeight;

	for (size_t i = 0; i < 1024; i++)
	{
		m_keys[i] = 0;
	}
	m_xChange = m_yChange = 0.0f;
}

int GLWindow::initialize()
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
	m_mainWindow = glfwCreateWindow(m_width, m_height, "Test Window", NULL, NULL);
	if (!m_mainWindow)
	{
		printf("GLFW window creation failed!");
		glfwTerminate();
		return 1;
	}


	//Get buffer size information
	glfwGetFramebufferSize(m_mainWindow, &m_bufferWidth, &m_bufferHeight);

	//set context for Glew to use
	glfwMakeContextCurrent(m_mainWindow);

	//handle key and mouse call backs
	createCallBacks();

	//lock mouse to window
	glfwSetInputMode(m_mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//allow modern extension features
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("GLeW init failed");
		glfwDestroyWindow(m_mainWindow);
		glfwTerminate();
		return 1;
	}

	//depth buffer
	glEnable(GL_DEPTH_TEST);

	//setup viewport size

	glViewport(0, 0, m_bufferWidth, m_bufferHeight);
	glfwSetWindowUserPointer(m_mainWindow, this);
	return 0;
}

GLfloat GLWindow::getXChange()
{
	GLfloat theChange = m_xChange;
	m_xChange = 0.0f;
	return theChange;
}

GLfloat GLWindow::getYChange()
{
	GLfloat theChange = m_yChange;
	m_yChange = 0.0f;
	return theChange;
}


GLWindow::~GLWindow()
{
	glfwDestroyWindow(m_mainWindow);
	glfwTerminate();
}

void GLWindow::createCallBacks()
{
	glfwSetKeyCallback(m_mainWindow, handleKeys);
	glfwSetCursorPosCallback(m_mainWindow, handleMouse);
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
			theWindow->m_keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			theWindow->m_keys[key] = false;
		}
	}

}

void GLWindow::handleMouse(GLFWwindow * window, double xpos, double ypos)
{
	GLWindow* theWindow = static_cast<GLWindow*>(glfwGetWindowUserPointer(window));
	
	if (theWindow->m_mouseFirstMoved)
	{
		theWindow->m_lastX = (GLfloat) xpos;
		theWindow->m_lastY = (GLfloat) ypos;
		theWindow->m_mouseFirstMoved = false;
	}
	theWindow->m_xChange = xpos - theWindow->m_lastX;

	//other way to make inverted y movement
	theWindow->m_yChange = theWindow->m_lastY - ypos;
	
	theWindow->m_lastX = (GLfloat) xpos;
	theWindow->m_lastY = (GLfloat) ypos;
}
