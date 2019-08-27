#include "Renderer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "VertexBuffer.h"
#include <glm/glm.hpp>

Renderer::SceneData* Renderer::_sceneData = new SceneData;

void Renderer::Init()
{
	glEnable(GL_DEPTH_TEST);
}
void Renderer::Begin(const glm::mat4& viewProjectionMatrix)
{
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	_sceneData->viewProjection = viewProjectionMatrix;
}
void Renderer::Render(const Shader* shader, const VertexArray* vao, const glm::mat4& transform)
{
	vao->Bind();
	shader->Bind();

	shader->UploadUniformMat4("u_Transform", _sceneData->viewProjection * transform);

	glDrawElements(GL_TRIANGLES, vao->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
}
