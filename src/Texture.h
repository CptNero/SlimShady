#ifndef SLIMSHADY_TEXTURE_H
#define SLIMSHADY_TEXTURE_H

#include <string>
#include <vector>
#include <list>

class Texture
{
private:
    unsigned int m_RendererID;
    std::string m_Name;
    std::list<std::string> m_TexturePaths;
    unsigned char* m_LocalBuffer;
    int m_Width, m_Height, m_BPP;


public:
    Texture(std::list<std::string>& texturePaths);
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
    std::vector<uint32_t> m_TextureIds;
    ~Texture();

    void Bind() const;
    void UnBind() const;

    std::string GetName();
    std::string GetPath();
    inline int GetWidth() const { return m_Width; }
    inline int GetHeight() const { return m_Height; }
};

#endif //SLIMSHADY_TEXTURE_H
