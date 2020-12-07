//
// Created by Neresis on 2020. 10. 16..
//

#ifndef SLIMSHADY_INDEXBUFFER_H
#define SLIMSHADY_INDEXBUFFER_H


#include <cstdint>

class IndexBuffer
{
private:
    uint32_t m_RendererID;
    uint32_t m_Count;
    uint32_t m_Size;
public:
    IndexBuffer(const uint32_t* data, uint32_t count);
    ~IndexBuffer();

    void Bind() const;
    void UnBind() const;

    inline uint32_t GetCount() const {return m_Count;}
};


#endif //SLIMSHADY_INDEXBUFFER_H
