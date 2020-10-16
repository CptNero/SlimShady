#ifndef SLIMSHADY_VERTEXBUFFER_H
#define SLIMSHADY_VERTEXBUFFER_H


class VertexBuffer
{
private:
    unsigned int m_RendererID;
public:
    VertexBuffer(const void* data, unsigned int size);
    ~VertexBuffer();

    void Bind() const;
    void UnBind() const;
};


#endif //SLIMSHADY_VERTEXBUFFER_H
