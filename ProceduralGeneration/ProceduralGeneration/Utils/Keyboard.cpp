#include "WindowDisplay.h"
#include "Keyboard.h"

#define TURNING_SPEED 90.0
#define MOVEMENT_SPEED 20.0

Keyboard::Keyboard(WindowDisplay* graphicEngine) {
	// Assign the entity of graphicEngine that the keyboard will use
	this->m_windowEntity = graphicEngine;

	// Ensure we can capture the keys being right
	glfwSetInputMode(graphicEngine->GetWindow(), GLFW_STICKY_KEYS, GL_TRUE);

	// Initilize the array so all the values for every key is false
	for (unsigned int i = 0; i < 350; i++)
	{
		this->currentKeyState[i] = false;
		this->previousKeyState[i] = false;
	}
}

void Keyboard::UpdatePreviousState() {
	for (unsigned int i = 0; i < 350; i++)
	{
		this->previousKeyState[i] = this->currentKeyState[i];
	}
}

void Keyboard::CheckKeyboardInput() {
	for (unsigned int i = 0; i < 350; i++)
	{
		int state = glfwGetKey(this->m_windowEntity->GetWindow(), i);
		if (state == GLFW_PRESS)
		{
			this->currentKeyState[i] = true;
		}
		if (state == GLFW_RELEASE)
		{
			this->currentKeyState[i] = false;
		}
	}
}

void Keyboard::KeyboardAction(/*Generator* generatorEntity*/) {
	this->UpdatePreviousState();
	this->CheckKeyboardInput();

	float turningSpeed = TURNING_SPEED * (this->m_windowEntity->GetTimer()->GetDeltaTime()); //Turning speed over delta time
	float movementSpeed = MOVEMENT_SPEED * (this->m_windowEntity->GetTimer()->GetDeltaTime());

	if (this->GetKeyDown(GLFW_KEY_A))
	{
		Camera::s_camera->GetTransform()->SetRot(Camera::s_camera->GetTransform()->GetRot() + glm::vec3(0.0f, turningSpeed, 0.0f));
	}
	if (this->GetKeyDown(GLFW_KEY_D))
	{
		Camera::s_camera->GetTransform()->SetRot(Camera::s_camera->GetTransform()->GetRot() - glm::vec3(0.0f, turningSpeed, 0.0f));
	}

	if (this->GetKeyDown(GLFW_KEY_W))
	{
		Camera::s_camera->GetTransform()->SetPos(Camera::s_camera->GetTransform()->GetPos() + Camera::s_camera->GetTransform()->GetForwardDirection() * movementSpeed);
	}
	if (this->GetKeyDown(GLFW_KEY_S))
	{
		Camera::s_camera->GetTransform()->SetPos(Camera::s_camera->GetTransform()->GetPos() - Camera::s_camera->GetTransform()->GetForwardDirection() * movementSpeed);
	}
	if (this->GetKeyDown(GLFW_KEY_SPACE))
	{
		Camera::s_camera->GetTransform()->SetRot(Camera::s_camera->GetTransform()->GetRot() + glm::vec3(turningSpeed, 0.0f, 0.0f));

	}
	if (this->GetKeyDown(GLFW_KEY_LEFT_CONTROL))
	{
		Camera::s_camera->GetTransform()->SetRot(Camera::s_camera->GetTransform()->GetRot() - glm::vec3(turningSpeed, 0.0f, 0.0f));
	}
	Camera::s_camera->ResetView();

	if (this->GetKeyRelease(GLFW_KEY_R)) {
		std::vector<GeneratedObjects*> objectListRefresh = this->m_windowEntity->GetListObject();
		GeneratedObjects* holdCloudsObject;

		for (unsigned int i = 0; i < objectListRefresh.size(); i++) {
			if (!objectListRefresh[i]->m_mesh->GetIsTree()) {
				GeneratedData genData;
				std::vector<glm::vec3> genDataTextureMap;

				if (i == 0) {
					genData = Generator(513, -50.0f, 0.1f).DiamondSquareAlgorithmCalculation();
				}
				else if (i == 1) {
					genData = Generator(65, -0.5f, 0.05f).DiamondSquareAlgorithmCalculation();
				}
				else if (i == 2) {
					genData = Generator(65, -2.0f, 0.0f).DiamondSquareAlgorithmCalculation();
					genDataTextureMap = Generator(17, -4.0f, 0.0f).PerlinNoise();
				}

				if (genDataTextureMap.size() > 0) {
					std::vector<glm::vec4> textureColour;
					for (unsigned int i = 0; i < genDataTextureMap.size(); i++) {
						textureColour.push_back(glm::vec4(genDataTextureMap[i].y, 0.0f, 0.0f, 1.0f));
					}

					std::vector<void*> cloudTextureData = { textureColour.data() };
					objectListRefresh[i]->m_texture->SetTextureData(cloudTextureData);
					objectListRefresh[i]->m_texture->LoadTexture();
					objectListRefresh[i]->m_texture->GenerateTexture();
				}

				objectListRefresh[i]->m_mesh->SetVertices(genData.vertexData);
				objectListRefresh[i]->m_mesh->SetIndices(genData.vertexIndices);
				objectListRefresh[i]->m_mesh->ResetArrayBufferData();

				if (i == 2) {
					holdCloudsObject = objectListRefresh[2];
					objectListRefresh.pop_back();
				}
			}
			else {
				objectListRefresh.erase(objectListRefresh.begin() + i, objectListRefresh.end() - 1);
				i--;
			}
		}

		std::vector<Vertex> holdGroundVertices = objectListRefresh[0]->m_mesh->GetVertices();
		GeneratedData holdTreeData = Generator(holdGroundVertices, 1.0f).LSystemPGTrees(3);
		
		for (unsigned int i = 0; i < holdGroundVertices.size(); i += 300) {
			if (holdGroundVertices[i].pos.y >= 18.0f && holdGroundVertices[i].pos.y <= 33.0f) {
				objectListRefresh.push_back(
					new GeneratedObjects(
						new Transform(holdGroundVertices[i].pos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)),
						holdTreeData.vertexData,
						holdTreeData.vertexIndices,
						GL_STATIC_DRAW, "SkyBoxShader", { "Default.jpg" }, 16, true));
			}
		}

		objectListRefresh.push_back(holdCloudsObject);

		this->m_windowEntity->SetNewObjects(objectListRefresh);
	}
}