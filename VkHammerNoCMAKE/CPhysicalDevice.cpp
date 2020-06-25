#include "CPhysicalDevice.h"



CPhysicalDevice::CPhysicalDevice()
{
}


CPhysicalDevice::~CPhysicalDevice()
{
}



VkPhysicalDevice CPhysicalDevice::PickPhysicalDevice(VkInstance instance, const std::vector<const char*> deviceExtensions, VkSurfaceKHR surface, bool debug)
{
	VkPhysicalDevice physicalDevice;

	vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

	devices.resize(deviceCount);
	vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

	physicalDevice = SelectBestPhysicalDevice();
	GetFeaturesAndProperties(physicalDevice);
	GetGraphicAndPresentQueue(surface, physicalDevice);

	if(!checkDeviceExtensionsSwapChain(physicalDevice, deviceExtensions))
		throw std::runtime_error("CPhysicalDevice: No present device extension for swap chain could be found!");

	if (debug)
		std::cout << "CPhysicalDevice: GPU found: " << deviceProperties.deviceName << std::endl;

	return physicalDevice;
}
VkPhysicalDevice CPhysicalDevice::SelectBestPhysicalDevice() {

	int index = 0;
	int score = 0;
	int scoreBuffer = 0;

	for (uint32_t i = 0; i != deviceCount; i++) {

		vkGetPhysicalDeviceProperties(devices[i], &deviceProperties);
		vkGetPhysicalDeviceFeatures(devices[i], &deviceFeatures);

		// Size of tetures in both 2D and 3D
		score += deviceProperties.limits.maxImageDimension2D;
		score += deviceProperties.limits.maxImageDimension3D;

		// If geometry shaders or the GPU is not discrete the device is not suitable so bye bye
		if (!deviceFeatures.geometryShader || deviceProperties.deviceType != VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
			continue;
		}

		if (score > scoreBuffer) {
			scoreBuffer = score;
			index = i;
		}
	}

	if (scoreBuffer == 0)
		throw std::runtime_error("CPhysicalDevice: Failed to find a suitable GPU!");

	return devices[index];
}

bool CPhysicalDevice::checkDeviceExtensionsSwapChain(VkPhysicalDevice device, const std::vector<const char*> deviceExtensions) {
	uint32_t extensionCount;
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

	std::vector<VkExtensionProperties> availableExtensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

	std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

	for (const auto& extension : availableExtensions) {
		requiredExtensions.erase(extension.extensionName);
	}

	return requiredExtensions.empty();
}

void CPhysicalDevice::GetGraphicAndPresentQueue(VkSurfaceKHR surface, VkPhysicalDevice physicalDevice)
{
	uint32_t queue_families_count;

	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queue_families_count, nullptr);

	std::vector<VkQueueFamilyProperties> queue_families(queue_families_count);

	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queue_families_count, queue_families.data());
	if (queue_families_count == 0)
		std::cout << "CPhysicalDevice: Could not get Physical Device Queue Family Properties." << std::endl;

	/*Graphics: For creating graphics pipelinesand drawing								VK_QUEUE_GRAPHICS_BIT
	Compute : For creating compute pipelinesand dispatching compute shaders				VK_QUEUE_COMPUTE_BIT
	Transfer : Used for very fast memory - copying operations							VK_QUEUE_TRANSFER_BIT
	Sparse : Allows for additional memory management features							VK_QUEUE_SPARSE_BINDING_BIT*/

	bool graphics = false, present = false;
	for (uint32_t index = 0; index != static_cast<uint32_t>(queue_families.size()); index++) {
		if ((queue_families[index].queueCount > 0) && (queue_families[index].queueFlags & VK_QUEUE_GRAPHICS_BIT) && !graphics) {
			//Just to use on family queue, to use more queue_family_index must be a vector
			queueFamilies.queueGraphicFamily = index;
			graphics = true;
		}

		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, index, surface, &presentSupport);

		if (queue_families[index].queueCount > 0 && presentSupport && !present) {
			queueFamilies.queuePresentFamily = index;
			present = true;
		}

	}

	if (queueFamilies.queueGraphicFamily == 4294967295)
		throw std::runtime_error("CPhysicalDevice: No graphic family could be found!");

	if (queueFamilies.queuePresentFamily == 4294967295)
		throw std::runtime_error("CPhysicalDevice: No present family could be found!");
}

void CPhysicalDevice::GetFeaturesAndProperties(VkPhysicalDevice physicalDevice) {

	vkGetPhysicalDeviceProperties(physicalDevice, &deviceProperties);
	vkGetPhysicalDeviceFeatures(physicalDevice, &deviceFeatures);
}


int CPhysicalDevice::GetQueueGraphicFamily()
{
	return queueFamilies.queueGraphicFamily;
}
int CPhysicalDevice::GetQueuePresentFamily()
{
	return queueFamilies.queuePresentFamily;
}

