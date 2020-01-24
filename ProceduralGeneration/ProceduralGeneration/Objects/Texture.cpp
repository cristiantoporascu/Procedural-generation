#include "Texture.h"
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include <imgLoader/stb_image.h>

#include <algorithm>

Texture::Texture(const std::vector<std::string>& fileName, int width, int height)
{
	this->m_textureNames = fileName;
	this->m_textureWidth = width;
	this->m_textureHeight = height;

	this->LoadTexture();
	this->GenerateTexture();
}

Texture::Texture(const std::vector<void*>& data, int width, int height) {
	this->m_textureWidth = width;
	this->m_textureHeight = height;
	this->m_imageDate = data;

	this->LoadTexture();
	this->GenerateTexture();
}

Texture::~Texture() {
	for (unsigned int i = 0; i < this->m_imageDate.size(); i++) {
		delete this->m_imageDate[i];
	}
	this->m_imageDate.clear();
}

void Texture::LoadTexture() {
	if (this->m_textureNames.size() > 0) {
		for (unsigned int i = 0; i < this->m_textureNames.size(); i++) {
			std::string buildName = "Resources/Textures/" + this->m_textureNames[i];
			int bits; // BitsPerPixels
			void* image = stbi_load(buildName.c_str(), &this->m_textureWidth, &this->m_textureHeight, &bits, 4);

			if (image == NULL) {
				std::cout << "ERROR: Texture failed to load!" << std::endl;
			}

			this->m_imageDate.push_back(image);
		}
	}
	else {
		std::vector<void*> refreshImageData;

		// Copy RGB data from input to output bitmap files, set output A to 1.
		for (unsigned int i = 0; i < this->m_imageDate.size(); i++)
		{
			float* convertImage = new float[this->m_textureWidth * this->m_textureHeight * 4];

			for (unsigned int v = 0; v < this->m_textureWidth * this->m_textureHeight; v++) {
				glm::vec4 tempReferrenceVertex = *((glm::vec4*)this->m_imageDate[i] + v);

				convertImage[v * 4] = (float)tempReferrenceVertex.r;
				convertImage[v * 4 + 1] = (float)tempReferrenceVertex.g;
				convertImage[v * 4 + 2] = (float)tempReferrenceVertex.b;
				convertImage[v * 4 + 3] = (float)tempReferrenceVertex.a;
			}

			refreshImageData.push_back(&convertImage[0]);

			// Save result to a PPM image
			/*std::ofstream ofs("./untitled.ppm", std::ios::out | std::ios::binary);
			ofs << "P6\n" << this->m_textureWidth << " " << this->m_textureHeight << "\n255\n";
			for (unsigned x = 0; x < this->m_textureWidth * this->m_textureHeight; ++x)
			{
				ofs << (unsigned char)(std::min((float)1, (float)convertImage[x * 4]) * 255) <<
					(unsigned char)(std::min((float)1, (float)convertImage[x * 4 + 1]) * 255) <<
					(unsigned char)(std::min((float)1, (float)convertImage[x * 4 + 2]) * 255);
			}
			ofs.close();*/
		}
		
		this->m_imageDate = refreshImageData;
	}
}

void Texture::GenerateTexture() {
	stbi_set_flip_vertically_on_load(1);

	for (unsigned int i = 0; i < this->m_imageDate.size(); i++) {
		this->m_textures.push_back(0);

		glGenTextures(1, &m_textures[i]);
		glBindTexture(GL_TEXTURE_2D, this->m_textures[i]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);

		if (this->m_textureNames.size() > 0) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->m_textureWidth, this->m_textureHeight, 0,
				GL_RGBA, GL_UNSIGNED_BYTE, this->m_imageDate[i]);
			stbi_image_free(this->m_imageDate[i]);
		}
		else {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->m_textureWidth, this->m_textureHeight, 0,
				GL_RGBA, GL_FLOAT, this->m_imageDate[i]);
			delete this->m_imageDate[i];
		}
	
		glGenerateMipmap(GL_TEXTURE_2D);
	}
}

void Texture::BindTexture(Shader* currentShader, float deltaTime)
{
	for (unsigned int i = 0; i < this->m_textures.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, this->m_textures[i]);
	}
	if (this->m_textures.size() > 1) {
		glUniform1iv(glGetUniformLocation(currentShader->GetProgramID(), "mainTexture"), this->m_textures.size(), (GLint*)&this->m_textures);
		glUniform1iv(glGetUniformLocation(currentShader->GetProgramID(), "layeredTexture"), this->m_textures.size(), (GLint*)&this->m_textures[0]);
	}
	glUniform1f(glGetUniformLocation(currentShader->GetProgramID(), "timer"), deltaTime);
	glUniform1f(glGetUniformLocation(currentShader->GetProgramID(), "textureSize"), this->m_textureWidth);
}