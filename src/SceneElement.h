#ifndef SLIMSHADY_SCENEELEMENT_H
#define SLIMSHADY_SCENEELEMENT_H


#include <memory>
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "Shader.h"
#include "ShaderTypes.h"

class SceneElement {
  public:
    explicit SceneElement(const std::string& sceneName);
    ~SceneElement();

    std::string GetShaderSourcePath(ShaderType shaderType);

    std::string GetShaderSource(ShaderType shaderType);

    void Draw();

  private:
    std::string m_sceneName;

    std::unique_ptr<VertexArray> m_VertexArrayObject;
    std::unique_ptr<IndexBuffer> m_IndexBuffer;
    std::unique_ptr<VertexBuffer> m_VertexBuffer;
    std::unique_ptr<Shader> m_Shader;

    glm::mat4 m_Proj, m_View;
    glm::vec3 m_TranslationA, m_TranslationB;

    std::string m_VertexShaderSourcePath;
    std::string m_FragmentShaderSourcePath;

    std::string m_VertexShaderSource;
    std::string m_FragmentShaderSource;

    void CreateShaderFile(ShaderType shaderType);
};


#endif //SLIMSHADY_SCENEELEMENT_H
