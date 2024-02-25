#pragma once

#include <glad/glad.h>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.hpp"

#include "Debug.hpp"

class Mesh {
public:
	Mesh(const std::vector<GLfloat>& vertices, const std::vector<GLuint>& indices);
	~Mesh();

	Mesh(const Mesh&) = delete;
	Mesh& operator= (const Mesh&) = delete;

	inline void Bind();
	inline void SendModelMatrix(const Shader& shader) const;

	std::vector<GLfloat> vertices;
	std::vector<GLuint> indices;

private:
	GLuint VertexArrayObject, VertexBufferObject, IndexBufferObject;
};

Mesh::Mesh(const std::vector<GLfloat>& vertices, const std::vector<GLuint>& indices): vertices(vertices), indices(indices) {
	glCreateVertexArrays(1, &VertexArrayObject);
	glCreateBuffers(1, &VertexBufferObject);
	glCreateBuffers(1, &IndexBufferObject);

	glNamedBufferData(VertexBufferObject, sizeof(GLfloat) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
	glNamedBufferData(IndexBufferObject,  sizeof(GLuint)  * indices.size(),  indices.data(),  GL_STATIC_DRAW);

	glEnableVertexArrayAttrib(VertexArrayObject, 0);
	glVertexArrayAttribBinding(VertexArrayObject, 0, 0);
	glVertexArrayAttribFormat(VertexArrayObject, 0, 3, GL_FLOAT, GL_FALSE, 0);

	glVertexArrayVertexBuffer(VertexArrayObject, 0, VertexBufferObject, 0, 3 * sizeof(GLfloat));
	glVertexArrayElementBuffer(VertexArrayObject, IndexBufferObject);

	DEBUGPRINT("Created mesh");
}

Mesh::~Mesh() {
	DEBUGPRINT("Destroyed mesh");
}

void Mesh::Bind() {
	glBindVertexArray(VertexArrayObject);
}

void Mesh::SendModelMatrix(const Shader& shader) const {
	static glm::mat4 model = glm::mat4(1.0f);
	glUniformMatrix4fv(glGetUniformLocation(shader.GetID(), "model"), 1, GL_FALSE, glm::value_ptr(model));
}