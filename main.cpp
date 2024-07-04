#define _USE_MATH_DEFINES
#define STB_IMAGE_IMPLEMENTATION

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
#include "Texture.h"
#include "Light.h"

// Transforms everything we multiply by this in a radian
const float toRadians = M_PI / 180.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;
Camera camera;

Texture dirtTexture;
Texture brickTexture;

Light mainLight;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

// Vertex Shader
static const char* vShader = "Shaders/shader.vert";

// Fragment Shader
static const char* fShader = "Shaders/shader.frag";

void CalcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices,
	unsigned int verticesCount, unsigned int vertexLenght, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vertexLenght;
		unsigned int in1 = indices[i + 1] * vertexLenght;
		unsigned int in2 = indices[i + 2] * vertexLenght;
		glm::vec3 v1(
			vertices[in1] - vertices[in0],
			vertices[in1 + 1] - vertices[in0 + 1],
			vertices[in1 + 2] - vertices[in0 + 2]
		);
		glm::vec3 v2(
			vertices[in2] - vertices[in0],
			vertices[in2 + 1] - vertices[in0 + 1],
			vertices[in2 + 2] - vertices[in0 + 2]
		);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset;
		in1 += normalOffset;
		in2 += normalOffset;

		vertices[in0] += normal.x;
		vertices[in0 + 1] += normal.y;
		vertices[in0 + 2] += normal.z;

		vertices[in1] += normal.x;
		vertices[in1 + 1] += normal.y;
		vertices[in1 + 2] += normal.z;

		vertices[in2] += normal.x;
		vertices[in2 + 1] += normal.y;
		vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticesCount / vertexLenght; i++)
	{
		unsigned int nOffset = i * vertexLenght + normalOffset;
		glm::vec3 vec(
			vertices[nOffset],
			vertices[nOffset + 1],
			vertices[nOffset + 2]
		);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x;
		vertices[nOffset + 1] = vec.y;
		vertices[nOffset + 2] = vec.z;
	}
}

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
		//x	   y	  z			u	  v			nx	  ny	nz
		-1.0f, -1.0f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 1.0f,		0.5f, 0.5,		0.0f, 0.0f, 0.0f,
		1.0f, -1.0f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f,
	};

	CalcAverageNormals(indices, 12, vertices, 32, 8, 5);

	Mesh* object01 = new Mesh();
	object01->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(object01);

	Mesh* object02 = new Mesh();
	object02->CreateMesh(vertices, indices, 32, 12);
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

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTexture();

	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTexture();

	mainLight = Light(1.0f, 1.0f, 1.0f, 0.3f, 1.0f, -1.0f, 2.0f, 1.3f);

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformAmbientIntensity = 0,
		uniformAmbientColour = 0, uniformDirection = 0, uniformDiffuseIntensity = 0;

	// Projection matrix
	glm::mat4 projection = glm::perspective(
		45.0f,														// Field of view
		mainWindow.getBufferWidth() / mainWindow.getBufferHeight(),	// Aspect(window width / window height)
		0.3f,														// Near view - closest thing we can see
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
		uniformAmbientColour = shaderList[0].GetAmbientColourLocation();
		uniformAmbientIntensity = shaderList[0].GetAmbientIntensityLocation();
		uniformDirection = shaderList[0].GetDirectionLocation();
		uniformDiffuseIntensity = shaderList[0].GetDiffuseIntensityLocation();

		mainLight.UseLight(uniformAmbientIntensity, uniformAmbientColour, uniformDiffuseIntensity, uniformDirection);

		// Model matrix
		glm::mat4 model(1.0f);

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
		// model = glm::rotate(model, currentAngle * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.CalculateViewMatrix()));
		brickTexture.UseTexture();

		meshList[0]->RenderMesh();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, -2.5f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dirtTexture.UseTexture();

		meshList[1]->RenderMesh();

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
