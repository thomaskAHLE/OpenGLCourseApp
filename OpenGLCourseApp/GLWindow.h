#pragma once
#include <stdio.h>

#include<GL/glew.h>
#include<GLFW/glfw3.h>

class GLWindow
{
public:
	GLWindow();
	GLWindow(GLint windowWidth, GLint windowHeight);
	GLfloat GetBufferWidth() const { return bufferWidth; }
	GLfloat GetBufferHeight() const { return bufferHeight; }
	int Initialize();
	bool GetShouldClose() { return glfwWindowShouldClose(mainWindow); }
	void swapBuffers() { glfwSwapBuffers(mainWindow); }
	~GLWindow();
private:
	GLFWwindow * mainWindow;
	GLint width, height;
	GLint bufferWidth, bufferHeight;
};

