#pragma once

#include <iostream>

#include "vulkan/vulkan.h"

#include "Object.h"
#include "Camera.h"

class Behaviour
{
public:
	Behaviour();
	~Behaviour();

	virtual void Start() {
		std::cout << "Entity Start" << std::endl;
	}
	virtual void Update(VkDevice logicalDevice, uint32_t currentImage, Camera c, float time) {
		std::cout << "Entity Update" << std::endl;
	}
	
};

