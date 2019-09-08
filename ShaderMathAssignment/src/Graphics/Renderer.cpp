#include "Renderer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "VertexBuffer.h"
#include <glm/glm.hpp>
#include "Material.h"
#include "Mesh.h"
#include <GLFW/glfw3.h>


void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	return;
	fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
		(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
		type, severity, message);
}

Renderer::SceneData* Renderer::_sceneData = new SceneData;

void Renderer::Init()
{
	glEnable(GL_DEPTH_TEST);
	glfwSwapInterval(0);
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(MessageCallback, 0);
}
void Renderer::Begin(const glm::mat4& viewProjectionMatrix)
{
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	_sceneData->viewProjection = viewProjectionMatrix;
}

void Renderer::Render(Mesh* mesh)
{
	const glm::mat4 matrix = mesh->GetTransform()->GetMatrix();
	Renderer::Render(mesh->GetMaterial(), mesh->GetVertexArray(), matrix);
}

void Renderer::Render(const Material* material, const VertexArray* vao, const glm::mat4& transform)
{
	vao->Bind();
	material->Bind();

	material->GetShader()->UploadUniformMat4("u_ViewProjection", _sceneData->viewProjection);
	material->GetShader()->UploadUniformMat4("u_World", transform);

	glDrawElements(GL_TRIANGLES, vao->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
}

void Renderer::RenderFrameBuffer()
{
	glBindVertexArray(0);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}
