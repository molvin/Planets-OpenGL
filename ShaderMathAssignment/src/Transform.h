#pragma once
#include <glm/vec3.hpp>
#include <glm/gtx/quaternion.hpp>

//TODO: add euler angle handling for ease of use, and ImGui

class Transform
{
public:
	glm::vec3 Position = glm::vec3(0.0f);
	glm::quat Rotation = glm::identity<glm::quat>();
	glm::vec3 Scale = glm::vec3(1.0f);

	void Rotate(float angle, glm::vec3 axis);
	glm::mat4 GetMatrix() const;
	Transform operator *(const Transform& other) const;

	//TODO: multiply transforms, use glm decompose to get components from resulting matrix
};

