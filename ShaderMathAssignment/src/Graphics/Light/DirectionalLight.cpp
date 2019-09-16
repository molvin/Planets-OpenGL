#include "DirectionalLight.h"
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include "../../ImGUI/imgui.h"
#include "../Material.h"

glm::mat4 DirectionalLight::GetLightProjection() const
{
	return glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, -100.f, 100.0f);
}
glm::mat4 DirectionalLight::GetLightView() const
{
	return glm::lookAt(glm::vec3(0.0f), Direction, glm::vec3(0.0f, 1.0f, 0.0f));
}
void DirectionalLight::DrawGui()
{
	ImGui::Begin("Directional Light");
	ImGui::SliderFloat3("Direction", &Direction[0], -1.0f, 1.0f);
	ImGui::InputFloat("Intensity", &Intensity);
	ImGui::ColorEdit3("Color", &Color[0]);
	Direction = normalize(Direction);
	ImGui::End();
}

void DirectionalLight::UploadToMaterial(const std::string& prefix, Material& material)
{
	material.SetUniform(prefix + ".Direction", Direction);
	material.SetUniform(prefix + ".Color", Color);
	material.SetUniform(prefix + ".ShadowBuffer", 1);
	material.SetUniform(prefix + ".ViewProjection", GetLightProjection() * GetLightView());
}
