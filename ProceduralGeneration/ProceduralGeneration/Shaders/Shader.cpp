#include "Shader.h"

Shader::Shader(std::string shaderFileName)
{
	// Create a shader program
	this->m_programId = glCreateProgram();

	// Attach a vertex shader to the m_shader[0]
	this->m_shaders[0] = CreateShader(LoadShader("Resources/Shaders/Vertex" + shaderFileName + ".glsl"), GL_VERTEX_SHADER);

	// Check compile test for vertexshader
	Shader::CheckCompileShader(this->m_shaders[0]);

	// Attach a fragment shader to the m_shader[1]
	this->m_shaders[1] = CreateShader(LoadShader("Resources/Shaders/Fragment" + shaderFileName + ".glsl"), GL_FRAGMENT_SHADER);

	// Check compile test for fragment shader
	Shader::CheckCompileShader(this->m_shaders[1]);

	// Attach shader to the shader program for both of the shaders
	for (unsigned int i = 0; i < Shader::NUMBER_SHADER; i++)
	{
		glAttachShader(this->m_programId, this->m_shaders[i]);
	}

	// Link the shaders with the final program
	glLinkProgram(this->m_programId);

	// Validate all the shaders in order to compile with the program
	glValidateProgram(this->m_programId);

	// Give acces to the transform uniform in vertex shader
	this->m_uniforms[VIEW_U] = glGetUniformLocation(this->m_programId, "viewMatrix");
	this->m_uniforms[PROJECTION_U] = glGetUniformLocation(this->m_programId, "projectionMatrix");
	this->m_uniforms[MODEL_U] = glGetUniformLocation(this->m_programId, "modelMatrix");
	this->m_uniforms[CAMERA_POS] = glGetUniformLocation(this->m_programId, "cameraPos");
}

Shader::~Shader()
{
	for (unsigned int i = 0; i < Shader::NUMBER_SHADER; i++)
	{
		glDetachShader(this->m_programId, this->m_shaders[i]);
		glDeleteShader(this->m_shaders[i]);
	}
	glDeleteProgram(this->m_programId);
}

void Shader::BindShader()
{
	glUseProgram(this->m_programId);
}

void Shader::UpdateShader(const Transform& transform, Camera& camera, bool checkRotation)
{
	glUniformMatrix4fv(this->m_uniforms[VIEW_U], 1, GL_FALSE, &camera.GetView()[0][0]);
	glUniformMatrix4fv(this->m_uniforms[PROJECTION_U], 1, GL_FALSE, &camera.GetProjection()[0][0]);
	glUniformMatrix4fv(this->m_uniforms[MODEL_U], 1, GL_FALSE, &transform.ModelMatrix(checkRotation)[0][0]);
	glUniform3fv(this->m_uniforms[CAMERA_POS], 1, glm::value_ptr(camera.GetTransform()->GetPos()));
}

std::string Shader::LoadShader(const std::string& fileName)
{
	std::ifstream file;
	file.open((fileName).c_str());

	std::string output;
	std::string line;

	if (file.is_open())
	{
		while (file.good())
		{
			getline(file, line);
			output.append(line + "\n");
		}
	}
	else
	{
		std::cout << "ERROR: Cannot Open the Shader! -> " << fileName << std::endl;
	}

	return output;
}

GLuint Shader::CreateShader(const std::string& text, GLenum shaderType)
{
	// Create a shader
	GLuint shader = glCreateShader(shaderType);

	const GLchar* shaderStringSource[1];
	GLint shaderStringSourceLength[1];
	shaderStringSource[0] = text.c_str();
	shaderStringSourceLength[0] = text.length();

	// Import the shader from the source
	glShaderSource(shader, 1, shaderStringSource, shaderStringSourceLength);

	// Get the shader and compile it
	glCompileShader(shader);

	return shader;
}

void Shader::CheckCompileShader(GLuint shader)
{
	GLint result = GL_FALSE;
	int logLength;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
	std::vector<GLchar> vertShaderError((logLength > 1) ? logLength : 1);
	glGetShaderInfoLog(shader, logLength, NULL, &vertShaderError[0]);
	if (result == GL_FALSE) {
		std::cout << &vertShaderError[0] << std::endl;
	}
}