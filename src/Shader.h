#ifndef SLIMSHADY_SHADER_H
#define SLIMSHADY_SHADER_H

#include <string>
#include <map>
#include <glm/glm.hpp>

class Shader
{
public:
    Shader(const std::string& vertexShaderSource, const std::string& fragmentShaderSource);
    ~Shader();
    void Bind() const;
    void UnBind() const;

    uint32_t GetRendererId() const;

private:
    uint32_t m_RendererID;

    uint32_t CompileShader(uint32_t type, const std::string &source);
    uint32_t CreateShader(const std::string &vertexShader, const std::string &fragmentShader);
};

#endif //SLIMSHADY_SHADER_H