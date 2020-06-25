#pragma once

#include <iostream>
#include <vector>

#include "vulkan/vulkan.h"

#include "Entities.h"
#include "Entity.h"
#include "Object.h"

class BuffersManagment
{
public:
	BuffersManagment();
	~BuffersManagment();

	void BindVertices();
	void CreateVertexBuffer(VkDevice logicalDevice, VkCommandPool commandPool, VkQueue graphicsQueue,  VkPhysicalDevice physicalDevice, VkBuffer &vertexBuffer, VkDeviceMemory &vertexBufferMemory, Entities entities);
	void CreateIndexBuffer(VkDevice logicalDevice, VkCommandPool commandPool, VkQueue graphicsQueue, VkPhysicalDevice physicalDevice, VkBuffer &indexBuffer, VkDeviceMemory &indexBufferMemory, Entities entities);

	void CreateVertexBuffer(VkDevice logicalDevice, VkCommandPool commandPool, VkQueue graphicsQueue, VkPhysicalDevice physicalDevice, Entity &entity);
	void CreateIndexBuffer(VkDevice logicalDevice, VkCommandPool commandPool, VkQueue graphicsQueue, VkPhysicalDevice physicalDevice, Entity &entity);

	//Vertex Buffer Creation for in Entity Class Creation
	static void CreateVertexBuffer(VkDevice logicalDevice, VkCommandPool commandPool, VkQueue graphicsQueue, VkPhysicalDevice physicalDevice, VkBuffer &vertexBuffer, VkDeviceMemory &vertexBufferMemory, std::vector<Entity::Vertex> vertices);
	//Index Buffer Creation for in Entity Class Creation
	static void CreateIndexBuffer(VkDevice logicalDevice, VkCommandPool commandPool, VkQueue graphicsQueue, VkPhysicalDevice physicalDevice, VkBuffer &indexBuffer, VkDeviceMemory &indexBufferMemory, std::vector<Entity::Vertex> vertices, std::vector<uint16_t> indices);

	void CreateUniformBuffer(VkDevice logicalDevice, VkPhysicalDevice physicalDevice, std::vector<VkImage> swapChainImages, std::vector<VkBuffer> &uniformBuffers, std::vector<VkDeviceMemory> &uniformBuffersMemory);

	void CreateDescriptorSetLayout(VkDevice logicalDevice, VkDescriptorSetLayout &descriptorSetLayout);
	void CreateDescriptorPool(VkDevice logicalDevice, std::vector<VkImage> swapChainImages, VkDescriptorPool &descriptorPool);
	void CreateDescriptorSets(VkDevice logicalDevice, std::vector<VkImage> swapChainImages, VkDescriptorSetLayout descriptorSetLayout, VkDescriptorPool descriptorPool, std::vector<VkDescriptorSet>& descriptorSets, std::vector<VkBuffer> &uniformBuffers, std::vector<VkDeviceMemory> &uniformBuffersMemory);

	static void CreateBuffer(VkDevice logicalDevice, VkPhysicalDevice physicalDevice, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer & vertexBuffer, VkDeviceMemory & vertexBufferMemory);
	static void CopyBuffer(VkDevice  logicalDevice, VkQueue graphicsQueue, VkCommandPool commandPool, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

	static uint32_t findMemoryType(uint32_t typeFilter, VkPhysicalDevice physicalDevice, VkMemoryPropertyFlags properties);
};

