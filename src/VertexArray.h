#ifndef SLIMSHADY_VERTEXARRAY_H
#define SLIMSHADY_VERTEXARRAY_H

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

class VertexArray
{
private:
    uint32_t m_RendererID;

public:
    VertexArray();
    ~VertexArray();

    void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
    void Bind() const;
    void UnBind() const;
};


#endif //SLIMSHADY_VERTEXARRAY_H
