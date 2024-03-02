#pragma once

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <glad/glad.h>

#include <vector>

#include "Window.hpp"

class UIElement {
public:
	UIElement(std::vector<UIElement*>& elements);
	~UIElement() {};

	virtual void OnUIOjectList() = 0;
};

UIElement::UIElement(std::vector<UIElement*>& elements) {
	elements.push_back(this);
}

namespace UIManager {
	std::vector<UIElement*> uiObjects;

	float viewPortAspectRatio = 1.0f;

	static GLuint frameBuffer;
	static GLuint frameBufferTexture;

	static bool demoWindowOpen = false;

	static bool vieportOpen = true;
	static bool objectListOpen = true;
	static bool consoleOpen = true;

	void InitImGui() {
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

		glGenFramebuffers(1, &frameBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

		glGenTextures(1, &frameBufferTexture);
		glBindTexture(GL_TEXTURE_2D, frameBufferTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 800, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, frameBufferTexture, 0);
	}

	inline void RenderUI() {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, Window::width, Window::height);

		glClear(GL_COLOR_BUFFER_BIT);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::DockSpaceOverViewport();

		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Exit")) { Window::SetWindowSouldClose(true); }
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Windows"))
			{
				if (ImGui::MenuItem("Viewport")) { vieportOpen = !vieportOpen;  }
				if (ImGui::MenuItem("Object List")) { objectListOpen = !objectListOpen; }
				if (ImGui::MenuItem("Console")) { consoleOpen = !consoleOpen; }
				if (ImGui::MenuItem("ImGui Demo Window")) { demoWindowOpen = !demoWindowOpen; }
				
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}

		static ImVec2 wsize;
		if (vieportOpen) {
			ImGui::Begin("Viewport", &vieportOpen);
			ImGui::BeginChild("GameRender");
			
			wsize = ImGui::GetWindowSize();
			viewPortAspectRatio = wsize.x / wsize.y;
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, wsize.x, wsize.y, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
			ImGui::Image((ImTextureID)frameBufferTexture, wsize, ImVec2(0, 1), ImVec2(1, 0));

			ImGui::EndChild();
			ImGui::End();
		}

		if (objectListOpen) {
			ImGui::Begin("Objects", &objectListOpen);
			for (UIElement* element : uiObjects) {
				element->OnUIOjectList();
			}
			ImGui::End();
		}

		if (consoleOpen) {
			ImGui::Begin("Console", &consoleOpen);
			ImGui::End();
		}

		if (demoWindowOpen) {
			ImGui::ShowDemoWindow(&demoWindowOpen);
		}

		ImGui::Render();
		static int display_w, display_h;
		glfwGetFramebufferSize(Window::windowPtr, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		static GLFWwindow* backup_current_context; backup_current_context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backup_current_context);

		glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
		glViewport(0, 0, wsize.x, wsize.y);
	}

	void TerminateImGui() {

		glDeleteTextures(1, &frameBufferTexture);
		glDeleteFramebuffers(1, &frameBuffer);

		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}
}