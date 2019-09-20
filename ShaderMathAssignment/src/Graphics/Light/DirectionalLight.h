#pragma once
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include "Light.h"
#include "../Texture.h"

class DirectionalLight : public Light
{
public:
	glm::vec3 Direction = glm::vec3(-1.0f);
	glm::vec3 Color = glm::vec3(1.0f);
	float Intensity = 1.0f;
	glm::mat4 GetLightProjection() const;
	glm::mat4 GetLightView() const;
	void DrawGui();
	void UploadToMaterial(Material& material) override;
	void UploadToMaterial(const std::string& prefix, Material& material) override;

};

