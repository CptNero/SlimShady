#include <glew.h>

#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(const void *data, uint32_t size)
{
  uint32_t buffer;
  glGenBuffers(1, &m_RendererID);
  glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
  glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer()
{
  glDeleteBuffers(1, &m_RendererID);
}

void VertexBuffer::Bind() const
{
  glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
}

void VertexBuffer::UnBind() const
{
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}
