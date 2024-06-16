#include "Window.h"

Window::Window()
{
	width = 800;
	height = 600;
	bufferWidth = 800;
	bufferHeight = 600;
	mainWindow = NULL;
}

Window::Window(GLint windowWidth, GLint windowHeight)
{
	width = windowWidth;
	height = windowHeight;
	bufferWidth = windowWidth;
	bufferHeight = windowHeight;
	mainWindow = NULL;
}

int Window::Initialise()
{
	// Initialize GLFW
	if (!glfwInit())
	{
		printf("GLFW initialization failed!");
		glfwTerminate();
		return 1;
	}

	// Setup GLFW window properties
	// OpenGL version 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Core profile = No backwards Compatibility
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Allow forward compatibility
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);

	mainWindow = glfwCreateWindow(width, height, "Test Window", NULL, NULL);

	if (!mainWindow)
	{
		printf("GLFW window creation failed!");
		glfwTerminate();
		return 1;
	}

	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	// Set the context for GLEW to use
	glfwMakeContextCurrent(mainWindow);

	// Allow modern extension features
	glewExperimental = GL_TRUE;

	// Initialize GLEW
	if (glewInit() != GLEW_OK)
	{
		printf("GLEW initialization failed!");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	// Depth buffer
	glEnable(GL_DEPTH_TEST);

	// Setup view port size
	glViewport(0, 0, bufferWidth, bufferHeight);

}

Window::~Window()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();
}
