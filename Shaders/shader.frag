#version 330

in vec4 vertexColor;
in vec2 TextureCoordinates;
in vec3 Normal;
in vec3 FragPosition;

out vec4 color;

const int MAX_POINT_LIGHTS = 3;

struct Light 
{
	vec3 colour;
	float ambientIntensity;
	float diffuseIntensity;
};

struct DirectionalLight 
{
	Light base;
	vec3 direction;
};

struct PointLight
{
	Light base;
	vec3 position;
	float constant;
	float linear;
	float exponent;
};

struct Material
{
	float specularIntensity;
	float shininess;
};

uniform int pointLightCount;

uniform DirectionalLight directionalLight;
uniform PointLight pointLights[MAX_POINT_LIGHTS];

uniform sampler2D theTexture;
uniform Material material;

uniform vec3 eyePosition;

vec4 CalculateLightByDirection(Light light, vec3 direction)
{
	vec4 ambientColor = vec4(light.colour, 1.0f) * light.ambientIntensity;

	float diffuseFactor = max(dot(normalize(Normal), normalize(direction)), 0.0f);
	vec4 diffuseColour = vec4(light.colour, 1.0f) * light.diffuseIntensity * diffuseFactor;

	vec4 specularColour = vec4(0, 0, 0, 0);

	if (diffuseFactor > 0.0f)
	{
		vec3 fragToeye = normalize(eyePosition - FragPosition);
		vec3 reflectedVertex = normalize(reflect(direction, normalize(Normal)));

		float specularFactor = dot(fragToeye, reflectedVertex);
		
		if(specularFactor > 0.0f)
		{
			specularFactor = pow(specularFactor, material.shininess);
			specularColour = vec4(light.colour * material.specularIntensity * specularFactor, 1.0f);
		}
	}

	return (ambientColor + diffuseColour + specularColour);
}

vec4 CalculateDirectionalLight()
{
	return CalculateLightByDirection(directionalLight.base, directionalLight.direction);
}

vec4 CalculatePointLights()
{
	vec4 totalColour = vec4(0, 0, 0, 0);

	for (int i = 0; i < pointLightCount; i++)
	{
		vec3 direction = FragPosition - pointLights[i].position;
		float distance = length(direction);
		direction = normalize(direction);
	
		vec4 colour = CalculateLightByDirection(pointLights[i].base, direction);
		float attenuation = pointLights[i].exponent * distance * distance + pointLights[i].linear * distance + pointLights[i].constant;

		totalColour += (colour / attenuation);
	}

	return totalColour;
}

void main()
{
	vec4 finalColour = CalculateDirectionalLight();
	finalColour += CalculatePointLights();
	color = texture(theTexture, TextureCoordinates) * finalColour;
};