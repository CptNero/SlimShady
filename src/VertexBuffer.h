#ifndef SLIMSHADY_VERTEXBUFFER_H
#define SLIMSHADY_VERTEXBUFFER_H


#include <cstdint>

class VertexBuffer
{
public:
    VertexBuffer(const void* data, uint32_t size);
    ~VertexBuffer();

    void Bind() const;
    void UnBind() const;

private:
    uint32_t m_RendererID;
};


#endif //SLIMSHADY_VERTEXBUFFER_H
