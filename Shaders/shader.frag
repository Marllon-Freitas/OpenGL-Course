#version 330

in vec4 vertexColor;
in vec2 TextureCoordinates;
in vec3 Normal;

out vec4 color;

struct DirectionalLight 
{
	vec3 colour;
	float ambientIntensity;
	vec3 direction;
	float diffuseIntensity;
};

uniform sampler2D theTexture;
uniform DirectionalLight directionalLight;

void main()
{
	vec4 ambientColor = vec4(directionalLight.colour, 1.0f) * directionalLight.ambientIntensity;

	float diffuseFactor = max(dot(normalize(Normal), normalize(directionalLight.direction)), 0.0f);
	vec4 diffuseColour = vec4(directionalLight.colour, 1.0f) * directionalLight.diffuseIntensity * diffuseFactor;

	color = texture(theTexture, TextureCoordinates) * (ambientColor + diffuseColour);
};