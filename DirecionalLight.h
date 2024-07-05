#pragma once

#include "Light.h"

class DirectionalLight : public Light
{
public:
	DirectionalLight();
	DirectionalLight(GLfloat red, GLfloat green, GLfloat blue,
		GLfloat aIntensity, GLfloat dIntensity,
		GLfloat xDirection, GLfloat yDirection, GLfloat zDirection);

	void UseLight(GLfloat ambientIntensityLocation, GLfloat ambientColourLocation,
		GLfloat diffuseIntensityLocation, GLfloat directionLocation
	);
	~DirectionalLight();

private:
	glm::vec3 direction;
};
