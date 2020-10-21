#include "VertexBufferLayout.h"

std::vector<VertexBufferLayout::VertexBufferElement> VertexBufferLayout::GetElements() const
{
  return m_Elements;
}

unsigned int VertexBufferLayout::GetStride() const
{
  return m_Stride;
}
