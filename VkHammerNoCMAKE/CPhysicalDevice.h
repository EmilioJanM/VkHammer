#pragma once

#include <vulkan/vulkan.h>

#include <iostream>
#include <vector>
#include <map>
#include <set>


//Get Physical Devices and get the queue family for graphics.

class CPhysicalDevice
{
private:
	
	uint32_t deviceCount = 0;

	struct QueueFamilies {
		uint32_t queueGraphicFamily = 4294967295;
		uint32_t queuePresentFamily = 4294967295;
	};

	std::vector<VkPhysicalDevice> devices;

	VkPhysicalDeviceProperties deviceProperties;
	VkPhysicalDeviceFeatures deviceFeatures;

	void GetFeaturesAndProperties(VkPhysicalDevice physicalDevice);
	VkPhysicalDevice SelectBestPhysicalDevice();
	void GetGraphicAndPresentQueue(VkSurfaceKHR surface, VkPhysicalDevice physicalDevice);

	bool checkDeviceExtensionsSwapChain(VkPhysicalDevice device, const std::vector<const char*> deviceExtensions);

public:
	CPhysicalDevice();
	~CPhysicalDevice();

	QueueFamilies queueFamilies;

	//Pick a GPU
	 VkPhysicalDevice PickPhysicalDevice(VkInstance instance, const std::vector<const char*> deviceExtensions, VkSurfaceKHR surface, bool debug);

	 int GetQueueGraphicFamily();
	 int GetQueuePresentFamily();

};

