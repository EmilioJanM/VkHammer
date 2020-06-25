#pragma once

#include <iostream>
#include <vector>

#include "vulkan/vulkan.h"

class CImageView
{

public:
	CImageView();
	~CImageView();

	std::vector<VkImageView> CreateImageViews(VkDevice logicalDevice, std::vector<VkImage> swapChainImages, VkFormat swapChainImageFormat);
	VkImageView CreateImageView(VkDevice logicalDevice, VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);

};

				