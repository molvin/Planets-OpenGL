#pragma once
#include "Light.h"
class AmbientLight : public Light
{
public:
	void UploadToMaterial(Material& material) override;
	void UploadToMaterial(const std::string& prefix, Material& material) override;
	
};

