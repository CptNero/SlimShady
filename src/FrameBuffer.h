//
// Created by neresis on 2021. 04. 10..
//

#ifndef SLIMSHADY_FRAMEBUFFER_H
#define SLIMSHADY_FRAMEBUFFER_H


#include <cstdint>

class FrameBuffer {
public:
    FrameBuffer(float imageWidth, float imageHeight);
    ~FrameBuffer();

    void Bind();
    void UnBind();
    uint32_t GetTexture();

private:
    uint32_t m_FrameBuffer;
    uint32_t m_DepthRenderBuffer;
    uint32_t m_Texture;

};


#endif //SLIMSHADY_FRAMEBUFFER_H
