#include <glew.h>
#include "Renderer.h"

Renderer::Renderer() = default;

Renderer::~Renderer() = default;

void Renderer::Clear() const {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::Draw(const VertexArray& vertexArray, const IndexBuffer& indexBuffer, const Shader& shader) {
  shader.Bind();
  vertexArray.Bind();
  indexBuffer.Bind();
  glDrawElements(GL_TRIANGLES, indexBuffer.GetCount(), GL_UNSIGNED_INT, nullptr);
}


