#include "VkHammer.h"


VkHammer::VkHammer()
{
}

VkHammer::~VkHammer()
{
}


#pragma region Initialize

// Instance and GLFW
void VkHammer::InitGLFW(int width, int height) {
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	HEIGHT = height;
	WIDTH = width;

	window = glfwCreateWindow(WIDTH, HEIGHT, AppName, nullptr, nullptr);
	glfwSetWindowUserPointer(window, this);

	glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

}


#pragma region Debug Message Callback Validation Layers

VkResult VkHammer::CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
	auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
	if (func != nullptr) {
		return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
	}
	else {
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}
}

void VkHammer::DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
	auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
	if (func != nullptr) {
		func(instance, debugMessenger, pAllocator);
	}
}
std::vector<const char*> VkHammer::getRequiredExtensions() {
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

	if (CInstance::debugEnabled) {
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}

	return extensions;
}
void VkHammer::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
	createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	createInfo.pfnUserCallback = debugCallback;
}
void VkHammer::setupDebugMessenger() {
	if (!CInstance::debugEnabled) return;

	VkDebugUtilsMessengerCreateInfoEXT createInfo;
	populateDebugMessengerCreateInfo(createInfo);

	if (CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS) {
		throw std::runtime_error("VkHammer: failed to set up debug messenger!");
	}
}

#pragma endregion

// Initialize Functions
void VkHammer::VHinitPhysicalDevice() {
	 physicalDevice = cphysicalDevice.PickPhysicalDevice(instance, deviceExtensions, surface, CInstance::debugEnabled);
	 std::cout << "Created Physical Device" << std::endl;
}
void VkHammer::VHinitLogicalDevice() {
	logicalDevice = clogicalDevice.CreateLogicalDevice(physicalDevice, deviceExtensions, cphysicalDevice.GetQueueGraphicFamily(), cphysicalDevice.GetQueuePresentFamily());
}
void VkHammer::VHinitSwapChain() {
	swapChain = cswapchain.CreateSwapchain(physicalDevice, logicalDevice, surface, window, swapChainImages, swapChainImageFormat, swapChainExtent, WIDTH, HEIGHT, cphysicalDevice.GetQueueGraphicFamily(), cphysicalDevice.GetQueuePresentFamily());
}
void VkHammer::VHCreateImageView() {
	swapChainImageViews = cimageview.CreateImageViews(logicalDevice, swapChainImages, swapChainImageFormat);
}
void VkHammer::VHCreateGraphicsPipeline() {
	renderPass = cgraphicsPipeline.CreateRenderPass(logicalDevice, swapChainImageFormat, FindDepthFormat());
	graphicsPipeline = cgraphicsPipeline.CreateGraphicsPipeline(logicalDevice, "Shaders/", swapChainExtent, renderPass, pipelineLayout, descriptorSetLayout, entities);
	swapChainFramebuffers = cgraphicsPipeline.CreateFrameBuffers(logicalDevice, swapChainImageViews, depthImageView, renderPass, swapChainExtent);
	commandPool = cgraphicsPipeline.CreateCommandPool(logicalDevice, cphysicalDevice.GetQueueGraphicFamily());
	VHCreateVertexBuffer();
	cgraphicsPipeline.CreateCommandBuffers(logicalDevice, commandPool, swapChainFramebuffers, renderPass, swapChainExtent, graphicsPipeline, commandBuffer, pipelineLayout, descriptorSets, entities);
}
void VkHammer::VHRecreateGraphicsPipeline() {
	renderPass = cgraphicsPipeline.CreateRenderPass(logicalDevice, swapChainImageFormat, FindDepthFormat());
	graphicsPipeline = cgraphicsPipeline.CreateGraphicsPipeline(logicalDevice, "Shaders/", swapChainExtent, renderPass, pipelineLayout, descriptorSetLayout, entities);
	swapChainFramebuffers = cgraphicsPipeline.CreateFrameBuffers(logicalDevice, swapChainImageViews, depthImageView, renderPass, swapChainExtent);
	VHCreateVertexBuffer();
	cgraphicsPipeline.CreateCommandBuffers(logicalDevice, commandPool, swapChainFramebuffers, renderPass, swapChainExtent, graphicsPipeline, commandBuffer, pipelineLayout, descriptorSets, entities);
}
void VkHammer::VHCreateVertexBuffer() {
	buffersManagment.CreateVertexBuffer(logicalDevice, commandPool, clogicalDevice.GetGraphicQueue(), physicalDevice, vertexBuffer, vertexBufferMemory, entities);
	buffersManagment.CreateIndexBuffer(logicalDevice, commandPool, clogicalDevice.GetGraphicQueue(), physicalDevice, indexBuffer, indexBufferMemory, entities);
	buffersManagment.CreateUniformBuffer(logicalDevice, physicalDevice, swapChainImages, uniformBuffers, uniformBuffersMemory);
}

