#version 430 core

const float PI = 3.1415926535897932384626433832795;

const float waveLength = 20.0;
const float waveAmplitude = 0.5;

layout(location = 0)in vec3 position;
layout(location = 1)in vec2 textureCoord;
layout(location = 2)in vec3 normal;
layout(location = 3)in vec3 colour;

out vec2 texCoord0;
out vec3 normal0; 
out vec4 colour0;
out vec4 FragPos;
out float timerFrag;

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;
uniform float timer;

float generateOffset(float x, float z){
	float radiansX = (x / waveLength + timer) * 2.0 * PI;
	float radiansZ = (z / waveLength + timer) * 2.0 * PI;
	return waveAmplitude * 0.5 * (sin(radiansZ) + cos(radiansX));
}

vec3 applyDistortion(vec3 vertex){
	float xDistortion = generateOffset(vertex.x, vertex.z);
	float yDistortion = generateOffset(vertex.x, vertex.z);
	float zDistortion = generateOffset(vertex.x, vertex.z);
	return vertex + vec3(xDistortion, yDistortion, zDistortion);
}

void main()
{
	vec3 currentVertex = position;

	currentVertex = applyDistortion(currentVertex);

	vec4 worldPos = modelMatrix * vec4(currentVertex, 1.0);
	vec4 viewPos = viewMatrix * vec4(worldPos.xyz, 1.0);
	vec4 clipPos = projectionMatrix * viewPos;
	gl_Position = clipPos;
	FragPos = worldPos;
	texCoord0 = textureCoord;
	colour0 = vec4(colour, 1.0);
	normal0 = (modelMatrix * vec4(normal, 0.0)).xyz;
	timerFrag = timer;
}