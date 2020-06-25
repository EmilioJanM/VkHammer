#pragma once

#define GLM_ENABLE_EXPERIMENTAL	

#include <iostream>
#include <vector>
#include <array>

#include "vulkan/vulkan.h"

#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/hash.hpp>

#include "Camera.h"
#include "Khronos.h"
#include "Object.h"
#include "Behaviour.h"

#include "ImageLoader.h"

class Entity
{
public:
	Entity();
	~Entity();

	Object object;

	struct Vertex {
		glm::vec3 pos;
		glm::vec3 color;
		glm::vec2 texCoord;

		static VkVertexInputBindingDescription getBindingDescription() {
			VkVertexInputBindingDescription bindingDescription = {};
			bindingDescription.binding = 0;
			bindingDescription.stride = sizeof(Vertex);
			bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

			return bindingDescription;
		}

		static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions() {
			std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions = {};

			attributeDescriptions[0].binding = 0;
			attributeDescriptions[0].location = 0;
			attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
			attributeDescriptions[0].offset = offsetof(Vertex, pos);

			attributeDescriptions[1].binding = 0;
			attributeDescriptions[1].location = 1;
			attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
			attributeDescriptions[1].offset = offsetof(Vertex, color);

			attributeDescriptions[2].binding = 0;
			attributeDescriptions[2].location = 2;
			attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
			attributeDescriptions[2].offset = offsetof(Vertex, texCoord);

			return attributeDescriptions;
		}

		bool operator==(const Vertex& other) const {
			return pos == other.pos && color == other.color && texCoord == other.texCoord;
		}
	} vertexData;

	
	virtual void Update(VkDevice logicalDevice, uint32_t currentImage, Camera c, float n, float distance, float time) {
		std::cout << "Entity Start" << std::endl;
	}
	virtual void Start() {
		std::cout << "Entity Update" << std::endl;
	}


	struct Position
	{
		float x, y, z;
	} position;

	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;

	VkBuffer vertexBuffer;
	VkDeviceMemory vertexBufferMemory;

	VkBuffer indexBuffer;
	VkDeviceMemory indexBufferMemory;

	std::vector<Behaviour*> behaviours;

	VkDescriptorPool descriptorPool;
	std::vector<VkDescriptorSet> descriptorSets;

	//Texture
	VkImage textureImage;
	VkDeviceMemory textureImageMemory;
	VkImageView textureImageView;
	VkSampler textureSampler;
	std::string _texture_path;

	int entityTag;

	short int dimension;


	void Cleanup(VkDevice logicalDevice, std::vector<VkImage> swapChainImages) {
		vkDestroyBuffer(logicalDevice, vertexBuffer, nullptr);
		vkDestroyBuffer(logicalDevice, indexBuffer, nullptr);
		vkFreeMemory(logicalDevice, vertexBufferMemory, nullptr);
		vkFreeMemory(logicalDevice, indexBufferMemory, nullptr);

		vkDestroyDescriptorPool(logicalDevice, descriptorPool, nullptr);
	}

	void CreateAllBuffers(VkDevice logicalDevice, VkPhysicalDevice physicalDevice, VkCommandPool commandPool, std::vector<VkImage> swapChainImages, VkQueue graphicsQueue) {
		CreateVertexBuffer(logicalDevice, commandPool, graphicsQueue, physicalDevice);
		CreateIndexBuffer(logicalDevice, commandPool, graphicsQueue, physicalDevice);
		CreateUniformBuffer(logicalDevice, physicalDevice, swapChainImages);
	}
	void CreateAllDescriptorsSets(VkDevice logicalDevice, std::vector<VkImage> swapChainImages, VkDescriptorSetLayout descriptorSetLayout, VkImageView textureImageView, VkSampler textureSampler) {
		CreateDescriptorPool(logicalDevice, swapChainImages);
		CreateDescriptorSets(logicalDevice, swapChainImages, descriptorSetLayout, textureImageView, textureSampler);
	}
	void CreateAllDescriptorsSets(VkDevice logicalDevice, std::vector<VkImage> swapChainImages, VkDescriptorSetLayout descriptorSetLayout) {
		CreateDescriptorPool(logicalDevice, swapChainImages);
		CreateDescriptorSets(logicalDevice, swapChainImages, descriptorSetLayout, textureImageView, textureSampler);
	}


