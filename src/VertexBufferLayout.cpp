#include "VertexBufferLayout.h"

std::vector<VertexBufferLayout::VertexBufferElement> VertexBufferLayout::GetElements() const
{
  return m_Elements;
}

uint32_t VertexBufferLayout::GetStride() const
{
  return m_Stride;
}
