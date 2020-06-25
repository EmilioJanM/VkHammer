//#pragma once
//
//#include <time.h>
//
//#include "Entity.h"
//#include "Node.h"
//
//class Sea : public Entity
//{
//public:
//	Sea(int _x, int _y, int _z, float _w, float _h);
//	~Sea();
//
//	float w, h, rot;
//	bool init = false;
//
//	std::vector<float> mesh;
//	std::vector<float> meshExtra;
//	std::vector<uint32_t> meshEBO;
//	std::vector<Node*> nodes;
//	std::vector<float> vel;
//
//	void Update(VkDevice logicalDevice, Camera camera, short int currentImage, VkCommandPool commandPool, VkQueue graphicsQueue, VkPhysicalDevice physicalDevice);
//
//private:
//	unsigned int meshTotalVertex;
//};
//
