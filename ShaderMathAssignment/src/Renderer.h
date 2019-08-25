#pragma once
#include <glm/mat4x4.hpp>
class Shader;
class VertexArray;

class Renderer
{
public:
	static void SetSceneData(const glm::mat4& viewProjectionMatrix);
	static void Render(const ::Shader* shader, const ::VertexArray* vao, const glm::mat4& transform);
private:
	struct SceneData
	{
		glm::mat4 viewProjection;
	};

	static SceneData* _sceneData;
};

