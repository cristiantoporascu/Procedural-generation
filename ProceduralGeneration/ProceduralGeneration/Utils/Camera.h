#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include "../Mathematics/Transform.h"

class Camera
{
private:
	glm::mat4 m_perspective;
	glm::mat4 m_view;
	Transform* m_cameraTransform;

public:
	/**
	 * Singletone for the camera to be accesable from everywhere
	 */
	static Camera* s_camera;

	Camera();
	Camera(const glm::vec3& pos, const glm::vec3& rot, float fov, float aspect, float zNear, float zFar);
	~Camera();

	/**
	 * Getters and setters
	 */
public:
	inline Transform* GetTransform() const { return this->m_cameraTransform; }
	inline glm::mat4 GetProjection() const { return this->m_perspective; }
	inline glm::mat4 GetView() const { return this->m_view; }

	inline void SetTransform(Transform* newTransform) { this->m_cameraTransform = newTransform; }
	inline void SetProjection(glm::mat4 newProj) { this->m_perspective = newProj; }
	inline void SetView(glm::mat4 newView) { this->m_view = newView; }

	inline void ResetView() {
		this->m_view = this->m_view = glm::lookAt(this->m_cameraTransform->GetPos(),
			this->m_cameraTransform->GetPos() + this->m_cameraTransform->GetForwardDirection(),
			this->m_cameraTransform->GetUpDirection());
	}
};