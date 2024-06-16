#define _USE_MATH_DEFINES

#include <stdio.h>
#include <string.h>
#include <cmath>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Window dimension
const GLint WIDTH = 800, HEIGHT = 600;
// Transforms everything we multiply by this in a radian
const float toRadians = M_PI / 180.0f;

GLuint VAO, VBO, IBO, shader, uniformModel;

// true = right, false = left
bool direction = true;
float triangleOffset = 0.0f;
float triangleMaxOffset = 0.7f;
float triangleIncrement = 0.005f;

float currentAngle = 0.0f;

bool sizeDirection = true;
float currentSize = 0.4f;
float maxSize = 0.8f;
float minSize = 0.1f;

// Vertex Shader
static const char* vShader = "											\n\
#version 330															\n\
																		\n\
layout(location = 0) in vec3 pos;										\n\
																		\n\
out vec4 vertexColor;													\n\
																		\n\
uniform mat4 model; 													\n\
																		\n\
void main()																\n\
{																		\n\
	gl_Position = model * vec4(pos, 1.0);								\n\
	vertexColor = vec4(clamp(pos, 0.0f, 1.0f), 1.0f);					\n\
}";

// Fragment Shader
static const char* fShader = "											\n\
#version 330															\n\
																		\n\
in vec4 vertexColor;													\n\
																		\n\
out vec4 color;															\n\
																		\n\
void main()																\n\
{																		\n\
	color = vertexColor;												\n\
}";

void CreateTriangle()
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

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// create buffer for IBO (or EBO)
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	// unbind the IBO/EBO after the VAO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void AddShader(GLint theProgram, const char* shaderCode, GLenum shaderType)
{
	GLint theShader = glCreateShader(shaderType);

	const GLchar* theCode[1];
	theCode[0] = shaderCode;

	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);

	glShaderSource(theShader, 1, theCode, codeLength);
	glCompileShader(theShader);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(theShader, sizeof(eLog), NULL, eLog);
		printf("Error compiling the %d shader: '%s'\n", shaderType, eLog);
		return;
	}

	glAttachShader(theProgram, theShader);
}

void CompileShaders()
{
	shader = glCreateProgram();

	if (!shader)
	{
		printf("Error creating shader program!\n");
		return;
	}

	AddShader(shader, vShader, GL_VERTEX_SHADER);
	AddShader(shader, fShader, GL_FRAGMENT_SHADER);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glLinkProgram(shader);
	glGetProgramiv(shader, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("Error linking program: '%s'\n", eLog);
		return;
	}

	glValidateProgram(shader);
	glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("Error validating program: '%s'\n", eLog);
		return;
	}

	uniformModel = glGetUniformLocation(shader, "model");
}

int main()
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

	GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Test Window", NULL, NULL);

	if (!mainWindow)
	{
		printf("GLFW window creation failed!");
		glfwTerminate();
		return 1;
	}

	// Get Buffer size information
	int bufferWidth, bufferHeight;
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

	CreateTriangle();
	CompileShaders();

	// Loop until window close
	while (!glfwWindowShouldClose(mainWindow))
	{
		// Get + Handle user input events
		glfwPollEvents();

		// Increment the offset of the triangle based on the direction
		if (direction)
		{
			triangleOffset += triangleIncrement;
		}
		else
		{
			triangleOffset -= triangleIncrement;
		}

		// If the offset exceeds our offset limit, change the direction
		if (abs(triangleOffset) >= triangleMaxOffset)
		{
			direction = !direction;
		}

		currentAngle += 0.5f;
		if (currentAngle >= 360)
		{
			currentAngle -= 360;
		}

		if (sizeDirection)
		{
			currentSize += 0.001f;
		}
		else
		{
			currentSize -= 0.001f;
		}

		if (currentSize >= maxSize || currentSize <= minSize)
		{
			sizeDirection = !sizeDirection;
		}

		// Clear window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(shader);

		// Model matrix
		glm::mat4 model(1.0f);
		model = glm::rotate(model, currentAngle * toRadians, glm::vec3(0.5f, 1.0f, 0.0f));
		//model = glm::translate(model, glm::vec3(triangleOffset, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));


		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		glBindVertexArray(VAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

		glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glUseProgram(0);

		glfwSwapBuffers(mainWindow);
	}

	return 0;
}