#pragma once
#include <stdio.h>

#include<GL/glew.h>
#include<GLFW/glfw3.h>

class GLWindow
{
public:
	GLWindow();
	GLWindow(GLint windowWidth, GLint windowHeight);
	GLfloat getBufferWidth() const { return static_cast<GLfloat>(m_bufferWidth); }
	GLfloat getBufferHeight() const { return static_cast<GLfloat>(m_bufferHeight); }
	int initialize();
	bool getShouldClose() { return glfwWindowShouldClose(m_mainWindow); }
	bool* getKeys() { return m_keys; }
	GLfloat getXChange();
	GLfloat getYChange();

	void swapBuffers() { glfwSwapBuffers(m_mainWindow); }
	~GLWindow();
private:
	GLFWwindow * m_mainWindow;
	GLint m_width, m_height;
	GLint m_bufferWidth, m_bufferHeight;

	//bool for each ascii character
	bool m_keys[1024]; 

	//mouse movement variables
	GLfloat m_lastX, m_lastY, m_xChange, m_yChange;
	bool m_mouseFirstMoved;


	void createCallBacks();
	static void handleKeys(GLFWwindow* window, int key, int code, int action, int mode);
	static void handleMouse(GLFWwindow * window, double xpos, double ypos);
};

