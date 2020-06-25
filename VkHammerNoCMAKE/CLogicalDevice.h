#pragma once

#include <vulkan/vulkan.h>

#include <stdexcept>

#include "CInstance.h"

//Create the Logical Device 

class CLogicalDevice
{
private:

	VkQueue graphicQueue;
	VkQueue presentQueue;

public:

	CLogicalDevice();
	~CLogicalDevice();
	
	VkDevice CreateLogicalDevice(VkPhysicalDevice physicalDevice, const std::vector<const char*> deviceExtensionsSwapChain, uint32_t queueFamilyIndex, uint32_t queuePresentFamilyIndex);
	
	VkQueue GetGraphicQueue() {
		return graphicQueue;
	}
	VkQueue GetPresentQueue() {
		return presentQueue;
	}
};