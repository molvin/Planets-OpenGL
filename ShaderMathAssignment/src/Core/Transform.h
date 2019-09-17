#pragma once
#include <glm/vec3.hpp>
#include <glm/gtx/quaternion.hpp>

//TODO: Add parent and apply in GetMatrix

class Transform
{
public:
	glm::vec3 Position = glm::vec3(0.0f);
	glm::quat Rotation = glm::identity<glm::quat>();
	glm::vec3 Scale = glm::vec3(1.0f);

	const glm::vec3& GetEuler() const { return _euler; }
	void SetEuler(const glm::vec3 value);
	void Rotate(float angle, glm::vec3 axis);
	glm::mat4 GetMatrix() const;
	Transform operator *(const Transform& other) const;
private:
	glm::vec3 _euler = glm::vec3(0.0f);
};

