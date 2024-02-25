#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Window.hpp"
#include "Shader.hpp"

#include "Debug.hpp"
#include "Time.hpp"

class Camera {
public:
	Camera();
	~Camera();

	Camera(const Camera&) = delete;
	Camera& operator= (const Camera&) = delete;

	inline void const SendMatrix(const Shader& shader);

	glm::vec3 position = glm::vec3(0.0f);
	float fov = 90.0f;

	float nearPlane = 0.1f;
	float farPlane = 100.0f;

private:
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 proj = glm::mat4(1.0f);

};

Camera::Camera() {
	DEBUGPRINT("Created camera");
}

Camera::~Camera() {
	DEBUGPRINT("Destroyed camera");
}

inline void const Camera::SendMatrix(const Shader& shader) {

	static glm::mat4 identityMatrix(1.0f);

	view = glm::translate(identityMatrix, position);
	proj = glm::perspective(glm::radians(fov), Window::aspectRatio, nearPlane, farPlane);

	glUniformMatrix4fv(glGetUniformLocation(shader.GetID(), "camera"), 1, GL_FALSE, glm::value_ptr(proj * view));
}