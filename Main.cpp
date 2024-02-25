#include "Window.hpp"
#include "Shader.hpp"
#include "Camera.hpp"

unsigned int RENDER_WIDTH = 1920;
unsigned int RENDER_HEIGHT = 1080;

std::vector<WindowHint> hints = { WindowHint(GLFW_CONTEXT_VERSION_MAJOR,	4),
								  WindowHint(GLFW_CONTEXT_VERSION_MINOR,	6),
								  WindowHint(GLFW_OPENGL_PROFILE,			GLFW_OPENGL_CORE_PROFILE) };

GLfloat vertices[] = {
	-0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // Lower left corner
	 0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // Lower right corner
	 0.0f,  0.5f * float(sqrt(3)) * 2 / 3, 0.0f // Upper corner
};

int main() {
	Window::Init(RENDER_WIDTH, RENDER_HEIGHT, "Game Window", hints);

	Shader shader("Basic");
	shader.Bind();

	Camera camera;
	camera.position = glm::vec3(0.0f, 0.0f, -2.0f);

	#pragma region MODEL

	glm::mat4 model = glm::mat4(1.0f);

	glUniformMatrix4fv(glGetUniformLocation(shader.GetID(), "model"), 1, GL_FALSE, glm::value_ptr(model));

	GLuint VertexArrayObject, VertexBufferObject;

	glGenVertexArrays(1, &VertexArrayObject);
	glGenBuffers(1, &VertexBufferObject);

	glBindVertexArray(VertexArrayObject);

	glBindBuffer(GL_ARRAY_BUFFER, VertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);
	#pragma endregion

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	while (Window::ShouldRunNextFrame()) {
		glClear(GL_COLOR_BUFFER_BIT);

		camera.SendMatrix(shader);

		glDrawArrays(GL_TRIANGLES, 0, 3);
	}

	return 0;
}