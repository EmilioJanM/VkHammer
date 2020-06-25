#pragma once

#include <GLFW/glfw3.h>

 class Khronos {

 public:

	static float currentFrame;
	static float lastFrame;
	static double time;

	static void InitializeKhronos() {
		currentFrame = glfwGetTime();
	}

	static void UpdateTimeBegin() {
		time = glfwGetTime();
		currentFrame = (time + currentFrame) - lastFrame;
	}

	static void UpdateTimeEnd() {
		lastFrame = currentFrame;
	}

	static float DeltaTime() {
		return currentFrame;
	}

};
