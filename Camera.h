#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
	Camera();
	Camera(glm::vec3 initialPosition,
		glm::vec3 initialUpDirection,
		GLfloat initialYaw,
		GLfloat initialPitch,
		GLfloat initialMovementSpeed,
		GLfloat initialTurnSpeed);
	void KeyControl(bool* keys, GLfloat deltaTime);
	void MouseControl(GLfloat xChange, GLfloat yChange);
	glm::vec3 GetCameraPosition();
	glm::mat4 CalculateViewMatrix();
	~Camera();

private:
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;	// Need to know which way the camera is rotating (where the sky is basically)

	GLfloat yaw;		// Rotation around the camera's Y-axis. Determines how much the camera moves side to side.
	GLfloat pitch;		// Rotation around the camera's X-axis. Determines how much the camera moves up and down.
	GLfloat movementSpeed;
	GLfloat turnSpeed;

	void update();
};
