#pragma once

#include <iostream>
#include <vector>
#include <stdexcept>
#include <cstdint>
#include <algorithm>

#include "vulkan/vulkan.h"
#include "GLFW/glfw3.h"

class CSwapChain
{
private:
	struct SwapChainSupportDetails {
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};

	SwapChainSupportDetails  GetSwapChainSupportDetails(VkPhysicalDevice device, VkSurfaceKHR surface);
	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, GLFWwindow *window);

public:

	VkSwapchainKHR CreateSwapchain(VkPhysicalDevice physicalDevice, VkDevice logicalDevice, VkSurfaceKHR surface, GLFWwindow *window, std::vector<VkImage> &swapChainImages, VkFormat &swapChainImageFormat, VkExtent2D &swapChainExtent, int WIDTH, int HEIGHT, uint32_t graphicsFamilyQueue, uint32_t presentFamilyQueue);

	CSwapChain();
	~CSwapChain();
};

