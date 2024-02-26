#pragma once

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <vector>

class UIElement {
public:
	UIElement(std::vector<UIElement*>& elements);
	~UIElement() {};

	virtual void OnUIStart() = 0;
	virtual void OnUIOjectList() = 0;
};

UIElement::UIElement(std::vector<UIElement*>& elements) {
	elements.push_back(this);
}

namespace UIManager {
	std::vector<UIElement*> uiObjects;

	void Init() {
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
	}

	inline void Run() {

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin("Objects");
		for (UIElement* element : uiObjects) {
			element->OnUIOjectList();
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

	void Terminate() {
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}
}