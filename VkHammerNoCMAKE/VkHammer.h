#pragma once



#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <time.h>

//Components
#include "Entities.h"
#include "CInstance.h"
#include "CPhysicalDevice.h"
#include "CLogicalDevice.h"
#include "CSwapChain.h"
#include "CImageView.h"
#include "CGraphicsPipeline.h"
#include "BuffersManagment.h"

#include "Camera.h"
#include "Khronos.h"

#include "Entity.h"
#include "Behaviour.h"

#include "Triangle.h"
#include "Sea.h"
#include "ImageLoader.h"
#include "ModelLoader.h"

//BEHAVIOURS
#include "MoonBehaviour.h"

class VkHammer
{
private:

	const std::vector<const char*> deviceExtensions = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

	Camera MainCamera;

	//GLFW
	GLFWwindow * window;

	int WIDTH, HEIGHT;

	Entities entities;

	//Vulkan Components
	VkInstance instance;
	VkDebugUtilsMessengerEXT debugMessenger;
	VkSurfaceKHR surface;
	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
	VkDevice logicalDevice;

	//Class Components, C goes for component
	CInstance cinstance;
	CPhysicalDevice cphysicalDevice;
	CLogicalDevice clogicalDevice;
	CSwapChain cswapchain;
	CImageView cimageview;
	CGraphicsPipeline cgraphicsPipeline;
	BuffersManagment buffersManagment;
	ImageLoader imageLoader;

	//Swap Chain
	VkSwapchainKHR swapChain;
	std::vector<VkImage> swapChainImages;
	VkFormat swapChainImageFormat;
	VkExtent2D swapChainExtent;

	//Image View
	std::vector<VkImageView> swapChainImageViews;

	//Graphics Pipeline
	VkRenderPass renderPass;
	VkPipelineLayout pipelineLayout;
	VkPipeline graphicsPipeline;
	VkPipeline graphicsPipeline2;

	VkDescriptorSetLayout descriptorSetLayout;
	VkDescriptorPool descriptorPool;
	VkDescriptorPool descriptorPool2;
	std::vector<VkDescriptorSet> descriptorSets;
	std::vector<VkDescriptorSet> descriptorSets2;

	//FrameBuffers, in Graphics Pipeline Class
	std::vector<VkFramebuffer> swapChainFramebuffers;
	bool frameBufferResized = false;

	//Command Buffers, in Graphics Pipeline Class
	VkCommandPool commandPool;
	VkCommandPool commandPool2;

	std::vector<VkCommandBuffer> commandBuffer;

	//Semaphores
	const int MAX_FRAMES_IN_FLIGHT = 2;
	std::vector<VkSemaphore> imageAvailableSemaphores;
	std::vector<VkSemaphore> renderFinishedSemaphores;
	std::vector<VkFence> inFlightFences;
	std::vector<VkFence> imagesInFlight;
	size_t currentFrame = 0;

	//Vertex & Index Buffers
	VkBuffer vertexBuffer;
	VkDeviceMemory vertexBufferMemory;
	VkBuffer indexBuffer;
	VkDeviceMemory indexBufferMemory;
	std::vector<VkBuffer> uniformBuffers;
	std::vector<VkDeviceMemory> uniformBuffersMemory;

	//Depth Buffer
	VkImage depthImage;
	VkDeviceMemory depthImageMemory;
	VkImageView depthImageView;

	//Camera Managment
	bool firstMouse = true;
	float yaw = 90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
	float pitch = 0.f;
	float lastX = 800.0f / 2.0;
	float lastY = 600.0 / 2.0;
	float fov = 45.0f;

	//Texture Test
	VkImage textureImage;
	VkImageView textureImageView;
	VkDeviceMemory textureImageMemory;
	VkSampler textureSampler;

	//Settings
	const double maxFPS = 144.0;
	const double maxPeriod = 1.0 / maxFPS;


	///////////////////////////////////////////////////Objects, geometry

	int l;

	//Model Loader
	ModelLoader *e = new ModelLoader;

	std::vector<Triangle> trianglesVector;
	std::vector<Entity*> moons;


	//Sea *sea = new Sea(3, -3, 0, 100, 100);

	float var = 0;
	bool n = true;

