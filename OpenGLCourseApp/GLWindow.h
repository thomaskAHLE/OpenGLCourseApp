#pragma once
#include <stdio.h>

#include<GL/glew.h>
#include<GLFW/glfw3.h>

class GLWindow
{
public:
	GLWindow();
	GLWindow(GLint windowWidth, GLint windowHeight);
	GLfloat GetBufferWidth() const { return (GLfloat)bufferWidth; }
	GLfloat GetBufferHeight() const { return (GLfloat)bufferHeight; }
	int Initialize();
	bool GetShouldClose() { return glfwWindowShouldClose(mainWindow); }
	bool* GetKeys() { return keys; }
	GLfloat GetXChange();
	GLfloat GetYChange();

	void SwapBuffers() { glfwSwapBuffers(mainWindow); }
	~GLWindow();
private:
	GLFWwindow * mainWindow;
	GLint width, height;
	GLint bufferWidth, bufferHeight;

	//bool for each ascii character
	bool keys[1024]; 

	//mouse movement variables
	GLfloat lastX, lastY, xChange, yChange;
	bool mouseFirstMoved;


	void createCallBacks();
	static void handleKeys(GLFWwindow* window, int key, int code, int action, int mode);
	static void handleMouse(GLFWwindow * window, double xpos, double ypos);
};

