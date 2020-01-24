#include "SkyBox.h"

SkyBox::SkyBox(float size) {
	Transform* newSkyBoxTransform = new Transform(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));

	std::vector<unsigned int> indices = {
		0, 3, 1, 2
	};

	std::vector<Vertex> verticesFRONT =
	{
		Vertex(glm::vec3(size, -size, size), glm::vec2(0.0,0.0), glm::vec3(0.0, 1.0, 0.0), glm::vec3(1.0,1.0,1.0)),
		Vertex(glm::vec3(size, size, size), glm::vec2(0.0,1.0), glm::vec3(0.0, 1.0, 0.0), glm::vec3(1.0,1.0,1.0)),
		Vertex(glm::vec3(-size, size, size), glm::vec2(1.0,1.0), glm::vec3(0.0, 1.0, 0.0), glm::vec3(1.0,1.0,1.0)),
		Vertex(glm::vec3(-size, -size, size), glm::vec2(1.0,0.0), glm::vec3(0.0, 1.0, 0.0), glm::vec3(1.0,1.0,1.0))
	};

	std::vector<Vertex> verticesBACK =
	{
		Vertex(glm::vec3(-size, -size, -size), glm::vec2(0.0,0.0), glm::vec3(0.0, 1.0, 0.0), glm::vec3(1.0,1.0,1.0)),
		Vertex(glm::vec3(-size, size, -size), glm::vec2(0.0,1.0), glm::vec3(0.0, 1.0, 0.0), glm::vec3(1.0,1.0,1.0)),
		Vertex(glm::vec3(size, size, -size), glm::vec2(1.0,1.0), glm::vec3(0.0, 1.0, 0.0), glm::vec3(1.0,1.0,1.0)),
		Vertex(glm::vec3(size, -size, -size), glm::vec2(1.0,0.0), glm::vec3(0.0, 1.0, 0.0), glm::vec3(1.0,1.0,1.0))
	};

	std::vector<Vertex> verticesTOP =
	{
		Vertex(glm::vec3(size, size, size), glm::vec2(0.0,0.0), glm::vec3(0.0, 1.0, 0.0), glm::vec3(1.0,1.0,1.0)),
		Vertex(glm::vec3(size, size, -size), glm::vec2(0.0,1.0), glm::vec3(0.0, 1.0, 0.0), glm::vec3(1.0,1.0,1.0)),
		Vertex(glm::vec3(-size, size, -size), glm::vec2(1.0,1.0), glm::vec3(0.0, 1.0, 0.0), glm::vec3(1.0,1.0,1.0)),
		Vertex(glm::vec3(-size, size, size), glm::vec2(1.0,0.0), glm::vec3(0.0, 1.0, 0.0), glm::vec3(1.0,1.0,1.0))
	};

	std::vector<Vertex> verticesBOTTOM =
	{
		Vertex(glm::vec3(size, -size, -size), glm::vec2(0.0,0.0), glm::vec3(0.0, 1.0, 0.0), glm::vec3(1.0,1.0,1.0)),
		Vertex(glm::vec3(size, -size, size), glm::vec2(0.0,1.0), glm::vec3(0.0, 1.0, 0.0), glm::vec3(1.0,1.0,1.0)),
		Vertex(glm::vec3(-size, -size, size), glm::vec2(1.0,1.0), glm::vec3(0.0, 1.0, 0.0), glm::vec3(1.0,1.0,1.0)),
		Vertex(glm::vec3(-size, -size, -size), glm::vec2(1.0,0.0), glm::vec3(0.0, 1.0, 0.0), glm::vec3(1.0,1.0,1.0)),
	};

	std::vector<Vertex> verticesRIGHT =
	{
		Vertex(glm::vec3(size, -size, -size), glm::vec2(0.0,0.0), glm::vec3(0.0, 1.0, 0.0), glm::vec3(1.0,1.0,1.0)),
		Vertex(glm::vec3(size, size, -size), glm::vec2(0.0,1.0), glm::vec3(0.0, 1.0, 0.0), glm::vec3(1.0,1.0,1.0)),
		Vertex(glm::vec3(size, size, size), glm::vec2(1.0,1.0), glm::vec3(0.0, 1.0, 0.0), glm::vec3(1.0,1.0,1.0)),
		Vertex(glm::vec3(size, -size, size), glm::vec2(1.0,0.0), glm::vec3(0.0, 1.0, 0.0), glm::vec3(1.0,1.0,1.0))
	};

	std::vector<Vertex> verticesLEFT =
	{
		Vertex(glm::vec3(-size, -size, size), glm::vec2(0.0,0.0), glm::vec3(0.0, 1.0, 0.0), glm::vec3(1.0,1.0,1.0)),
		Vertex(glm::vec3(-size, size, size), glm::vec2(0.0,1.0), glm::vec3(0.0, 1.0, 0.0), glm::vec3(1.0,1.0,1.0)),
		Vertex(glm::vec3(-size, size, -size), glm::vec2(1.0,1.0), glm::vec3(0.0, 1.0, 0.0), glm::vec3(1.0,1.0,1.0)),
		Vertex(glm::vec3(-size, -size, -size), glm::vec2(1.0,0.0), glm::vec3(0.0, 1.0, 0.0), glm::vec3(1.0,1.0,1.0))
	};

	this->m_skyBoxObject.push_back(new GeneratedObjects(newSkyBoxTransform, verticesFRONT, indices, GL_DYNAMIC_DRAW, "SkyBoxShader", { "SkyBox/Front.bmp" }, 2048));
	this->m_skyBoxObject.push_back(new GeneratedObjects(newSkyBoxTransform, verticesBACK, indices, GL_DYNAMIC_DRAW, "SkyBoxShader", { "SkyBox/Back.bmp" }, 2048));
	this->m_skyBoxObject.push_back(new GeneratedObjects(newSkyBoxTransform, verticesTOP, indices, GL_DYNAMIC_DRAW, "SkyBoxShader", { "SkyBox/Top.bmp" }, 2048));
	this->m_skyBoxObject.push_back(new GeneratedObjects(newSkyBoxTransform, verticesBOTTOM, indices, GL_DYNAMIC_DRAW, "SkyBoxShader", { "SkyBox/Bottom.bmp" }, 2048));
	this->m_skyBoxObject.push_back(new GeneratedObjects(newSkyBoxTransform, verticesRIGHT, indices, GL_DYNAMIC_DRAW, "SkyBoxShader", { "SkyBox/Right.bmp" }, 2048));
	this->m_skyBoxObject.push_back(new GeneratedObjects(newSkyBoxTransform, verticesLEFT, indices, GL_DYNAMIC_DRAW, "SkyBoxShader", { "SkyBox/Left.bmp" }, 2048));
}

void SkyBox::DrawSkyBox() {
	glm::vec3 currentCamPos = Camera::s_camera->GetTransform()->GetPos();
	for (unsigned int i = 0; i < this->m_skyBoxObject.size(); i++) {
		if (this->m_skyBoxObject[i]->m_transform->GetPos() 
			!= currentCamPos) {
			this->m_skyBoxObject[i]->m_transform->SetPos(currentCamPos);
		}
		this->m_skyBoxObject[i]->DrawScene();
	}
}