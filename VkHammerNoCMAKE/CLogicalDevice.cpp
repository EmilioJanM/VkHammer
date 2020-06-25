#include "CLogicalDevice.h"



CLogicalDevice::CLogicalDevice()
{
}


CLogicalDevice::~CLogicalDevice()
{
}

VkDevice CLogicalDevice::CreateLogicalDevice(VkPhysicalDevice physicalDevice, const std::vector<const char*> deviceExtensionsSwapChain, uint32_t queueGraphicsFamilyIndex, uint32_t queuePresentFamilyIndex)
{
	VkDevice logicalDevice;

	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	std::set<uint32_t> uniqueQueueFamilies = { queueGraphicsFamilyIndex, queuePresentFamilyIndex };

	float queuePriority = 1.0f;
	for (uint32_t queueFamily : uniqueQueueFamilies) {
		VkDeviceQueueCreateInfo queueCreateInfo = {};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = queueFamily;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &queuePriority;
		queueCreateInfos.push_back(queueCreateInfo);
	}


	///////////////////////////////////////////////////////////
	VkPhysicalDeviceFeatures deviceFeatures = {};
	deviceFeatures.geometryShader =			1;
	deviceFeatures.fillModeNonSolid =		1;
	deviceFeatures.wideLines =				1;
	deviceFeatures.samplerAnisotropy =		1;

	VkDeviceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	
	createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
	createInfo.pQueueCreateInfos = queueCreateInfos.data();

	createInfo.pEnabledFeatures = &deviceFeatures;

	createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensionsSwapChain.size());
	createInfo.ppEnabledExtensionNames = deviceExtensionsSwapChain.data();


	if (CInstance::debugEnabled) {
		createInfo.enabledLayerCount = static_cast<uint32_t>(CInstance::validationLayers.size());
		createInfo.ppEnabledLayerNames = CInstance::validationLayers.data();
	}
	else {
		createInfo.enabledLayerCount = 0;
	}

	if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &logicalDevice) != VK_SUCCESS) {
		throw std::runtime_error("Failed to Create Logical Device!");
	}

	std::cout << std::endl;


	std::cout << "CLogicalDevice: Created Logical Device" << std::endl;

	vkGetDeviceQueue(logicalDevice, queueGraphicsFamilyIndex, 0, &graphicQueue);
	std::cout << "CLogicalDevice: Got Graphics Device Queue: " << graphicQueue << std::endl;

	vkGetDeviceQueue(logicalDevice, queuePresentFamilyIndex, 0, &presentQueue);
	std::cout << "CLogicalDevice: Got Present Device Queue: " << presentQueue << std::endl;


	////////////////////////////////////////////////////////////////////////////
	
	return logicalDevice;
}
