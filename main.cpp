#define _USE_MATH_DEFINES

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"

// Transforms everything we multiply by this in a radian
const float toRadians = M_PI / 180.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;
Camera camera;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

// Vertex Shader
static const char* vShader = "Shaders/shader.vert";

// Fragment Shader
static const char* fShader = "Shaders/shader.frag";

void CreateObjects()
{
	unsigned int indices[] =
	{
		0, 3, 1, // left side
		1, 3, 2, // right side
		2, 3, 0, // front side
		0, 1, 2  // base
	};

	GLfloat vertices[] =
	{
		-1.0f, -1.0f, 0.0f, // bottom left [0]
		0.0f, -1.0f, 1.0f,  // background  [1]
		1.0f, -1.0f, 0.0f,  // bottom right[2]
		0.0f, 1.0f, 0.0f    // top		   [3]
	};

	Mesh* object01 = new Mesh();
	object01->CreateMesh(vertices, indices, 12, 12);
	meshList.push_back(object01);

	Mesh* object02 = new Mesh();
	object02->CreateMesh(vertices, indices, 12, 12);
	meshList.push_back(object02);
}

void CreateShaders()
{
	Shader* shader01 = new Shader();
	shader01->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader01);
}

int main()
{
	mainWindow = Window(800, 600);
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 3.0f, 0.09f);

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0;

	// Projection matrix
	glm::mat4 projection = glm::perspective(
		45.0f,														// Field of view
		mainWindow.getBufferWidth() / mainWindow.getBufferHeight(),	// Aspect(window width / window height)
		1.0f,														// Near view - closest thing we can see
		100.0f														// Far view - farthest thing we can see
	);


	// Loop until window close
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		lastTime = now;

		// Get + Handle user input events
		glfwPollEvents();

		camera.KeyControl(mainWindow.getKeys(), deltaTime);

		camera.MouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();

		// Model matrix
		glm::mat4 model(1.0f);

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
		// model = glm::rotate(model, currentAngle * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.CalculateViewMatrix()));
		meshList[0]->RenderMesh();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, -2.5f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
