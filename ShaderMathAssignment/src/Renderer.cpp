#include "Renderer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "VertexBuffer.h"
#include <glm/glm.hpp>

Renderer::SceneData* Renderer::_sceneData = new SceneData;

void Renderer::SetSceneData(const glm::mat4& viewProjectionMatrix)
{
	_sceneData->viewProjection = viewProjectionMatrix;
}
void Renderer::Render(const Shader* shader, const VertexArray* vao, const glm::mat4& transform)
{
	shader->Bind();
	vao->Bind();

	shader->UploadUniformMat4("u_Transform", _sceneData->viewProjection * transform);

	glDrawElements(GL_TRIANGLES, vao->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
}
