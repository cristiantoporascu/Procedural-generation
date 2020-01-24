#include "Transform.h"

Transform::Transform()
{
	this->m_pos = glm::vec3();
	this->m_rot = glm::vec3();
	this->m_scale = glm::vec3(1.0, 1.0, 1.0);
}

Transform::Transform(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale)
{
	this->m_pos = position;
	this->m_rot = rotation;
	this->m_scale = scale;
}

Transform::~Transform() {}

glm::mat4 Transform::ModelMatrix(bool checkRotation) const
{
	// Solve the position matrix
	glm::mat4 posMatrix = glm::translate(this->m_pos);

	// Solve the rotation matrix
	glm::mat4 rotXMatrix = glm::rotate(this->m_rot.x, glm::vec3(1, 0, 0));
	glm::mat4 rotYMatrix = glm::rotate(this->m_rot.y, glm::vec3(0, 1, 0));
	glm::mat4 rotZMatrix = glm::rotate(this->m_rot.z, glm::vec3(0, 0, 1));
	glm::mat4 rotMatrix = rotZMatrix * rotYMatrix * rotXMatrix;

	// Solve the scale matrix 
	glm::mat4 scaleMatrix = glm::scale(this->m_scale);

	if (checkRotation)
	{
		return rotMatrix * posMatrix * scaleMatrix;
	}
	else
	{
		return posMatrix * rotMatrix * scaleMatrix;
	}
}