void VkHammer::CreateDepthBuffer()
{
	VkFormat depthFormat = FindDepthFormat();

	ImageLoader::CreateImage(logicalDevice, physicalDevice,swapChainExtent.width, swapChainExtent.height, depthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, depthImage, depthImageMemory);
	depthImageView = cimageview.CreateImageView(logicalDevice, depthImage, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT);
}

VkFormat VkHammer::FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features)
{
	for (VkFormat format : candidates) {
		VkFormatProperties props;
		vkGetPhysicalDeviceFormatProperties(physicalDevice, format, &props);

		if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
			return format;
		}
		else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
			return format;
		}
	}

	throw std::runtime_error("failed to find supported format!");
}

VkFormat VkHammer::FindDepthFormat()
{
	return FindSupportedFormat(
		{ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
		VK_IMAGE_TILING_OPTIMAL,
		VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
	);
}

void VkHammer::VHCreateSemaphores()
{
	cgraphicsPipeline.CreateSemaphores(logicalDevice, MAX_FRAMES_IN_FLIGHT, imageAvailableSemaphores, renderFinishedSemaphores, inFlightFences, imagesInFlight, swapChainImages);
}

void VkHammer::VHinit(const char* name, int width, int height) {
	AppName = name;
	InitGLFW(width, height);
	instance = cinstance.CreateInstance(AppName);
	setupDebugMessenger();

	srand(time(NULL));

	//Create Surface
	if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS) {
		throw std::runtime_error("CSurface: Failed to create window surface!");
	}

	VHinitPhysicalDevice();
	VHinitLogicalDevice();
	swapChain = cswapchain.CreateSwapchain(physicalDevice, logicalDevice, surface, window, swapChainImages, swapChainImageFormat, swapChainExtent, WIDTH, HEIGHT, cphysicalDevice.GetQueueGraphicFamily(), cphysicalDevice.GetQueuePresentFamily());
	VHCreateImageView();


	l = 50;

	ModelLoader *sun = new ModelLoader;
	sun->LoadModel("../Assets/Moon 2K.obj", "../Assets/space.png"); 
	entities.AddEntity(sun);

	ModelLoader *sun2 = new ModelLoader;
	sun2->LoadModel("../Assets/Moon 2K.obj", "../Assets/sun.png");
	entities.AddEntity(sun2);

	for (size_t i = 0; i != l; i++)
	{
		ModelLoader *t = new ModelLoader;
		if (i % 3 == 1) {
			t->LoadModel("../Assets/Moon 2K.obj", "../Assets/space.png");
			moons.push_back(t);
		}
		else if (i % 3 == 2) {
			t->LoadModel("../Assets/Moon 2K.obj", "../Assets/Diffuse_2K.png");
			moons.push_back(t);
		}
		else {
			t->LoadModel("../Assets/Moon 2K.obj", "../Assets/space.png");
			moons.push_back(t);
		}
	}

	for (size_t i = 0; i != l; i++)
	{
		entities.AddEntity(moons[i]);
	}

	renderPass = cgraphicsPipeline.CreateRenderPass(logicalDevice, swapChainImageFormat, FindDepthFormat());
	buffersManagment.CreateDescriptorSetLayout(logicalDevice, descriptorSetLayout);
	graphicsPipeline = cgraphicsPipeline.CreateGraphicsPipeline(logicalDevice, "Shaders/TextureTest", swapChainExtent, renderPass, pipelineLayout, descriptorSetLayout, entities);
	CreateDepthBuffer();
	swapChainFramebuffers = cgraphicsPipeline.CreateFrameBuffers(logicalDevice, swapChainImageViews, depthImageView, renderPass, swapChainExtent);
	commandPool = cgraphicsPipeline.CreateCommandPool(logicalDevice, cphysicalDevice.GetQueueGraphicFamily());

	//Load Textures
	for (size_t i = 0; i != entities.allEntities.size(); i++)
	{
		entities.allEntities[i]->LoadTexture(logicalDevice, physicalDevice, commandPool, clogicalDevice.GetGraphicQueue());
	}
	
	for (size_t i = 0; i != entities.allEntities.size(); i++) {
		entities.allEntities[i]->CreateAllBuffers(logicalDevice, physicalDevice, commandPool, swapChainImages, clogicalDevice.GetGraphicQueue());
		entities.allEntities[i]->CreateAllDescriptorsSets(logicalDevice, swapChainImages, descriptorSetLayout);
		for (size_t j = 0; j != entities.allEntities[i]->descriptorSets.size(); j++)
		{
			entities.allDescriptorSets.push_back(entities.allEntities[i]->descriptorSets[j]);
		}
		for (size_t j = 0; j != entities.allEntities[i]->object.uniformBuffersMemory.size(); j++)
		{
			entities.allUniformBuffers.push_back(entities.allEntities[i]->object.uniformBuffersMemory[j]);
		}
	}

	cgraphicsPipeline.CreateCommandBuffers(logicalDevice, commandPool, swapChainFramebuffers, renderPass, swapChainExtent, graphicsPipeline, commandBuffer, pipelineLayout, entities);

	VHCreateSemaphores();

}

