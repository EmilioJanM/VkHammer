#pragma once

#include "vulkan/vulkan.h"

class VkHammerStruct
{
public:
	static struct VkHammerInfo
	{
		static VkDevice logicalDevice;
		static VkPhysicalDevice physicalDevice;
	};
}