#include "Renderable.h"
#include "Graphics/Renderer.h"
#include <sstream>

Renderable::Renderable(Mesh* mesh, Material* material)
{
	_mesh = mesh;
	_material = material;
}
void Renderable::Render() const
{
	if (_mesh == nullptr || _material == nullptr)
		return;

	Renderer::Render(_material, _mesh->GetVertexArray(), _transform.GetMatrix());
}

std::string Renderable::GetSaveFormat() const
{
	std::stringstream save;
	save << "Renderable: \n";
	save << "{\n";

	//Mesh reference id
	save << 0 << "\n";
	//Material reference id
	save << 0 << "\n";
	//Transform
	save << _transform.Position.x << "\n";
	save << _transform.Position.y << "\n";
	save << _transform.Position.z << "\n";
	save << _transform.Scale.x << "\n";
	save << _transform.Scale.y << "\n";
	save << _transform.Scale.z << "\n";
	save << _transform.Rotation.x << "\n";
	save << _transform.Rotation.y << "\n";
	save << _transform.Rotation.z << "\n";
	save << _transform.Rotation.w << "\n";

	save << "}\n";
	return save.str();
}
