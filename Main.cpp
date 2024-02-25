#include "Window.hpp"
#include "Shader.hpp"
#include "Camera.hpp"
#include "Mesh.hpp"	

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

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
	Window::Init("Game Window", hints);

	Shader shader("Basic");
	shader.Bind();

	Camera camera;
	camera.position = glm::vec3(0.0f, 0.0f, -2.0f);

	Mesh mesh(vertices, indices);
	mesh.SendModelMatrix(shader);
	mesh.Bind();

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows

	io.ConfigViewportsNoTaskBarIcon = true;

	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(Window::windowPtr, true);
	ImGui_ImplOpenGL3_Init("#version 460");


	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	while (Window::ShouldRunNextFrame()) {
		glClear(GL_COLOR_BUFFER_BIT);

		camera.SendMatrix(shader);
		mesh.SendModelMatrix(shader);

		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mesh.indices.size()), GL_UNSIGNED_INT, 0);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin("Objects");
			if (ImGui::TreeNode("Camera")) {
				ImGui::DragFloat3("Position",  &camera.position.x, 0.1f, -100.0f, 100.0f);
				ImGui::DragFloat3("Rotation",  &camera.rotation.x, 0.1f, -180.0f, 180.0f);
				ImGui::DragFloat("Fov",        &camera.fov,        0.1f,    5.0f, 360.0f);
				ImGui::DragFloat("Near plane", &camera.nearPlane,  0.01f,   0.0f, 100.0f);
				ImGui::DragFloat("Far plane",  &camera.farPlane,   0.01f,  50.0f, 1000.0f);
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("Triangles")) {
				ImGui::DragFloat3("Position", &mesh.position.x, 0.1f, -100.0f, 100.0f);
				ImGui::DragFloat3("Rotation", &mesh.rotation.x, 0.1f, -180.0f, 180.0f);
				ImGui::DragFloat3("Scale",    &mesh.scale.x,       0.1f, -50.0f, 50.0f);
				ImGui::TreePop();
			}
		ImGui::End();

		ImGui::Render();
		static int display_w, display_h;
		glfwGetFramebufferSize(Window::windowPtr, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		static GLFWwindow* backup_current_context; backup_current_context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backup_current_context);
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	Window::Terminate();

	return 0;
}