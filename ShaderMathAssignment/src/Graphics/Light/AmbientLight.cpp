#include "AmbientLight.h"
#include "../Material.h"

void AmbientLight::UploadToMaterial(Material& material)
{
	UploadToMaterial("u_AmbientLight", material);
}
void AmbientLight::UploadToMaterial(const std::string& prefix, Material& material)
{
	material.SetUniform(prefix + ".Intensity", Intensity);
	material.SetUniform(prefix + ".Color", Color);
}
