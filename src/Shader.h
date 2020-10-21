#ifndef SLIMSHADY_SHADER_H
#define SLIMSHADY_SHADER_H

#include <string>
#include <unordered_map>
#include <glm/glm.hpp>

class Shader
{
private:
    std::string m_VertexShaderFilePath;
    std::string m_FragmentShaderFilePath;
    unsigned int m_RendererID;
    std::unordered_map<std::string, int> m_UniformLocationCache;

    enum class ShaderType
    {
        NONE = 0,
        VERTEX = 1,
        FRAGMENT = 2
    };

public:
    Shader();
    ~Shader();
    void Bind() const;
    void UnBind() const;
    void SetUniform1i(const std::string& name, int value);
    void SetUniform4f(const std::string& name, float v0, float  v1, float f2, float f3);
    void SetUniformMat4f(const std::string& name, const glm::mat4 &matrix);

private:
    std::string ParseShader(const std::string& shaderFilePath);
    unsigned int CompileShader(unsigned int type, const std::string &source);
    unsigned int CreateShader(const std::string &vertexShader, const std::string &fragmentShader);

    int GetUniformLocation(const std::string& name);
};

#endif //SLIMSHADY_SHADER_H