#include <glew.h>

#include <utility>

#include "Texture.h"
#include "stb_image/stb_image.h"

Texture::Texture(std::list<std::string>& texturePaths)
        : m_RendererID(0), m_TexturePaths(texturePaths), m_LocalBuffer(nullptr), m_Width(0), m_Height(0), m_BPP(0)
{
  stbi_set_flip_vertically_on_load(1);

  int counter = 0;
  for(auto const& texturePath : m_TexturePaths) {
    m_LocalBuffer = stbi_load(texturePath.c_str(), &m_Width, &m_Height, &m_BPP, 4);
    m_TextureIds.resize(m_TexturePaths.size());

    glGenTextures(1, &m_TextureIds[counter]);
    glBindTexture(GL_TEXTURE_2D, m_TextureIds[counter]);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer);
    glGenerateMipmap(GL_TEXTURE_2D);

    if (m_LocalBuffer)
      stbi_image_free(m_LocalBuffer);
    counter++;
  }
}

Texture::~Texture()
{
  glDeleteTextures(1, &m_RendererID);
}

void Texture::Bind() const
{
  for (int i = 0; i < m_TextureIds.size(); i++) {
    glActiveTexture(m_GlTextures[i]);
    glBindTexture(GL_TEXTURE_2D, m_TextureIds[i]);
  }
}

void Texture::UnBind() const
{
  glBindTexture(GL_TEXTURE_2D, 0);
}

std::string Texture::GetPath() {
  return "";
}

std::string Texture::GetName() {
  return m_Name;
}
