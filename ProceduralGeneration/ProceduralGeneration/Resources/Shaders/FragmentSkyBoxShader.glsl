#version 430 core

in vec2 texCoord0;
in vec3 normal0;
in vec4 colour0;
in vec4 FragPos;

uniform vec3 cameraPos;

out vec4 colourExport;
uniform sampler2D diffuse;

float diffDir;

float spec;
float specularStrength = 4;

vec4 expColour;

vec4 makeColour()
{
	vec3 norm = normalize(normal0.xyz);
	vec3 lightDir = normalize((vec3(0.0, 700.0, -1000.0) - FragPos.xyz).xyz);
	diffDir = max(dot(norm, lightDir), 0.4);

	vec3 lightDiffuse = diffDir * vec3(1.0, 1.0, 1.0); // Diffuse direction and the light colour

	vec3 viewDir = normalize(cameraPos - FragPos.xyz);
	vec3 reflectDir = reflect(-lightDir, norm);

	spec = pow(max(dot(viewDir, reflectDir), 0.0), 16) * specularStrength;

	return vec4((lightDiffuse + spec) * colour0, 0.0);
}

void main()
{
	expColour = makeColour();
	vec4 textureSet = texture(diffuse, texCoord0);
	
	colourExport = colour0 * textureSet * clamp(dot(vec3(0,1,0), normalize(normal0)), 0.4, 1.0);
}