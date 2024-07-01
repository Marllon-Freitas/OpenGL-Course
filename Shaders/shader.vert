#version 330

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 texture;

out vec4 vertexColor;
out vec2 TextureCoordinates;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

void main()
{
	gl_Position = projection * view * model * vec4(pos, 1.0);
	vertexColor = vec4(clamp(pos, 0.0f, 1.0f), 1.0f);

	TextureCoordinates = texture;
};