#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"


class Camera {
public:
	float xx = 2.0;
	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)960 / (float)540, 0.1f, 100000.0f);
	glm::mat4 rotation{ 0 };
	glm::mat4 view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));

	glm::mat4 GetProjection() {
		return projection;
	}
	glm::mat4 GetView() {
		//return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		return glm::lookAt(glm::vec3(xx, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	}
	float cameraSpeed = 3.f;
	float cameraNormalSpeed = cameraSpeed;


	void SetPos(int x, int y, int z) { cameraPos = glm::vec3(x, y, z); }
	void SetRot(const float angle, const float x, const float y, const float z) { rotation = glm::rotate(rotation, glm::radians(angle), glm::vec3(x, y, z)); }
	void ParentPos(int x, int y, int z) {
		cameraPos.x = x;
		cameraPos.y = y;
		cameraPos.z = z;
	}

};

