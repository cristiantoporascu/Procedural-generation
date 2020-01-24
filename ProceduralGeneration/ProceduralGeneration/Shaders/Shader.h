#pragma once
#include "../Utils/Camera.h"
#include "../Mathematics/Transform.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <fstream>
#include <iostream>
#include <string>

class Shader
{
private:
	enum
	{
		VIEW_U,
		PROJECTION_U,
		MODEL_U,
		CAMERA_POS,

		NUMBER_UNIFORMS
	};

	static const unsigned int NUMBER_SHADER = 2;
	GLuint m_shaders[NUMBER_SHADER];
	GLuint m_uniforms[NUMBER_UNIFORMS];
	GLuint m_programId;

public:
	Shader(std::string shaderFileName);
	~Shader();

	/**
	 * Use the program and binds it from the programId
	 */
	void BindShader();

	/**
	 * Updates the shader according with the transform matrix
	 * @param transform			value of each object used as position info
	 * @param camera			camera position used for ray direction on the shader
	 * @param checkRotation		rotation axis ( false => own axis, true => origin )
	 */
	void UpdateShader(const Transform& transform, Camera& camera, bool checkRotation);

	/**
	 * Load the shader from a GLSL file
	 * @param fileName			name of the file to be imported
	 * @return string			the content of the file as a string
	 */
	static std::string LoadShader(const std::string& fileName);

	/**
	 * Creates and compiles the shader
	 * @param text				loaded shader from the external file
	 * @param shaderType		a GLenum that defined the type of the shader vertex/fragment
	 * @return GLuint			an integer which hold the referrence to the shader
	 */
	static GLuint CreateShader(const std::string& text, GLenum shaderType);

	/**
	 * Check for the copilation of the shader
	 * @param shader			referrence number of the shader
	 */
	static void CheckCompileShader(GLuint shader);

	

	/**
	 * Getters and Setters
	 */
public:
	inline GLuint GetProgramID() const { return this->m_programId; }
	inline void SetProgramID(GLuint newProgramID) { this->m_programId = newProgramID; }
};