#pragma endregion


#pragma region Update

void VkHammer::VHupdate() {
	//while (!glfwWindowShouldClose(window)) {
	//	glfwPollEvents();
	//	DrawFrame();
	//}
	DrawFrame();

	vkDeviceWaitIdle(logicalDevice);
}

void VkHammer::DrawFrame()
{
	//Kronos::UpdateTimeBegin();

	MoonBehaviour* beh1 = new MoonBehaviour(entities.allEntities[0]->object, 0.5f, 0);
	entities.allEntities[0]->behaviours.push_back(beh1);

	MoonBehaviour* beh2 = new MoonBehaviour(entities.allEntities[1]->object, 0.5f, 1);
	entities.allEntities[1]->behaviours.push_back(beh2);

	for (size_t i = 2; i != entities.allEntities.size(); i++)
	{
		MoonBehaviour *beh = new MoonBehaviour(entities.allEntities[i]->object, i * 0.3f, i * 11.f);
		entities.allEntities[i]->behaviours.push_back(beh);
	}

	for (size_t i = 0; i != entities.allEntities.size(); i++)
	{
		for (size_t j = 0; j != entities.allEntities[i]->behaviours.size(); j++)
		{
			entities.allEntities[i]->behaviours[j]->Start();
		}
	}

	float time = 0;
	double lastTime = 0.0;

	while (!glfwWindowShouldClose(window)) {

		double timeclock = glfwGetTime();
		double deltaTime = timeclock - lastTime;

		if (deltaTime >= maxPeriod) {
			lastTime = timeclock;
			glfwPollEvents();

			Khronos::UpdateTimeBegin();

			vkWaitForFences(logicalDevice, 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

			uint32_t imageIndex;
			VkResult result = vkAcquireNextImageKHR(logicalDevice, swapChain, UINT64_MAX, imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);

			if (result == VK_ERROR_OUT_OF_DATE_KHR) {
				RecreateSwapChain();
				return;
			}
			else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
				throw std::runtime_error("failed to acquire swap chain image!");
			}

			processInput(window);
			processInputCam(window);


			if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
				time += glfwGetTime() / 1900000;
			else if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
				time += 0;
			else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
				time += glfwGetTime() / 8000;
			else
				time += glfwGetTime() / 120000;


			for (size_t i = 0; i != entities.allEntities.size(); i++)
			{
				for (size_t j = 0; j != entities.allEntities[i]->behaviours.size(); j++)
				{
					entities.allEntities[i]->behaviours[j]->Update(logicalDevice, imageIndex, MainCamera, time);
				}
			}

			
			if (imagesInFlight[imageIndex] != VK_NULL_HANDLE) {
				vkWaitForFences(logicalDevice, 1, &imagesInFlight[imageIndex], VK_TRUE, UINT64_MAX);
			}
			imagesInFlight[imageIndex] = inFlightFences[currentFrame];

			VkSubmitInfo submitInfo = {};
			submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

			VkSemaphore waitSemaphores[] = { imageAvailableSemaphores[currentFrame] };
			VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
			submitInfo.waitSemaphoreCount = 1;
			submitInfo.pWaitSemaphores = waitSemaphores;
			submitInfo.pWaitDstStageMask = waitStages;

			submitInfo.commandBufferCount = 1;
			submitInfo.pCommandBuffers = &commandBuffer.data()[imageIndex];

			VkSemaphore signalSemaphores[] = { renderFinishedSemaphores[currentFrame] };
			submitInfo.signalSemaphoreCount = 1;
			submitInfo.pSignalSemaphores = signalSemaphores;

			vkResetFences(logicalDevice, 1, &inFlightFences[currentFrame]);

			if (vkQueueSubmit(clogicalDevice.GetGraphicQueue(), 1, &submitInfo, inFlightFences[currentFrame]) != VK_SUCCESS) {
				throw std::runtime_error("failed to submit draw command buffer!");
			}

			VkPresentInfoKHR presentInfo = {};
			presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

			presentInfo.waitSemaphoreCount = 1;
			presentInfo.pWaitSemaphores = signalSemaphores;

			VkSwapchainKHR swapChains[] = { swapChain };
			presentInfo.swapchainCount = 1;
			presentInfo.pSwapchains = swapChains;

			presentInfo.pImageIndices = &imageIndex;

			result = vkQueuePresentKHR(clogicalDevice.GetPresentQueue(), &presentInfo);

			if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || frameBufferResized) {
				frameBufferResized = false;
				RecreateSwapChain();
			}
			else if (result != VK_SUCCESS) {
				throw std::runtime_error("failed to present swap chain image!");
			}

			currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;

			Khronos::UpdateTimeEnd();
			/*if(n == false)
				vkDestroyPipeline(logicalDevice, graphicsPipeline2, nullptr);*/
		}
	}
}


