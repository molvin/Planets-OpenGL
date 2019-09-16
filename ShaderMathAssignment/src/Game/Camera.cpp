#include "Camera.h"
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/quaternion_float.hpp>
#include <glm/gtx/quaternion.hpp>
#include "../Application/Input.h"
#include "../ImGUI/imgui.h"
#include "../Core/Time.h"

Camera::Camera(glm::vec3 position) : Position(position)
{
}

void Camera::Update()
{
	if (Input::GetMouseButton(GLFW_MOUSE_BUTTON_2))
	{
		const glm::vec2 mouseDir = Input::GetMouseDirection() * RotationSpeed * Time::GetDeltaTime();
		Rotate(mouseDir.x, glm::vec3(0.0f, 1.0f, 0.0f));
		Rotate(mouseDir.y, GetRight());
	}
	const float horizontal = Input::GetKey(GLFW_KEY_D) ? -1 : Input::GetKey(GLFW_KEY_A) ? 1 : 0;
	const float vertical = Input::GetKey(GLFW_KEY_S) ? -1 : Input::GetKey(GLFW_KEY_W) ? 1 : 0;
	const float height = Input::GetKey(GLFW_KEY_Q) ? -1 : Input::GetKey(GLFW_KEY_E) ? 1 : 0;
	Position += GetForward() * vertical * Speed * Time::GetDeltaTime();
	Position += -GetRight() * horizontal * Speed * Time::GetDeltaTime();
	Position += glm::vec3(0.0f, 1.0f, 0.0f) * height * Speed * Time::GetDeltaTime();
}
void Camera::Rotate(const float angle, const glm::vec3& axis)
{
	const glm::quat rotation = glm::angleAxis(glm::radians(angle), axis);
	Direction = rotation * Direction;
}
void Camera::DrawGui()
{
	ImGui::Begin("Camera");
	ImGui::InputFloat3("Position", &Position[0]);
	ImGui::SliderFloat3("Direction", &Direction[0], -1.0f, 1.0f);
	Direction = glm::normalize(Direction);
	ImGui::End();
}
glm::mat4 Camera::GetViewMatrix() const
{
	return glm::lookAt(Position, Position + Direction, glm::vec3(0.0f, 1.0f, 0.0f));
}
