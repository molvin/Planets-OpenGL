#pragma once
#include <glm/vec3.hpp>
#include <string>
class Material;

class Light
{

public:
	virtual void UploadToMaterial(Material& material) = 0;
	virtual void UploadToMaterial(const std::string& prefix, Material& material) = 0;

	glm::vec3 Position;
	glm::vec3 Color;
	float Intensity;
};
