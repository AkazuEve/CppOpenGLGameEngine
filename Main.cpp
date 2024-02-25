#include "Window.hpp"
#include "Shader.hpp"
#include "Camera.hpp"
#include "Mesh.hpp"	


unsigned int RENDER_WIDTH = 1280;
unsigned int RENDER_HEIGHT = 720;

std::vector<WindowHint> hints = { WindowHint(GLFW_CONTEXT_VERSION_MAJOR,	4),
								  WindowHint(GLFW_CONTEXT_VERSION_MINOR,	6),
								  WindowHint(GLFW_OPENGL_PROFILE,			GLFW_OPENGL_CORE_PROFILE) };

std::vector<GLfloat> vertices = {
	-0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // Lower left corner
	0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // Lower right corner
	0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f, // Upper corner
	-0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f, // Inner left
	0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f, // Inner right
	0.0f, -0.5f * float(sqrt(3)) / 3, 0.0f // Inner down
};

std::vector<GLuint> indices = {
	0, 3, 5, // Lower left triangle
	3, 2, 4, // Upper triangle
	5, 4, 1 // Lower right triangle
};


int main() {
	Window::Init(RENDER_WIDTH, RENDER_HEIGHT, "Game Window", hints);

	Shader shader("Basic");
	shader.Bind();

	Camera camera;
	camera.position = glm::vec3(0.0f, 0.0f, -2.0f);

	Mesh mesh(vertices, indices);
	mesh.SendModelMatrix(shader);
	mesh.Bind();

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	while (Window::ShouldRunNextFrame()) {
		glClear(GL_COLOR_BUFFER_BIT);

		camera.SendMatrix(shader);

		glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0);
	}

	Window::Terminate();

	return 0;
}