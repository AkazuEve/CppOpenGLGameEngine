#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "WindowUtil.hpp"
#include "Debug.hpp"

#include <vector>

namespace Window {

	float aspectRatio;
	GLFWwindow* windowPtr;

	void WindowResizeCallback(GLFWwindow* window, int width, int height) {
		glViewport(0, 0, width, height);
		if(width == 0 && height == 0) {
			aspectRatio = 1.0f;
			DEBUGPRINT("Window was minimized, avoiding explosions");
		}
		else {
			aspectRatio = (float)width / (float)height;
			DEBUGPRINT("Resolution changed width: " << width << " height: " << height << " aspect ratio: " << aspectRatio);
		}
	}

	void Init(unsigned int& width, unsigned int& height, const char* name, const std::vector<WindowHint>& hints) {
		switch (glfwInit()) {
		case GLFW_TRUE:
			DEBUGPRINT("GLFW initailized");
			break;
		case GLFW_FALSE:
			throw std::runtime_error("Failed to init GLFW");
		}

		for (WindowHint hint : hints) {
			glfwWindowHint(hint.windowArgument, hint.windowArgumentValue);
		}

		DEBUGFUNC(glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE));

		int monitorCount;
		GLFWmonitor** monitors = glfwGetMonitors(&monitorCount);

		if (monitorCount > 1) {
			windowPtr = glfwCreateWindow(width, height, name, monitors[2], nullptr);
		}
		else {
			windowPtr = glfwCreateWindow(width, height, name, nullptr, nullptr);
		}

		if (!windowPtr)
			throw std::exception("GLFW faile to create window");

		glfwMakeContextCurrent(windowPtr);

		glfwSetFramebufferSizeCallback(windowPtr, WindowResizeCallback);

		switch (gladLoadGL())
		{
		case true:
			DEBUGPRINT("Glad loaded");
			break;
		case false:
			throw std::runtime_error("Failed to load glad");
			break;
		}

		DEBUGFUNC(
			glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(debug::glDebugOutput, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
		DEBUGPRINT("OpenGL Debug Enabled");
		);

		aspectRatio = (float)width / (float)height;
		glViewport(0, 0, width, height);
	}

	void Terminate() {
		glfwDestroyWindow(windowPtr);
		DEBUGPRINT("Window destroyed");
		glfwTerminate();
		DEBUGPRINT("GLFW terminated");
	}

	inline bool ShouldRunNextFrame() {
		glfwPollEvents();
		glfwSwapBuffers(windowPtr);

		if (glfwGetKey(windowPtr, GLFW_KEY_ESCAPE)) {
			glfwSetWindowShouldClose(windowPtr, true);
		}

		return (!glfwWindowShouldClose(windowPtr));
	}
}