	void InitGLFW(int width, int height);

	//Debug Callback from valdiation layers
	std::vector<const char*> getRequiredExtensions();
	VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);
	void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);
	void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
	void setupDebugMessenger();
	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {
		std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

		return VK_FALSE;
	}

public:

	VkHammer();
	~VkHammer();

	const char* AppName;

	//Set Components Functions
	void SetInstance(VkInstance Instance);
	void SetPhysicalDevice(VkPhysicalDevice Device);

	//VkHammer Functions
	//Initialize VkHammer
	void VHinit(const char* name, int width, int height);
	void VHinitPhysicalDevice();
	void VHinitLogicalDevice();
	void VHinitSwapChain();
	void VHCreateImageView();
	void VHCreateGraphicsPipeline();
	void VHRecreateGraphicsPipeline();
	void VHCreateSemaphores();
	void VHCreateVertexBuffer();

	//Depth buffer creation
	void CreateDepthBuffer();
	VkFormat FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
	VkFormat FindDepthFormat();
	bool HasStencilComponent(VkFormat format);

	//Update VkHammer
	void VHupdate();
	void DrawFrame();
	void UpdateUniformBuffer(uint32_t currentImage, Entity entity, float n, float distance);
	void UpdateUniformBuffers(uint32_t currentImage, Entities entities, float n, float distance);

	//Recreation
	void CleanupSwapChain();
	void RecreateSwapChain();

	//Clean VkHammer
	void Cleanup();

	//Camera Managment
	static void framebufferResizeCallback(GLFWwindow* window, int width, int height) {
		auto app = reinterpret_cast<VkHammer*>(glfwGetWindowUserPointer(window));
		app->frameBufferResized = true;
	}
	void processInput(GLFWwindow *window)
	{
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);
	}
	void processInputCam(GLFWwindow *window)
	{
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			MainCamera.cameraPos += MainCamera.cameraSpeed * MainCamera.cameraFront * Khronos::DeltaTime();
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			MainCamera.cameraPos -= MainCamera.cameraSpeed * MainCamera.cameraFront * Khronos::DeltaTime();
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			MainCamera.cameraPos -= glm::normalize(glm::cross(MainCamera.cameraFront, MainCamera.cameraUp)) * MainCamera.cameraSpeed * Khronos::DeltaTime();
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			MainCamera.cameraPos += glm::normalize(glm::cross(MainCamera.cameraFront, MainCamera.cameraUp)) * MainCamera.cameraSpeed * Khronos::DeltaTime();
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
			MainCamera.cameraSpeed = MainCamera.cameraNormalSpeed * 3;
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
			MainCamera.cameraSpeed = MainCamera.cameraNormalSpeed;


	}
	static void mouse_callback(GLFWwindow* window, double xpos, double ypos)
	{
		auto app = reinterpret_cast<VkHammer*>(glfwGetWindowUserPointer(window));
		if (app->firstMouse)
		{
			app->lastX = xpos;
			app->lastY = ypos;
			app->firstMouse = false;
		}

		float xoffset = xpos - app->lastX;
		float yoffset = app->lastY - ypos;
		app->lastX = xpos;
		app->lastY = ypos;

		float sensitivity = 0.05;
		xoffset *= sensitivity;
		yoffset *= sensitivity;

		app->yaw += xoffset;
		app->pitch += yoffset;

		if (app->pitch > 89.0f)
			app->pitch = 89.0f;
		if (app->pitch < -89.0f)
			app->pitch = -89.0f;

		glm::vec3 front;
		front.x = cos(glm::radians(app->yaw)) * cos(glm::radians(app->pitch));
		front.y = sin(glm::radians(app->pitch));
		front.z = sin(glm::radians(app->yaw)) * cos(glm::radians(app->pitch));
		app->MainCamera.cameraFront = glm::normalize(front);
	}
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		auto app = reinterpret_cast<VkHammer*>(glfwGetWindowUserPointer(window));
		if (app->fov >= 1.0f && app->fov <= 45.0f)
			app->fov -= yoffset;
		if (app->fov <= 1.0f)
			app->fov = 1.0f;
		if (app->fov >= 45.0f)
			app->fov = 45.0f;
	}
};
