#include "ModelLoader.h"


ModelLoader::ModelLoader()
{
}


ModelLoader::~ModelLoader()
{
}



void ModelLoader::LoadModel(std::string model_path, std::string texture_path)
{

	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string warn, err;

	if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, model_path.c_str())) {
		throw std::runtime_error(warn + err);
	}

	_texture_path = texture_path;

	std::unordered_map<Vertex, uint32_t> uniqueVertices = {};

	for (const auto& shape : shapes) {
		for (const auto& index : shape.mesh.indices) {
			Vertex vertex = {};

			vertex.pos = {
				attrib.vertices[3 * index.vertex_index + 0],
				attrib.vertices[3 * index.vertex_index + 1],
				attrib.vertices[3 * index.vertex_index + 2]
			};

			vertex.texCoord = {
				attrib.texcoords[2 * index.texcoord_index + 0],
				1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
			};

			vertex.color = { 1.0f, 1.0f, 1.0f };

			if (uniqueVertices.count(vertex) == 0) {
				uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
				vertices.push_back(vertex);
			}

			indices.push_back(uniqueVertices[vertex]);
		}
	}
}


void ModelLoader::Start() {
	/*int randomS = rand() % 70 + 1;

	object.transform.scale.x = randomS;
	object.transform.scale.y = randomS;
	object.transform.scale.z = randomS;*/
}
void ModelLoader::Update(VkDevice logicalDevice, uint32_t currentImage, Camera c, float n, float distance, float time) {

	//object.transform.n = n;
	//object.transform.distance = distance;

	//if (distance == 0) {
	//	object.transform.scale.x = 5000;
	//	object.transform.scale.y = 5000;
	//	object.transform.scale.z = 5000;
	//}
	//else if (distance == 1) {
	//	object.transform.scale.x = 200;
	//	object.transform.scale.y = 200;
	//	object.transform.scale.z = 200;
	//}
	//else {
	//	finalDistance = distance + 500;
	//}

	///*Transform.scale.x = n;
	//Transform.scale.y = n;
	//Transform.scale.z = n;*/
	////Entity Behaviour

	//int d = 30;

	////float time = (glfwGetTime() * glfwGetTime() / d);

	//object.transform.position.x = glm::sin((time / 50) * n);
	//object.transform.position.y = glm::cos((time / 80) * n);
	//object.transform.position.z = glm::cos((time / 50) * n) * glm::sin((time / 50) * n);

	//object.transform.position.x = glm::sin((time / 8) * n) * 1.1f;
	//object.transform.position.z = glm::cos((time / 8) * n);

	///*Transform.position.x = pow((1 - pow(Transform.position.y, 2)), 0.5f);

	//Transform.position.y = pow((1 - pow(Transform.position.x, 2)), 0.5f);*/

	////Do not touch this unless you know what it means
	//glm::mat4 model(1);

	//object.UBO.model = glm::translate(model, glm::vec3(object.transform.position.x * finalDistance, object.transform.position.y * finalDistance, object.transform.position.z * finalDistance)) * glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f) * 0.3f * n, glm::vec3(0.0f, 0.0f, 1.0f));
	//object.UBO.view = glm::lookAt(c.cameraPos, c.cameraPos + c.cameraFront, c.cameraUp);
	//object.UBO.scale = glm::vec3(object.transform.scale.x, object.transform.scale.y, object.transform.scale.z);
	//object.UBO.proj = c.GetProjection();
	//object.UBO.ucolors = glm::vec4(1, 1, 1, 1);
	//object.UBO.proj[1][1] *= -1;


	//void* data;
	//vkMapMemory(logicalDevice, object.uniformBuffersMemory[currentImage], 0, sizeof(object.UBO), 0, &data);
	//memcpy(data, &object.UBO, sizeof(object.UBO));
	//vkUnmapMemory(logicalDevice, object.uniformBuffersMemory[currentImage]);
}
