#include "GeneratedObject.h"

GeneratedObjects::GeneratedObjects()
{
	this->m_transform = new Transform();
	this->m_shader = new Shader("GroundShader");
	this->m_mesh = new Mesh();
}

GeneratedObjects::GeneratedObjects(
	Transform* transform, 
	std::vector<Vertex> vertices, 
	std::vector<unsigned int> indices,
	GLenum movementState, 
	std::string shaderFileName,
	std::vector<std::string> textureFileName,
	int textureSize,
	bool isTree
)
{
	this->m_transform = transform;
	this->m_shader = new Shader(shaderFileName);
	this->m_mesh = new Mesh(vertices, indices, movementState, isTree);
	this->m_texture = new Texture(textureFileName, textureSize, textureSize);
}

GeneratedObjects::GeneratedObjects(
	Transform* transform,
	std::vector<Vertex> vertices,
	std::vector<unsigned int> indices,
	GLenum movementState,
	std::vector<void*> textureData,
	std::string shaderFileName,
	int textureSize,
	bool isTree
)
{
	this->m_transform = transform;
	this->m_shader = new Shader(shaderFileName);
	this->m_mesh = new Mesh(vertices, indices, movementState, isTree);
	this->m_texture = new Texture(textureData, textureSize, textureSize);
}

GeneratedObjects::GeneratedObjects(
	Transform* transform,
	Mesh* mesh,
	std::string shaderFileName,
	std::vector<std::string> textureFileName,
	int textureSize
) {
	this->m_transform = transform;
	this->m_mesh = mesh;
	this->m_shader = new Shader(shaderFileName);
	this->m_texture = new Texture(textureFileName, textureSize, textureSize);
}

GeneratedObjects::GeneratedObjects(
	Transform* transform,
	Mesh* mesh,
	std::vector<void*> textureData,
	std::string shaderFileName,
	int textureSize
) {
	this->m_transform = transform;
	this->m_mesh = mesh;
	this->m_shader = new Shader(shaderFileName);
	this->m_texture = new Texture(textureData, textureSize, textureSize);
}

void GeneratedObjects::DrawScene(float deltaTime)
{
	this->m_shader->BindShader();
	this->m_shader->UpdateShader(*this->m_transform, *Camera::s_camera, 0);
	this->m_texture->BindTexture(this->m_shader, deltaTime);
	this->m_mesh->DrawMesh();
}

void GeneratedObjects::Start() {
	return;
}

void GeneratedObjects::Update() {
	return;
}

GeneratedObjects::~GeneratedObjects() {
	delete this->m_mesh;
	delete this->m_shader;
	delete this->m_texture;
	delete this->m_transform;
}