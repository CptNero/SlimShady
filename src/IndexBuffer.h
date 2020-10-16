//
// Created by Neresis on 2020. 10. 16..
//

#ifndef SLIMSHADY_INDEXBUFFER_H
#define SLIMSHADY_INDEXBUFFER_H


class IndexBuffer
{
private:
    unsigned int m_RendererID;
    unsigned int m_Count;
    unsigned int m_Size;
public:
    IndexBuffer(const unsigned int* data, unsigned int count);
    ~IndexBuffer();

    void Bind() const;
    void UnBind() const;

    inline unsigned int GetCount() const {return m_Count;}
};


#endif //SLIMSHADY_INDEXBUFFER_H
