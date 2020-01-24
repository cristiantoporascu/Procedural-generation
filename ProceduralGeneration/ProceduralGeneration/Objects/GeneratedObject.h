#pragma once
#include "../Mathematics/Transform.h"
#include "../Mathematics/Vertex.h"
#include "../Shaders/Shader.h"
#include "Texture.h"
#include "Mesh.h"
#include <glm/glm.hpp>

class GeneratedObjects
{
public:
	Transform* m_transform;
	Mesh* m_mesh;
	Shader* m_shader;
	Texture* m_texture;
	glm::vec3 direction = glm::vec3(1.0, 0.0, 0.0);

	GeneratedObjects();
	GeneratedObjects(
		Transform* transform,
		std::vector<Vertex> vertices,
		std::vector<unsigned int> indices, 
		GLenum movementState,
		std::string shaderFileName = "GroundShader",
		std::vector<std::string> textureFileName = { "Default.jpg" },
		int textureSize = 1,
		bool isTree = false);
	GeneratedObjects(
		Transform* transform,
		std::vector<Vertex> vertices,
		std::vector<unsigned int> indices,
		GLenum movementState,
		std::vector<void*> textureData,
		std::string shaderFileName = "GroundShader",
		int textureSize = 1,
		bool isTree = false);
	GeneratedObjects(
		Transform* transform,
		Mesh* mesh,
		std::string shaderFileName = "GroundShader",
		std::vector<std::string> textureFileName = { "Default.jpg" },
		int textureSize = 1);
	GeneratedObjects(
		Transform* transform,
		Mesh* mesh,
		std::vector<void*> textureData,
		std::string shaderFileName = "GroundShader",
		int textureSize = 1);
	~GeneratedObjects();

	/**
	 * Calls properties for the graphics part of the
	 * GeneratedObjects in order to render the mesh on the scene
	 * @param deltaTime							Value used to shift the texture offset and animate it
	 */
	virtual void DrawScene(float deltaTime = 0);

	/**
	 * What does the object need to do when you first run the application
	 */
	virtual void Start();

	/**
	 * What does the object need to do every frame drawn
	 */
	virtual void Update();
};
