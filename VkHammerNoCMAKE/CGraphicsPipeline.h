#pragma once

#include <fstream>
#include <iostream>
#include <vector>
#include <array>

#include "vulkan/vulkan.h"

#include "Entities.h"


class CGraphicsPipeline
{
private:

	static std::vector<char> ReadFile(const std::string& filename);
	VkShaderModule CreateShaderModule(VkDevice logicalDevice, const std::vector<char>& code);

public:
	CGraphicsPipeline();
	~CGraphicsPipeline();

	VkPipeline  CreateGraphicsPipeline(VkDevice logicalDevice, const std::string & filename, VkExtent2D swapChainExtent, VkRenderPass renderPass, VkPipelineLayout &pipelineLayout, VkDescriptorSetLayout & descriptorSetLayout, Entities entities);
	VkPipeline  CreateGraphicsPipeline_Line(VkDevice logicalDevice, const std::string & filename, VkExtent2D swapChainExtent, VkRenderPass renderPass, VkPipelineLayout &pipelineLayout, VkDescriptorSetLayout & descriptorSetLayout, Entities entities);
	VkRenderPass CreateRenderPass(VkDevice logicalDevice, VkFormat swapChainImageFormat, VkFormat depthFormat);
	std::vector<VkFramebuffer> CreateFrameBuffers(VkDevice logicalDevice, std::vector<VkImageView> swapChainImageViews, VkImageView depthImageView, VkRenderPass renderPass, VkExtent2D swapChainExtent);
	VkCommandPool CreateCommandPool(VkDevice logicalDevice, uint32_t graphicsFamilyQueue);
	void CreateCommandBuffers(VkDevice logicalDevice, VkCommandPool commandPool, std::vector<VkFramebuffer> swapChainFramebuffers, VkRenderPass renderPass, VkExtent2D swapChainExtent, VkPipeline graphicsPipeline, std::vector<VkCommandBuffer> &commandBuffers, VkPipelineLayout pipelineLayout, std::vector<VkDescriptorSet> descriptorSets, Entities entities);
	void CreateCommandBuffers(VkDevice logicalDevice, VkCommandPool commandPool, std::vector<VkFramebuffer> swapChainFramebuffers, VkRenderPass renderPass, VkExtent2D swapChainExtent, VkPipeline graphicsPipeline, std::vector<VkCommandBuffer> &commandBuffers, VkPipelineLayout pipelineLayout, Entities entities);
	void CreateSemaphore(VkDevice logicalDevice, VkSemaphore & s);
	void CreateSemaphores(VkDevice logicalDevice, int MAX_FRAMES_IN_FLIGHT, std::vector<VkSemaphore> &imageAvailableSemaphores, std::vector<VkSemaphore> &renderFinishedSemaphores, std::vector<VkFence> &inFlightFences, std::vector<VkFence> &imagesInFlight, std::vector<VkImage> swapChainImages);
};

