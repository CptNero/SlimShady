//
// Created by neresis on 2021. 04. 10..
//

#include <GL/glew.h>
#include <iostream>
#include "FrameBuffer.h"

FrameBuffer::FrameBuffer(float imageWidth, float imageHeight) {
  glGenFramebuffers(1, &m_FrameBuffer);
  glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer);

  glGenTextures(1, &m_Texture);
  glBindTexture(GL_TEXTURE_2D, m_Texture);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_Texture, 0);

  glGenRenderbuffers(1, &m_DepthRenderBuffer);
  glBindRenderbuffer(GL_RENDERBUFFER, m_DepthRenderBuffer);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, imageWidth, imageHeight);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_DepthRenderBuffer);

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    std::cout << "Framebuffer is not complete!" << std::endl;
  }

  glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

FrameBuffer::~FrameBuffer() = default;

void FrameBuffer::Bind() {
  glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer);
  //glEnable(GL_DEPTH_TEST);
}

void FrameBuffer::UnBind() {
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  //glDisable(GL_DEPTH_TEST);
}

uint32_t FrameBuffer::GetTexture() {
  return m_Texture;
}

