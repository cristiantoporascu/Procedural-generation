#pragma once
#include "../Shaders/Shader.h"
#include <string>
#include <glad/glad.h>
#include <vector>

class Texture
{
private:
	std::vector<void*> m_imageDate;
	std::vector<std::string> m_textureNames;
	int m_textureWidth;
	int m_textureHeight;

	std::vector<GLuint> m_textures;

public:
	Texture(const std::vector<std::string>& fileName, int width, int height);
	Texture(const std::vector<void*>& data, int width, int height);
	~Texture();

	/**
	 * Generate texture to the VAO texture address through specific filePath
	 */
	void GenerateTexture();

	/**
	 * Load texture from the generated texture data
	 */
	void LoadTexture();

	/**
	 * Bind specific texture to the VAO while computing a certain object/s
	 * @param currentShader						The current shader used by this object
	 * @param deltaTime							Value used to shift the texture offset and animate it
	 */
	void BindTexture(Shader* currentShader, float deltaTime);

	/**
	 * Getters and setters
	 */
public:
	inline std::vector<std::string> GetTextureName() const { return this->m_textureNames; }

	inline void SetTextureName(std::vector<std::string> newTexturePath) { this->m_textureNames = newTexturePath; }
	inline void SetTextureData(std::vector<void*> newTextureData) { this->m_imageDate = newTextureData; }
};