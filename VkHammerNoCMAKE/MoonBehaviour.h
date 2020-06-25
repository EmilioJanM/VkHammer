#pragma once

#include "Behaviour.h"

class MoonBehaviour : public Behaviour
{
public:
	MoonBehaviour(Object& object, float n, float distance);

	

	Object object; //This entity, the object to which the script is linked 

	float n; 
	float distance; 
	float time = 0;
	float finalDistance;

	void Start();
	void Update(VkDevice logicalDevice, uint32_t currentImage, Camera c, float time);
};

