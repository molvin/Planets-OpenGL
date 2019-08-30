#include "Camera.h"
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/quaternion_float.hpp>
#include <glm/gtx/quaternion.hpp>


Camera::Camera()
{
}

void Camera::Rotate(const float angle, const glm::vec3& axis)
{
	glm::quat rotation = glm::angleAxis(glm::radians(angle), axis);
	Direction = rotation * Direction;
}


glm::mat4 Camera::GetViewMatrix()
{
	return glm::lookAt(Position, Position + Direction, glm::vec3(0.0f, 1.0f, 0.0f));
}
