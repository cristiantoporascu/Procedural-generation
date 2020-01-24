#pragma once
#include "GeneratedObject.h"

class SkyBox {
private:
	std::vector<GeneratedObjects*> m_skyBoxObject;

public:
	SkyBox(float size);

	/**
	 * Method that deals with all the bindings in order to draw skybox
	 */
	void DrawSkyBox();

	/**
	 * Getters and setters
	 */

public:
	inline std::vector<GeneratedObjects*> GetSkyBoxFaces() const { return this->m_skyBoxObject; }
};