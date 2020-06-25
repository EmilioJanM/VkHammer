#pragma once

#include <iostream>
#include <vulkan/vulkan.h>

//#include "BuffersManagment.h"

#include "stb_image.h"

class ImageLoader
{
public:
	ImageLoader();
	~ImageLoader();

	static void CreateTextureImage(VkDevice logicalDevice, VkPhysicalDevice physicalDevice, VkCommandPool commandPool, VkQueue graphicsQueue, VkImage& textureImage, VkDeviceMemory& textureImageMemory, std::string texture_path);
	static void CreateTextureImageView(VkDevice logicalDevice, VkImage textureImage, VkImageView &textureImageView);
	static void CreateTextureSampler(VkDevice logicalDevice, VkSampler &textureSampler);
	static void CreateImage(VkDevice logicalDevice, VkPhysicalDevice physicalDevice, uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& textureImage, VkDeviceMemory& textureImageMemory);
	static uint32_t findMemoryType(uint32_t typeFilter, VkPhysicalDevice physicalDevice, VkMemoryPropertyFlags properties);

};