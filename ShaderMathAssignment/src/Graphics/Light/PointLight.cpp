#include "PointLight.h"
#include "../Material.h"

void PointLight::UploadToMaterial(Material& material)
{
	UploadToMaterial("u_PointLight", material);
}

void PointLight::UploadToMaterial(const std::string& prefix, const int index, Material& material)
{
	UploadToMaterial(prefix + "[" + std::to_string(index) + "]", material);
}
void PointLight::UploadToMaterial(const std::string& prefix, Material& material)
{
	material.SetUniform(prefix + ".Position", Position);
	material.SetUniform(prefix + ".Color", Color);
	material.SetUniform(prefix + ".Radius", Radius);
	material.SetUniform(prefix + ".Intensity", Intensity);
}
