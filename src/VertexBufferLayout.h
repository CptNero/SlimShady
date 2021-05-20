#ifndef SLIMSHADY_VERTEXBUFFERLAYOUT_H
#define SLIMSHADY_VERTEXBUFFERLAYOUT_H

#include <GL/glew.h>

#include <vector>

class VertexBufferLayout
{
private:
    struct VertexBufferElement
    {
        uint32_t type;
        uint32_t count;
        uint32_t normalized;
    };
    std::vector<VertexBufferElement> m_Elements;
    uint32_t m_Stride;
public:
    VertexBufferLayout()
            : m_Stride(0) {}

    template<typename T>
    void Push(uint32_t count) {}

    std::vector<VertexBufferElement> GetElements() const;

    uint32_t GetStride() const;

    static uint32_t GetSizeOfType(uint32_t type)
    {
      switch (type)
      {
        case GL_FLOAT: return 4;
        case GL_UNSIGNED_INT: return 4;
        case GL_UNSIGNED_BYTE: return 1;
      }

      return 0;
    }
};

template<> inline
void VertexBufferLayout::Push<float>(uint32_t count)
{
  m_Elements.push_back({ GL_FLOAT, count, GL_FALSE });
  m_Stride += GetSizeOfType(GL_FLOAT) * count;
}

template<> inline
void VertexBufferLayout::Push<uint32_t>(uint32_t count)
{
  m_Elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
  m_Stride += GetSizeOfType((GL_UNSIGNED_INT)) * count;
}

template<> inline
void VertexBufferLayout::Push<unsigned char>(uint32_t count)
{
  m_Elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
  m_Stride += GetSizeOfType(GL_UNSIGNED_BYTE) * count;
}




#endif //SLIMSHADY_VERTEXBUFFERLAYOUT_H
