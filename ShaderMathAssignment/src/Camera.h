#pragma once
#include <glm/vec3.hpp>
#include <glm/mat4x2.hpp>

class Camera
{
public:
	Camera();

	glm::vec3 Position = glm::vec3(0.0f);
	glm::vec3 Direction = glm::vec3(0.0f, 0.0f, 1.0f);

	glm::mat4 GetViewMatrix();
};

