#include "Camera.h"
#include <glm/ext/matrix_transform.hpp>


Camera::Camera()
{
}
glm::mat4 Camera::GetViewMatrix()
{
	return glm::lookAt(Position, Position + Direction, glm::vec3(0.0f, 1.0f, 0.0f));
}
