#pragma once
#include "Graphics/Mesh.h"
#include "Graphics/Material.h"

class Renderable
{
public:
	Renderable() = default;
	Renderable(Mesh* mesh, Material* material);
	void Render() const;
	const Mesh* GetMesh() const { return _mesh; }
	const Material* GetMaterial() const { return _material; }
	Transform* GetTransform() { return &_transform; }
	std::string GetSaveFormat() const;
private:
	Mesh* _mesh = nullptr;
	Material* _material = nullptr;
	Transform _transform;
};

