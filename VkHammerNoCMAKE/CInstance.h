#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <optional>
#include <set>

class CInstance
{
private:

	bool checkValidationLayerSupport();

public:

	static const std::vector<const char*> validationLayers;

#ifdef NDEBUG
static const bool debugEnabled = false;
#else
static const bool debugEnabled = true;
#endif

	VkInstance CreateInstance(const char* AppName);

	CInstance();
	~CInstance();

};