#ifndef SLIMSHADY_SHADER_H
#define SLIMSHADY_SHADER_H

#include <string>
#include <unordered_map>
#include <glm/glm.hpp>

class Shader
{
private:
    uint32_t m_RendererID;
    std::unordered_map<std::string, int> m_UniformLocationCache;

public:
    Shader(const std::string& vertexShaderSource, const std::string& fragmentShaderSource);
    ~Shader();
    void Bind() const;
    void UnBind() const;
    void SetUniform1i(const std::string& name, int value);
    void SetUniform1f(const std::string& name, float value);
    void SetUniform4f(const std::string& name, float v0, float  v1, float f2, float f3);
    void SetUniformMat4f(const std::string& name, const glm::mat4 &matrix);

private:
    uint32_t CompileShader(uint32_t type, const std::string &source);
    uint32_t CreateShader(const std::string &vertexShader, const std::string &fragmentShader);

    int GetUniformLocation(const std::string& name);
};

#endif //SLIMSHADY_SHADER_H