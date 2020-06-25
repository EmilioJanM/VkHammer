#pragma once

#include <iostream>
#include <vector>

#include "Entity.h"

class Entities
{
public:
	Entities();
	~Entities();

	uint32_t totalIndices = 0;
	uint32_t totalVertices = 0;
	uint32_t totalEntities = 0;

	std::vector<VkDescriptorSet> allDescriptorSets;
	std::vector<VkDeviceMemory> allUniformBuffers;


	std::vector<Entity*> allEntities;

	void AddEntity(Entity *ent) {
		Entities::totalEntities++;
		Entities::totalVertices += ent->vertices.size();
		Entities::totalIndices += ent->indices.size();
		Entities::allEntities.push_back(ent);
		//for (size_t i = 0; i != ent->descriptorSets.size(); i++)
		//{
		//	Entities::allDescriptorSets.push_back(ent->descriptorSets[i]);
		//}
	}
};

