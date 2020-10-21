#ifndef SLIMSHADY_VERTEXBUFFERLAYOUT_H
#define SLIMSHADY_VERTEXBUFFERLAYOUT_H

#include <glew.h>

#include <vector>

class VertexBufferLayout
{
private:
    struct VertexBufferElement
    {
        unsigned int type;
        unsigned int count;
        unsigned char normalized;
    };
    std::vector<VertexBufferElement> m_Elements;
    unsigned int m_Stride;
public:
    VertexBufferLayout()
            : m_Stride(0) {}

    template<typename T>
    void Push(unsigned int count) {}

    template<>
    void Push<float>(unsigned int count)
    {
      m_Elements.push_back({ GL_FLOAT, count, GL_FALSE });
      m_Stride += GetSizeOfType(GL_FLOAT) * count;
    }

    template<>
    void Push<unsigned int>(unsigned int count)
    {
      m_Elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
      m_Stride += GetSizeOfType((GL_UNSIGNED_INT)) * count;
    }

    template<>
    void Push<unsigned char>(unsigned int count)
    {
      m_Elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
      m_Stride += GetSizeOfType(GL_UNSIGNED_BYTE) * count;
    }

    std::vector<VertexBufferElement> GetElements() const;

    unsigned int GetStride() const;

    static unsigned int GetSizeOfType(unsigned int type)
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

#endif //SLIMSHADY_VERTEXBUFFERLAYOUT_H
