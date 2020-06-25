#include "CInstance.h"

const std::vector<const char*> CInstance::validationLayers = {
	   "VK_LAYER_KHRONOS_validation",
	   "VK_LAYER_LUNARG_monitor" };

CInstance::CInstance()
{
	
}


CInstance::~CInstance()
{
}

std::vector<const char*> getRequiredExtensions() {

	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	std::cout << glfwExtensions << std::endl;


	std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

	std::cout << extensions.size() << std::endl;

	for (size_t i = 0; i != extensions.size(); i++)
	{
		std::cout << extensions[i] << std::endl;
	}

	if (CInstance::debugEnabled) {
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}

	std::cout << std::endl;

	return extensions;
}

bool CInstance::checkValidationLayerSupport() {

	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	for (const auto& layerProperties : availableLayers) {
		std::cout << layerProperties.layerName << std::endl;
	}

	std::cout << std::endl;

	for (const char* layerName : validationLayers) {
		bool layerFound = false;

		for (const auto& layerProperties : availableLayers) {
			if (strcmp(layerName, layerProperties.layerName) == 0) {
				layerFound = true;
				break;
			}
		}

		if (!layerFound) {
			return false;
		}

		std::cout << "CInstance: Found Layer: " << layerName << std::endl;
	}

	return true;
}

VkInstance CInstance::CreateInstance(const char* AppName) {
	VkInstance _instance;

	if (debugEnabled && !checkValidationLayerSupport()) {
		throw std::runtime_error("CInstance: validation layers requested, but not available!");
	}

	std::cout << std::endl;

	VkApplicationInfo appInfo = {};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = AppName;
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "No Engine";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_0;

	VkInstanceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;

	auto extensions = getRequiredExtensions();
	createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
	createInfo.ppEnabledExtensionNames = extensions.data();

	if (debugEnabled) {
		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();

	}
	else {
		createInfo.enabledLayerCount = 0;

		createInfo.pNext = nullptr;
	}

	if (vkCreateInstance(&createInfo, nullptr, &_instance) != VK_SUCCESS) {
		throw std::runtime_error("CInstance: failed to create instance!");
	}
	return _instance;
}