#pragma endregion

#pragma region Recreation
void VkHammer::CleanupSwapChain()
{
	for (auto framebuffer : swapChainFramebuffers) {
		vkDestroyFramebuffer(logicalDevice, framebuffer, nullptr);
	}

	vkFreeCommandBuffers(logicalDevice, commandPool, static_cast<uint32_t>(commandBuffer.size()), commandBuffer.data());

	vkDestroyPipeline(logicalDevice, graphicsPipeline, nullptr);
	vkDestroyPipelineLayout(logicalDevice, pipelineLayout, nullptr);
	vkDestroyRenderPass(logicalDevice, renderPass, nullptr);

	for (auto imageView : swapChainImageViews) {
		vkDestroyImageView(logicalDevice, imageView, nullptr);
	}

	vkDestroySwapchainKHR(logicalDevice, swapChain, nullptr);

	uint32_t swapChainSize = swapChainImages.size();

	//for (size_t j = 0; j != swapChainSize; j++) {
	//	vkDestroyBuffer(logicalDevice, uniformBuffers[j], nullptr);
	//	vkFreeMemory(logicalDevice, uniformBuffersMemory[j], nullptr);
	//}

	for (size_t i = 0; i != entities.allEntities.size(); i++) {
		for (size_t j= 0; j != swapChainSize; j++) {
			vkDestroyBuffer(logicalDevice, entities.allEntities[i]->object.uniformBuffers[j], nullptr);
			vkFreeMemory(logicalDevice, entities.allEntities[i]->object.uniformBuffersMemory[j], nullptr);
		}
	}
	
	vkDestroyDescriptorPool(logicalDevice, descriptorPool, nullptr);
}

void VkHammer::RecreateSwapChain() 
{
	int width = 0, height = 0;
	while (width == 0 || height == 0) {
		glfwGetFramebufferSize(window, &width, &height);
		glfwWaitEvents();
	}

	vkDeviceWaitIdle(logicalDevice);

	CleanupSwapChain();

	VHinitSwapChain();
	VHCreateImageView();
	VHRecreateGraphicsPipeline();
}

#pragma endregion


void VkHammer::Cleanup() {

	CleanupSwapChain();

	vkDestroyDescriptorSetLayout(logicalDevice, descriptorSetLayout, nullptr);

	vkDestroyBuffer(logicalDevice, indexBuffer, nullptr);
	vkFreeMemory(logicalDevice, indexBufferMemory, nullptr);

	for (size_t i = 0; i != entities.allEntities.size(); i++) {
		entities.allEntities[i]->Cleanup(logicalDevice, swapChainImages);
	}

	vkDestroyBuffer(logicalDevice, vertexBuffer, nullptr);
	vkFreeMemory(logicalDevice, vertexBufferMemory, nullptr);

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		vkDestroySemaphore(logicalDevice, renderFinishedSemaphores[i], nullptr);
		vkDestroySemaphore(logicalDevice, imageAvailableSemaphores[i], nullptr);
		vkDestroyFence(logicalDevice, inFlightFences[i], nullptr);
	}

	vkDestroyCommandPool(logicalDevice, commandPool, nullptr);
	vkDestroyCommandPool(logicalDevice, commandPool2, nullptr);

	vkDestroyDevice(logicalDevice, nullptr);

	if (CInstance::debugEnabled) {
		DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
	}

	vkDestroySurfaceKHR(instance, surface, nullptr);
	vkDestroyInstance(instance, nullptr);

	glfwDestroyWindow(window);

	glfwTerminate();
}

#pragma region Set
void VkHammer::SetInstance(VkInstance Instance) {
	instance = Instance;
}
void VkHammer::SetPhysicalDevice(VkPhysicalDevice Device) {
	physicalDevice = Device;
}
#pragma endregion