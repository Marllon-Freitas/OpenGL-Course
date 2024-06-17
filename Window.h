#pragma once

#include "stdio.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Window
{
public:
	Window();
	Window(GLint windowWidth, GLint windowHeight);
	int Initialise();
	GLfloat getBufferWidth() { return bufferWidth; }
	GLfloat getBufferHeight() { return bufferHeight; }
	GLfloat getXChange();
	GLfloat getYChange();
	bool getShouldClose() { return glfwWindowShouldClose(mainWindow); }
	bool* getKeys() { return keys; }
	void swapBuffers() { glfwSwapBuffers(mainWindow); }
	~Window();

private:
	GLFWwindow* mainWindow;

	GLint width, height;
	GLint bufferWidth, bufferHeight;

	// Last coordinates of the mouse
	GLfloat lastX;
	GLfloat lastY;

	// We can get the difference between the last and the current coordinates to get the how much things change
	GLfloat xChange;
	GLfloat yChange;

	bool mousedFirstMoved;
	bool keys[1024];

	void CreateCallbacks();
	static void handleKeys(GLFWwindow* window, int key, int code, int action, int mode);
	static void handleMouse(GLFWwindow* window, double xPos, double yPos);
};
