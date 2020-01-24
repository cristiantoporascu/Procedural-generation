#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, GLenum movementState, bool isTree)
{
	this->m_vertices = vertices;
	this->m_indices = indices;

	this->isTree = isTree;

	this->m_drawCount = indices.size();

	this->m_movementState = movementState;

	// Generate the id for the VAO
	// You need to pass in the memory location of it because it need to be a pointer and if it's not an array is not automaticaly a pointer in the class
	glGenVertexArrays(1, &this->m_vertexArrayObject);

	// Bind the VAO
	glBindVertexArray(this->m_vertexArrayObject);

	// Generate the id for the VBO
	glGenBuffers(NUMBER_BUFFERS, this->m_vertexBufferObject);

	this->SetArrayData();

	// Stop everything which affects the VAO
	glBindVertexArray(0);
}

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &this->m_vertexArrayObject);
	glDeleteBuffers(1, this->m_vertexBufferObject);
}

void Mesh::SetArrayData() {
	// Redefine so the shaders can access the values
	std::vector<glm::vec3> positions;
	std::vector<glm::vec2> textureCoords;
	std::vector<glm::vec3> colour;
	std::vector<glm::vec3> normals;

	for (unsigned int i = 0; i < this->m_vertices.size(); i++)
	{
		positions.push_back(this->m_vertices[i].pos);
		textureCoords.push_back(this->m_vertices[i].textureCoord);
		colour.push_back(this->m_vertices[i].colour);
		normals.push_back(this->m_vertices[i].normals);
	}

	// POSITION
	this->SetBufferData(this->m_vertexBufferObject[POSITION_VBO], positions.data(), positions.size() * sizeof(glm::vec3));

	// Set the information to the first input in the buffer
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// TEXTURE
	this->SetBufferData(this->m_vertexBufferObject[TEXTURE_COORD], textureCoords.data(), textureCoords.size() * sizeof(glm::vec2));

	// Set the information to the first input in the buffer
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	// NORMAL
	this->SetBufferData(this->m_vertexBufferObject[NORMAL_VB], normals.data(), normals.size() * sizeof(glm::vec3));

	// Set the information to the first input in the buffer
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// COLOUR
	this->SetBufferData(this->m_vertexBufferObject[COLOUR_VB], colour.data(), colour.size() * sizeof(glm::vec3));

	// Set the information to the first input in the buffer
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// INDEX
	this->SetBufferData(this->m_vertexBufferObject[INDEX_VB], this->m_indices.data(), this->m_indices.size() * sizeof(unsigned int), true);
}

void Mesh::SetBufferData(GLuint bufferObject, const void* data, unsigned int size, bool indices) {
	glBindBuffer(indices ? GL_ELEMENT_ARRAY_BUFFER : GL_ARRAY_BUFFER, bufferObject);
	glBufferData(indices ? GL_ELEMENT_ARRAY_BUFFER : GL_ARRAY_BUFFER, size, data, this->m_movementState);
}

void Mesh::ResetArrayBufferData() {
	this->m_drawCount = this->m_indices.size();

	// Redefine so the shaders can access the values
	std::vector<glm::vec3> positions;
	std::vector<glm::vec2> textureCoords;
	std::vector<glm::vec3> colour;
	std::vector<glm::vec3> normals;

	for (unsigned int i = 0; i < this->m_vertices.size(); i++)
	{
		positions.push_back(this->m_vertices[i].pos);
		textureCoords.push_back(this->m_vertices[i].textureCoord);
		colour.push_back(this->m_vertices[i].colour);
		normals.push_back(this->m_vertices[i].normals);
	}

	// POSITION
	this->SetBufferData(this->m_vertexBufferObject[POSITION_VBO], positions.data(), positions.size() * sizeof(glm::vec3));

	// TEXTURE
	this->SetBufferData(this->m_vertexBufferObject[TEXTURE_COORD], textureCoords.data(), textureCoords.size() * sizeof(glm::vec2));

	// NORMAL
	this->SetBufferData(this->m_vertexBufferObject[NORMAL_VB], normals.data(), normals.size() * sizeof(glm::vec3));

	// COLOUR
	this->SetBufferData(this->m_vertexBufferObject[COLOUR_VB], colour.data(), colour.size() * sizeof(glm::vec3));

	// INDEX
	this->SetBufferData(this->m_vertexBufferObject[INDEX_VB], this->m_indices.data(), this->m_indices.size() * sizeof(unsigned int), true);
}

void Mesh::DrawMesh()
{
	glBindVertexArray(this->m_vertexArrayObject);

	if (this->isTree) {
		glDrawElements(GL_LINES, this->m_drawCount, GL_UNSIGNED_INT, nullptr);
	}
	else {
		glDrawElements(GL_TRIANGLE_STRIP, this->m_drawCount, GL_UNSIGNED_INT, nullptr);
	}
	glBindVertexArray(0);
}