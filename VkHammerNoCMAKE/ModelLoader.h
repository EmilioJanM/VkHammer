#pragma once

#include "Entity.h"
#include "ImageLoader.h"

#include <iostream>
#include <string>
#include <unordered_map>

#include "tiny_obj_loader.h"

#include <random>     
#include <time.h>
#include <math.h>

class ModelLoader : public Entity
{
private:
	ImageLoader imageLoader;

public:
	ModelLoader();
	~ModelLoader();
/*
	std::string MODEL_PATH;
	std::string TEXTURE_PATH;*/

	void LoadModel(std::string model_path, std::string texture_path);

	float time = 0;
	float finalDistance;

	void Start();
	void Update(VkDevice logicalDevice, uint32_t currentImage, Camera c, float n, float distance, float time);

	static struct ModelInfo
	{
		VkDevice logicalDevice;
		VkPhysicalDevice physicalDevice;
		VkCommandPool commandPool;
		VkQueue graphicQueue;
	}modelInfo;
};