#pragma once
#include <glm/vec3.hpp>
#include <glm/mat4x2.hpp>

class Camera
{
public:
	Camera(glm::vec3 position);
	void Update();
	void Rotate(const float angle, const glm::vec3& axis);
	void DrawGui();
	
	glm::vec3 GetForward() const { return glm::normalize(Direction); }
	glm::vec3 GetRight() const { return glm::normalize(glm::cross(glm::normalize(Direction), glm::vec3(0.0f, 1.0f, 0.0f))); }
	glm::mat4 GetViewMatrix() const;

	glm::vec3 Position = glm::vec3(0.0f);
	glm::vec3 Direction = glm::vec3(0.0f, 0.0f, 1.0f);

private:
	const float Speed = 5.0f;
	const float RotationSpeed = 125.0f;
};

