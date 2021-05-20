#include <GL/glew.h>

#include "VertexArray.h"

VertexArray::VertexArray()
{
  glGenVertexArrays(1, &m_RendererID);
}

VertexArray::~VertexArray()
{
  glDeleteVertexArrays(1, &m_RendererID);
}

void VertexArray::AddBuffer(const VertexBuffer& vertexBuffer, const VertexBufferLayout& layout)
{
  Bind();
  vertexBuffer.Bind();
  const auto& elements = layout.GetElements();
  uint32_t offset = 0;

  for (uint32_t i = 0; i < elements.size(); i++)
  {
    const auto& element = elements[i];
    glEnableVertexAttribArray(i);
    glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void*) offset);
    offset += element.count * VertexBufferLayout::GetSizeOfType(element.type);
  }
};

void VertexArray::Bind() const {
  glBindVertexArray(m_RendererID);
}

void VertexArray::UnBind() const {
  glBindVertexArray(0);
}
