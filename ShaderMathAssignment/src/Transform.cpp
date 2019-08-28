#include "Transform.h"

void Transform::Rotate(float angle, glm::vec3 axis)
{
	Rotation = glm::rotate(Rotation, angle, axis);
}
glm::mat4 Transform::GetMatrix()
{
	const glm::mat4 translation = glm::translate(glm::mat4(1.0f), Position);
	const glm::mat4 rotation = glm::toMat4(Rotation);
	const glm::mat4 scale = glm::scale(glm::mat4(1.0f), Scale);
	return translation * rotation * scale;
}
