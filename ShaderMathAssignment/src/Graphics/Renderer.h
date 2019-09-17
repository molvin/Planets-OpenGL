#pragma once
#include <glm/mat4x4.hpp>
class Material;
class Shader;
class VertexArray;
class Mesh;

class Renderer
{
public:
	static void Init();
	static void Begin(const glm::mat4& viewProjectionMatrix);
	static void Render(const Material* material, const ::VertexArray* vao, const glm::mat4& transform);
	static void RenderFrameBuffer(const Material& material);
private:
	struct SceneData
	{
		glm::mat4 viewProjection;
	};

	static SceneData* _sceneData;
};

