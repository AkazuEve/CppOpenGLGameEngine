#pragma once

#include <glad/glad.h>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.hpp"
#include "UIManager.hpp"

#include "Debug.hpp"

class Mesh: public UIElement{
public:
	Mesh(const char* name, const std::vector<GLfloat>& vertices, const std::vector<GLuint>& indices);
	~Mesh();

	Mesh(const Mesh&) = delete;
	Mesh& operator= (const Mesh&) = delete;

	inline void Bind();
	inline void SendModelMatrix(const Shader& shader);

	virtual void OnUIOjectList() override {
		if (ImGui::TreeNode(name)) {
			ImGui::DragFloat3("Position", &position.x, 0.1f, -100.0f, 100.0f);
			ImGui::DragFloat3("Rotation", &rotation.x, 0.1f, -180.0f, 180.0f);
			ImGui::DragFloat3("Scale",    &scale.x,    0.1f, -50.0f,  50.0f);
			if (ImGui::Button("Reset")) { position = glm::vec3(0.0f); rotation = glm::vec3(0.0f); scale = glm::vec3(1.0f); }

			ImGui::TreePop();
		}
	};

	std::vector<GLfloat> vertices;
	std::vector<GLuint> indices;

	glm::vec3 position = glm::vec3(0.0f);
	glm::vec3 rotation = glm::vec3(0.0f);
	glm::vec3 scale = glm::vec3(1.0f);

private:
	const char* name;

	glm::mat4 model = glm::mat4(1.0f);

	GLuint VertexArrayObject, VertexBufferObject, IndexBufferObject;
};

Mesh::Mesh(const char* name, const std::vector<GLfloat>& vertices, const std::vector<GLuint>& indices):name(name), vertices(vertices), indices(indices), UIElement(UIManager::uiObjects) {
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

inline void Mesh::SendModelMatrix(const Shader& shader) {
	static glm::mat4 identityMatrix(1.0f);

	model = glm::translate(identityMatrix, position);
	model = glm::rotate(model, rotation.x, glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, rotation.y, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, scale);

	glUniformMatrix4fv(glGetUniformLocation(shader.GetID(), "model"), 1, GL_FALSE, glm::value_ptr(model));
}