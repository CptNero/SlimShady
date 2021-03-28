#ifndef SLIMSHADY_TEXTURE_H
#define SLIMSHADY_TEXTURE_H

#include <string>
#include <vector>
#include <list>

class Texture
{
public:
    Texture(std::list<std::string>& texturePaths);
    ~Texture();

    void Bind() const;
    void UnBind() const;

    std::string GetName();

    const std::vector<uint32_t> &GetTextureIds() const;

    inline int GetWidth() const { return m_Width; }
    inline int GetHeight() const { return m_Height; }

private:
    unsigned int m_RendererID;
    std::string m_Name;
    std::vector<uint32_t> m_TextureIds;
    std::list<std::string> m_TexturePaths;

    GLint m_GlTextures[11] =
            { GL_TEXTURE0,
              GL_TEXTURE1,
              GL_TEXTURE2,
              GL_TEXTURE3,
              GL_TEXTURE4,
              GL_TEXTURE5,
              GL_TEXTURE6,
              GL_TEXTURE7,
              GL_TEXTURE8,
              GL_TEXTURE9,
              GL_TEXTURE10 };

    unsigned char* m_LocalBuffer;
    int m_Width, m_Height, m_BPP;



};

#endif //SLIMSHADY_TEXTURE_H
