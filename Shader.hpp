#pragma once

#include <glad/glad.h>

#include "Debug.hpp"
#include "Util.hpp"

class Shader {
public:
	Shader(const char* name);
	~Shader();

	Shader(const Shader&) = delete;
	Shader& operator= (const Shader&) = delete;

	inline void Bind() const { glUseProgram(m_ID); DEBUGPRINT("Bound shader " << m_name); }
	inline GLuint GetID() const { return m_ID; }

private:
	GLuint m_ID;
	const char* m_name;
};

Shader::Shader(const char* name): m_name(name) {
	std::string tmpSource;
	const char* tmpChar;

	tmpSource = get_file_contents(std::string("./Resources/Shaders/") + std::string(name) + std::string(".vert"));
	tmpChar = tmpSource.c_str();
	GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vShader, 1, &tmpChar, nullptr);
	glCompileShader(vShader);

	tmpSource = get_file_contents(std::string("./Resources/Shaders/") + std::string(name) + std::string(".frag"));
	tmpChar = tmpSource.c_str();
	GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER); //Damn debug is useful
	glShaderSource(fShader, 1, &tmpChar, nullptr);
	glCompileShader(fShader);

	GLint isCompiled = 0;
	glGetShaderiv(fShader, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(fShader, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<GLchar> errorLog(maxLength);
		glGetShaderInfoLog(fShader, maxLength, &maxLength, &errorLog[0]);

		// Provide the infolog in whatever manor you deem best.
		DEBUGPRINT(errorLog.data());
		// Exit with failure.
		throw std::runtime_error("Shader failed to compile");
	}

	m_ID = glCreateProgram();
	glAttachShader(m_ID, vShader);
	glAttachShader(m_ID, fShader);
	glLinkProgram(m_ID);

	DEBUGPRINT("Created shader " << name);

	glDeleteShader(vShader);
	glDeleteShader(fShader);
}

Shader::~Shader() {
	glDeleteProgram(m_ID);
	DEBUGPRINT("Deleted shader " << m_name);
}