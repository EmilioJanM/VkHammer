#include "Entity.h"



Entity::Entity()
{
}


Entity::~Entity()
{
}

//void Entity::UpdateE(VkDevice logicalDevice, uint32_t currentImage, Camera c, float n, float distance) {
//
//	Transform.n = n;
//	Transform.distance = distance;
//
//	//Entity Behaviour
//	
//	int d = 30;
//
//	float time = (glfwGetTime() * glfwGetTime() / d);
//
//	Transform.position.x = glm::sin((time / 8) * Transform.n) * Transform.distance;
//	Transform.position.y = glm::cos((time / 8) * Transform.n) * Transform.distance;
//
//
//
//	//Do not touch this unless you know what it means
//	glm::mat4 model(1);
//
//	UBO.model = glm::translate(model, glm::vec3(Transform.position.x, Transform.position.y, Transform.position.z)) * glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f) * 0.3f * n, glm::vec3(0.0f, 0.0f, 1.0f));
//	UBO.view = glm::lookAt(c.cameraPos, c.cameraPos + c.cameraFront, c.cameraUp);
//	UBO.proj = c.GetProjection();
//	UBO.ucolors = glm::vec4(1, 1, 1, 1);
//	UBO.proj[1][1] *= -1;
//
//
//	void* data;
//	vkMapMemory(logicalDevice, uniformBuffersMemory[currentImage], 0, sizeof(UBO), 0, &data);
//	memcpy(data, &UBO, sizeof(UBO));
//	vkUnmapMemory(logicalDevice, uniformBuffersMemory[currentImage]);
//}
