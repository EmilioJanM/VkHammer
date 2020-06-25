#pragma once

#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>

#include "vulkan/vulkan.h"

#include <iostream>
#include <vector>

class Object
{
public:
	struct EntityInfo {
		struct Position
		{
			//Position of the object in each axis
			float x = 0, y = 0, z = 0;
		} position;
		struct Rotation
		{
			//In degrees
			float x = 0, y = 0, z = 0;
		} rotation;
		struct Scale
		{
			float x = 1, y = 1, z = 1;
		} scale;
		int n, distance;
	} transform;

	struct UniformBufferObject {
		alignas(16) glm::mat4 model;
		alignas(16) glm::mat4 view;
		alignas(16) glm::mat4 proj;
		glm::vec3 scale;
		glm::vec4 ucolors;
		float time;
	} UBO;

	std::vector<VkBuffer> uniformBuffers;
	std::vector<VkDeviceMemory> uniformBuffersMemory;

private:

};