#pragma once
#include "Light.h"
#include <string>

class PointLight : public Light
{
public:
	void UploadToMaterial(Material& material) override;
	void UploadToMaterial(const std::string& prefix, int index, Material& material);
	void UploadToMaterial(const std::string& prefix, Material& material) override;

	float Radius;
};



