#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

class Transform
{
private:
	glm::vec3 m_pos;
	glm::vec3 m_rot;
	glm::vec3 m_scale;

public:
	Transform();
	Transform(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale);
	~Transform();

	/**
	 * Get model view matrix of the rotation
	 * @param checkRotation			rotation axis ( false => own axis, true => origin )
	 * @return glm::mat4			model view matrix rotation based
	 */
	glm::mat4 ModelMatrix(bool checkRotation) const;

	/**
	 * Getters and setters
	 */
public:
	inline glm::vec3 GetPos() const { return this->m_pos; }
	inline glm::vec3 GetRot() const { return this->m_rot; }
	inline glm::vec3 GetScale() const { return this->m_scale; }

	inline void SetPos(glm::vec3 newPos) { this->m_pos = newPos; }
	inline void SetRot(glm::vec3 newRot) { this->m_rot = newRot; }
	inline void SetScale(glm::vec3 newScale) { this->m_scale = newScale; }

	inline glm::vec3 GetForwardDirection() const {
		return glm::normalize(glm::vec3(
			sin(glm::radians(this->m_rot.y)),
			tan(glm::radians(this->m_rot.x)),
			cos(glm::radians(this->m_rot.y))
		));
	}

	inline glm::vec3 GetRightDirection() const { 
		return glm::cross(GetForwardDirection(), glm::vec3(0, 1, 0)); 
	}
	inline glm::vec3 GetUpDirection() const {
		return glm::cross(GetForwardDirection(), -GetRightDirection());
	}
};
