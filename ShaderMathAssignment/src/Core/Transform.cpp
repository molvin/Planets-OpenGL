#include "Transform.h"
#include "glm/gtx/matrix_decompose.hpp"



void Transform::Rotate(float angle, glm::vec3 axis)
{
	Rotation = glm::rotate(Rotation, angle, axis);
}
void Transform::SetEuler(const glm::vec3 value)
{
	_euler = value;
	Rotation = glm::quat(glm::radians(_euler));
}
glm::mat4 Transform::GetMatrix() const
{
	const glm::mat4 translation = glm::translate(glm::mat4(1.0f), Position);
	const glm::mat4 rotation = glm::toMat4(Rotation);
	const glm::mat4 scale = glm::scale(glm::mat4(1.0f), Scale);
	return translation * rotation * scale;
}

Transform Transform::operator*(const Transform& other) const
{
	const glm::mat4 matrix = GetMatrix();
	const glm::mat4 otherMatrix = other.GetMatrix();

	const glm::mat4 resultMatrix = matrix * otherMatrix;
	Transform resultTransform;
	glm::vec3 skew;
	glm::vec4 perspective;

	decompose(resultMatrix, resultTransform.Scale, resultTransform.Rotation, resultTransform.Position, skew, perspective);
	return resultTransform;
}
