#include "Material.h"

Material::Material()
{
	specularIntensity = 0;
	shininess = 0;
}

Material::Material(GLfloat sIntensity, GLfloat shine)
{
	specularIntensity = sIntensity;
	shininess = shine;
}

void Material::UseMaterial(GLuint specularIntensityLocation, GLuint shininesLocation)
{
	glUniform1f(specularIntensityLocation, specularIntensity);
	glUniform1f(shininesLocation, shininess);
}

Material::~Material()
{
}
