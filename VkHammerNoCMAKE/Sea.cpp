//#include "Sea.h"
//
//
//
//Sea::Sea(int _x, int _y, int _z, float _w, float _h) : w(_w), h(_h)
//{
//	position.x = _x; position.y = _y; position.z = _z;
//
//	srand(time(NULL));
//	float length = h / 2; float wide = w / 2;
//
//	if (h == 1)
//		length = 0.5f;
//	if (w == 1)
//		wide = 0.5f;
//	int count = 0;
//	for (float i = -length; i != length + 1; i++) {
//		for (float j = -wide; j != wide + 1; j++) {
//			Node *n = new Node();
//			n->x = i;
//			n->z = j;
//			n->vertexInt = count;
//			nodes.push_back(n);
//			Entity::Vertex b;
//			//if ((int)j % 2 == 0)
//			//	n->y = 5;
//			b.pos = glm::vec3(n->x, n->y, n->z);
//			b.color = glm::vec3(1, 1, 1);
//			vertices.push_back(b);
//			vel.push_back((static_cast <float> (rand()) / static_cast <float> (RAND_MAX))*0.04f);
//		}
//	}
//
//	int wBuff = w; int size = ((h + 1)*(w + 1)) - w - 1;
//	meshTotalVertex = ((h + 1)*(w + 1));
//	for (size_t i = 0; i != size; i++) {
//		if (i != wBuff) {
//			indices.push_back(i);	indices.push_back(i + 1);	indices.push_back(i + w + 1);
//			indices.push_back(i + w + 1);	indices.push_back(i + w + 2);	indices.push_back(i + 1);
//		}
//		else
//			wBuff += w + 1;
//	}
//}
//
//Sea::~Sea()
//{
//}
//
//void Sea::Update(VkDevice logicalDevice, Camera camera, short int currentImage, VkCommandPool commandPool, VkQueue graphicsQueue, VkPhysicalDevice physicalDevice)
//{
//	//unsigned int vertex;
//	/*for (size_t i = 0; i != vertices.size(); i++) {
//		vertices[i].pos.y += vel[i];
//		if (vertices[i].pos.y > 2.f)
//			vel[i] *= -1;
//		else if (vertices[i].pos.y < 0)
//			vel[i] *= -1;
//	}*/
//
//	vertices[0].pos.y++;
//
//	glm::mat4 model(1); glm::mat4 rotation(1);
//
//	UBO.model = glm::translate(model, glm::vec3(position.x, position.y, position.z)) * glm:: rotate(rotation, glm::radians(rot), glm::vec3(1, 0, 0));
//	UBO.view = glm::lookAt(camera.cameraPos, camera.cameraPos + camera.cameraFront, camera.cameraUp);
//	UBO.proj = camera.GetProjection();
//	//UBO.ucolors = glm::vec4(time, time * 5, time, time * 10);
//	UBO.proj[1][1] *= -1;
//	UBO.time = 1;
//
//
//	void* data2;
//	vkMapMemory(logicalDevice, uniformBuffersMemory[currentImage], 0, sizeof(UBO), 0, &data2);
//	memcpy(data2, &UBO, sizeof(UBO));
//	vkUnmapMemory(logicalDevice, uniformBuffersMemory[currentImage]);
//}