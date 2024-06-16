#pragma once

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>

#include <GL/glew.h>

class Shader
{
public:
	Shader();
	void CreateFromString(const char* vertexCode, const char* fragmentCode);
	void CreateFromFiles(const char* vertexLocation, const char* fragmentLocation);
	std::string ReadFile(const char* fileLocation);
	void UseShader();
	void ClearShader();
	GLuint GetProjectionLocation();
	GLuint GetModelLocation();
	~Shader();

private:
	GLuint shaderID, uniformModel, uniformProjection;
	void CompileShader(const char* vertexCode, const char* fragmentCode);
	void AddShader(GLint theProgram, const char* shaderCode, GLenum shaderType);
};
