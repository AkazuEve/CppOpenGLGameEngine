#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.hpp"

#include "UIManager.hpp"

#include "Debug.hpp"
#include "Time.hpp"

class Camera: public UIElement {
public:
	Camera(const char* name);
	~Camera();

	Camera(const Camera&) = delete;
	Camera& operator= (const Camera&) = delete;

	inline void const CalculateMatrix();
	inline void const SendFullMatrix(const Shader& shader);
	inline void const SendCubemapMatrix(const Shader& shader);

	glm::vec3 position = glm::vec3(0.0f);
	glm::vec3 rotation = glm::vec3(0.0f);
	float fov = 50.0f;

	float nearPlane = 0.1f;
	float farPlane = 100.0f;

	const char* name;

	virtual void OnUIOjectList() override {
		if (ImGui::TreeNode(name)) {
			ImGui::DragFloat3("Position",  &position.x, 0.1f, -100.0f, 100.0f);
			ImGui::DragFloat3("Rotation",  &rotation.x, 0.1f, -360.0f, 360.0f);
			ImGui::DragFloat("Fov",        &fov,        0.1f,  5.0f,   360.0f);
			ImGui::DragFloat("Near plane", &nearPlane,  0.01f, 0.0f,   100.0f);
			ImGui::DragFloat("Far plane",  &farPlane,   0.01f, 50.0f,  1000.0f);

			if (ImGui::Button("Reset")) { position = glm::vec3(0.0f, 0.0f, -2.0f); rotation = glm::vec3(0.0f); }

			ImGui::TreePop();
		}
	};

private:
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 proj = glm::mat4(1.0f);
};

Camera::Camera(const char* name): name(name), UIElement(UIManager::uiCameras) {
	
	DEBUGPRINT("Created camera: " << name);
}

Camera::~Camera() {
	DEBUGPRINT("Destroyed camera");
}

inline void const Camera::CalculateMatrix() {
	static glm::mat4 identityMatrix(1.0f);

	view = glm::rotate(identityMatrix, glm::radians(rotation.x), glm::vec3(0.0f, 1.0f, 0.0f));
	view = glm::rotate(view, glm::radians(rotation.y), glm::vec3(1.0f, 0.0f, 0.0f));
	view = glm::rotate(view, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	view = glm::translate(view, position);
	proj = glm::perspective(glm::radians(fov), UIManager::viewPortAspectRatio, nearPlane, farPlane);
}

inline void const Camera::SendFullMatrix(const Shader& shader) {
	glUniformMatrix4fv(glGetUniformLocation(shader.GetID(), "camera"), 1, GL_FALSE, glm::value_ptr(proj * view));
}

inline void const Camera::SendCubemapMatrix(const Shader& shader) {
	static glm::mat4 cubemap;
	cubemap = proj * view;
	cubemap[3] *= glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	glUniformMatrix4fv(glGetUniformLocation(shader.GetID(), "camera"), 1, GL_FALSE, glm::value_ptr(cubemap));
}