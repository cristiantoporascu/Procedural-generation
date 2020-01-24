#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include "../Mathematics/Vertex.h"

class Mesh
{
private:
	enum
	{
		POSITION_VBO,
		TEXTURE_COORD,
		NORMAL_VB,
		COLOUR_VB,

		INDEX_VB,

		NUMBER_BUFFERS
	};

	// Lists of vertices and indices
	std::vector<Vertex> m_vertices;
	std::vector<unsigned int> m_indices;

	// Id for the VAO
	GLuint m_vertexArrayObject;

	// Id for the VBO
	GLuint m_vertexBufferObject[NUMBER_BUFFERS];

	// Number of the triagnles which need to be drawn
	unsigned int m_drawCount;

	// Movement state
	GLenum m_movementState;

	// Trees generated
	bool isTree = false;

public:
	Mesh() {}
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, GLenum movementState, bool isTree = false);
	~Mesh();

	/**
	 * Use the specific indices or vertices for drawing the VAO
	 */
	void DrawMesh();

	/**
	 * Binds the data on the specific layout location and entry point on the shader
	 */
	void SetArrayData();

	/**
	 * Binds the specific data to the right buffer
	 * @param bufferObject					Buffer location for bufferData
	 * @param data							Buffer data casted in void*
	 * @param size							Necesary size for the buffer
	 * @param indices						Whether it should bind to the array or to the buffer
	 */
	void SetBufferData(GLuint bufferObject, const void* data, unsigned int size, bool indices = false);

	/**
	 * Resets the array buffer data on request based on the changes made to vertices data
	 */
	void ResetArrayBufferData();

	/**
	 * Getters and setters
	 */
public:
	inline GLuint* GetVertexBufferObject() const { return (GLuint*)(&this->m_vertexBufferObject[0]); }
	inline std::vector<Vertex> GetVertices() const { return this->m_vertices; }
	inline std::vector<unsigned int> GetIndices() const { return this->m_indices; }
	inline GLenum GetMovementState() const { return this->m_movementState; }
	inline const bool& GetIsTree() const { return this->isTree; }

	inline void SetVertices(std::vector<Vertex> newVertices) { this->m_vertices = newVertices; }
	inline void SetIndices(std::vector<unsigned int> newIndices) { this->m_indices = newIndices; }
	inline void SetMovementState(GLenum newMovingState) { this->m_movementState = newMovingState; }
};