	void CreateVertexBuffer(VkDevice logicalDevice, VkCommandPool commandPool, VkQueue graphicsQueue, VkPhysicalDevice physicalDevice){
		VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();;

		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;
		CreateBuffer(logicalDevice, physicalDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

		void* data;
		vkMapMemory(logicalDevice, stagingBufferMemory, 0, bufferSize, 0, &data);
		memcpy(data, vertices.data(), (size_t)bufferSize);
		vkUnmapMemory(logicalDevice, stagingBufferMemory);

		CreateBuffer(logicalDevice, physicalDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBuffer, vertexBufferMemory);

		CopyBuffer(logicalDevice, graphicsQueue, commandPool, stagingBuffer, vertexBuffer, bufferSize);

		vkDestroyBuffer(logicalDevice, stagingBuffer, nullptr);
		vkFreeMemory(logicalDevice, stagingBufferMemory, nullptr);
	}
	void CreateIndexBuffer(VkDevice logicalDevice, VkCommandPool commandPool, VkQueue graphicsQueue, VkPhysicalDevice physicalDevice) {
		VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();;

		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;
		CreateBuffer(logicalDevice, physicalDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

		void* data;
		vkMapMemory(logicalDevice, stagingBufferMemory, 0, bufferSize, 0, &data);
		memcpy(data, indices.data(), (size_t)bufferSize);
		vkUnmapMemory(logicalDevice, stagingBufferMemory);

		CreateBuffer(logicalDevice, physicalDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBuffer, indexBufferMemory);

		CopyBuffer(logicalDevice, graphicsQueue, commandPool, stagingBuffer, indexBuffer, bufferSize);

		vkDestroyBuffer(logicalDevice, stagingBuffer, nullptr);
		vkFreeMemory(logicalDevice, stagingBufferMemory, nullptr);
	}
	void CreateUniformBuffer(VkDevice logicalDevice, VkPhysicalDevice physicalDevice, std::vector<VkImage> swapChainImages) {
		VkDeviceSize bufferSize = sizeof(Object::UniformBufferObject);

		object.uniformBuffers.resize(swapChainImages.size());
		object.uniformBuffersMemory.resize(swapChainImages.size());

		for (size_t i = 0; i < swapChainImages.size(); i++) {
			CreateBuffer(logicalDevice, physicalDevice, bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, object.uniformBuffers[i], object.uniformBuffersMemory[i]);
		}
	}

	void CreateBuffer(VkDevice logicalDevice, VkPhysicalDevice physicalDevice, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer & vertexBuffer, VkDeviceMemory & vertexBufferMemory)
	{
		VkBufferCreateInfo bufferInfo = {};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = size;
		bufferInfo.usage = usage;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		if (vkCreateBuffer(logicalDevice, &bufferInfo, nullptr, &vertexBuffer) != VK_SUCCESS) {
			throw std::runtime_error("failed to create buffer!");
		}

		VkMemoryRequirements memRequirements;
		vkGetBufferMemoryRequirements(logicalDevice, vertexBuffer, &memRequirements);

		VkMemoryAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, physicalDevice, properties);

		if (vkAllocateMemory(logicalDevice, &allocInfo, nullptr, &vertexBufferMemory) != VK_SUCCESS) {
			throw std::runtime_error("failed to allocate buffer memory!");
		}

		vkBindBufferMemory(logicalDevice, vertexBuffer, vertexBufferMemory, 0);
	}
	void CopyBuffer(VkDevice logicalDevice, VkQueue graphicsQueue, VkCommandPool commandPool, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) {
		VkCommandBufferAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = commandPool;
		allocInfo.commandBufferCount = 1;

		VkCommandBuffer commandBuffer;
		vkAllocateCommandBuffers(logicalDevice, &allocInfo, &commandBuffer);

		VkCommandBufferBeginInfo beginInfo = {};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		vkBeginCommandBuffer(commandBuffer, &beginInfo);

		VkBufferCopy copyRegion = {};
		copyRegion.size = size;
		vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

		vkEndCommandBuffer(commandBuffer);

		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffer;

		vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
		vkQueueWaitIdle(graphicsQueue);

		vkFreeCommandBuffers(logicalDevice, commandPool, 1, &commandBuffer);
	}

	void CreateDescriptorPool(VkDevice logicalDevice, std::vector<VkImage> swapChainImages)
	{
		std::array<VkDescriptorPoolSize, 2> poolSize = {};
		poolSize[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		poolSize[0].descriptorCount = static_cast<uint32_t>(swapChainImages.size());
		poolSize[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		poolSize[1].descriptorCount = static_cast<uint32_t>(swapChainImages.size());

		VkDescriptorPoolCreateInfo poolInfo = {};
		poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		poolInfo.poolSizeCount = static_cast<uint32_t>(poolSize.size());;
		poolInfo.pPoolSizes = poolSize.data();
		poolInfo.maxSets = static_cast<uint32_t>(swapChainImages.size());;

		if (vkCreateDescriptorPool(logicalDevice, &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
			throw std::runtime_error("failed to create descriptor pool!");
		}
	}
	void CreateDescriptorSets(VkDevice logicalDevice, std::vector<VkImage> swapChainImages, VkDescriptorSetLayout descriptorSetLayout, VkImageView textureImageView, VkSampler textureSampler)
	{
		std::vector<VkDescriptorSetLayout> layouts(swapChainImages.size(), descriptorSetLayout);
		VkDescriptorSetAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = descriptorPool;
		allocInfo.descriptorSetCount = static_cast<uint32_t>(swapChainImages.size());
		allocInfo.pSetLayouts = layouts.data();

		descriptorSets.resize(swapChainImages.size());
		if (vkAllocateDescriptorSets(logicalDevice, &allocInfo, descriptorSets.data()) != VK_SUCCESS) {
			throw std::runtime_error("failed to allocate descriptor sets!");
		}

		for (size_t i = 0; i < swapChainImages.size(); i++) {
			VkDescriptorBufferInfo bufferInfo = {};
			bufferInfo.buffer = object.uniformBuffers[i];
			bufferInfo.offset = 0;
			bufferInfo.range = sizeof(Object::UniformBufferObject);

			VkDescriptorImageInfo imageInfo = {};
			imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			imageInfo.imageView = textureImageView;
			imageInfo.sampler = textureSampler;

			std::array<VkWriteDescriptorSet, 2> descriptorWrite = {};

			descriptorWrite[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorWrite[0].dstSet = descriptorSets[i];
			descriptorWrite[0].dstBinding = 0;
			descriptorWrite[0].dstArrayElement = 0;
			descriptorWrite[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			descriptorWrite[0].descriptorCount = 1;
			descriptorWrite[0].pBufferInfo = &bufferInfo;

			descriptorWrite[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorWrite[1].dstSet = descriptorSets[i];
			descriptorWrite[1].dstBinding = 1;
			descriptorWrite[1].dstArrayElement = 0;
			descriptorWrite[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			descriptorWrite[1].descriptorCount = 1;
			descriptorWrite[1].pImageInfo = &imageInfo;

			vkUpdateDescriptorSets(logicalDevice, static_cast<uint32_t>(descriptorWrite.size()), descriptorWrite.data(), 0, nullptr);
		}
	}

	uint32_t findMemoryType(uint32_t typeFilter, VkPhysicalDevice physicalDevice, VkMemoryPropertyFlags properties) {
		VkPhysicalDeviceMemoryProperties memProperties;
		vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

		for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
			if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
				return i;
			}
		}

		throw std::runtime_error("failed to find suitable memory type!");
	}

	void LoadTexture(VkDevice logicalDevice, VkPhysicalDevice physicalDevice, VkCommandPool commandPool, VkQueue graphicsQueue)
	{
		ImageLoader::CreateTextureImage(logicalDevice, physicalDevice, commandPool, graphicsQueue, textureImage, textureImageMemory, _texture_path);
		ImageLoader::CreateTextureImageView(logicalDevice, textureImage, textureImageView);
		ImageLoader::CreateTextureSampler(logicalDevice, textureSampler);
	}
};

namespace std {
	template<> struct hash<Entity::Vertex> {
		size_t operator()(Entity::Vertex const& vertex) const {
			return ((hash<glm::vec3>()(vertex.pos) ^ (hash<glm::vec3>()(vertex.color) << 1)) >> 1) ^ (hash<glm::vec2>()(vertex.texCoord) << 1);
		}
	};
}