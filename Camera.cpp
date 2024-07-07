#include "Camera.h"

Camera::Camera()
{
}

Camera::Camera(glm::vec3 initialPosition, glm::vec3 initialUpDirection, GLfloat initialYaw, GLfloat initialPitch, GLfloat initialMovementSpeed, GLfloat initialTurnSpeed)
{
	position = initialPosition;
	worldUp = initialUpDirection;
	yaw = initialYaw;
	pitch = initialPitch;
	front = glm::vec3(0.0f, 0.0f, -1.0f);

	movementSpeed = initialMovementSpeed;
	turnSpeed = initialTurnSpeed;

	update();
}

void Camera::KeyControl(bool* keys, GLfloat deltaTime)
{
	GLfloat velocity = movementSpeed * deltaTime;

	if (keys[GLFW_KEY_W])
	{
		position += front * velocity;
	}

	if (keys[GLFW_KEY_S])
	{
		position -= front * velocity;
	}

	if (keys[GLFW_KEY_A])
	{
		position -= right * velocity;
	}

	if (keys[GLFW_KEY_D])
	{
		position += right * velocity;
	}

	if (keys[GLFW_KEY_LEFT_SHIFT])
	{
		position -= up * velocity;
	}

	if (keys[GLFW_KEY_SPACE])
	{
		position += up * velocity;
	}
}

void Camera::MouseControl(GLfloat xChange, GLfloat yChange)
{
	xChange *= turnSpeed;
	yChange *= turnSpeed;

	yaw += xChange;
	pitch += yChange;

	// This prevents the camera from flipping over at the top.
	if (pitch > 89.0f)
	{
		pitch = 89.0f;
	}

	// This prevents the camera from flipping over at the bottom.
	if (pitch < -89.0f)
	{
		pitch = -89.0f;
	}

	update();
}

glm::vec3 Camera::GetCameraPosition()
{
	return position;
}

glm::vec3 Camera::GetCameraDirection()
{
	return glm::normalize(front);
}

glm::mat4 Camera::CalculateViewMatrix()
{
	return glm::lookAt(position, position + front, up);
}

void Camera::update()
{

	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(front);

	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}

Camera::~Camera()
{
}
