#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "WindowUtil.hpp"
#include "Debug.hpp"

#include <vector>

namespace Window {

	float aspectRatio;
	static GLFWwindow* windowPtr;

	static bool isFullscreen = false;

	void KeyCallback (GLFWwindow* window, int key, int scancode, int action, int mods) {
		if (key == GLFW_KEY_F11 && action == GLFW_PRESS) {
			int monitorcount = 0;
			GLFWmonitor** monitors = glfwGetMonitors(&monitorcount);
			const GLFWvidmode* mode = glfwGetVideoMode(monitors[0]);

			switch (isFullscreen) {
			case false:
				glfwSetWindowMonitor(windowPtr, monitors[0], 0, 0, mode->width, mode->height, mode->refreshRate);
				isFullscreen = !isFullscreen;
				break;
			case true:
				glfwSetWindowMonitor(windowPtr, nullptr, mode->width / 2 - (mode->width / 2) / 2, mode->height / 2 - (mode->height / 2) / 2, mode->width / 2, mode->height / 2, mode->refreshRate);
				isFullscreen = !isFullscreen;
				break;
			}
		}
	}

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

	void Init(const char* name, const std::vector<WindowHint>& hints) {
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

		int monitorcount = 0;
		GLFWmonitor** monitors = glfwGetMonitors(&monitorcount);
		const GLFWvidmode* mode = glfwGetVideoMode(monitors[0]);

		windowPtr = glfwCreateWindow(mode->width / 2, mode->height / 2, name, nullptr, nullptr);
		glfwSetWindowPos(windowPtr, mode->width / 2 - (mode->width / 2) / 2, mode->height / 2 - (mode->height / 2) / 2);

		if (!windowPtr)
			throw std::exception("GLFW faile to create window");

		glfwMakeContextCurrent(windowPtr);

		glfwSetFramebufferSizeCallback(windowPtr, WindowResizeCallback);
		glfwSetKeyCallback(windowPtr, KeyCallback);

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

		aspectRatio = ((float)mode->width / 2) / ((float)mode->height / 2);
		glViewport(0, 0, mode->width / 2, mode->height / 2);
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