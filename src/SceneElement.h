#ifndef SLIMSHADY_SCENEELEMENT_H
#define SLIMSHADY_SCENEELEMENT_H


#include <memory>
#include <map>
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "Shader.h"
#include "ShaderTypes.h"

class SceneElement {
  public:
    SceneElement(const std::string& sceneName,
                 const std::string& vertexShaderSource,
                 const std::string& fragmentShaderSource,
                 std::map<int, glm::vec3> vertices,
                 std::map<int, int> indices);
    SceneElement(const std::string& sceneElementName);
    ~SceneElement();

    std::string GetSceneName();
    std::string GetShaderSourcePath(ShaderType shaderType);
    std::string GetShaderSource(ShaderType shaderType);
    void SetShaderSource(const std::string& source, ShaderType shaderType);

    void Draw();

  private:
    std::string m_SceneName;

    std::unique_ptr<VertexArray> m_VertexArrayObject;
    std::unique_ptr<IndexBuffer> m_IndexBuffer;
    std::unique_ptr<VertexBuffer> m_VertexBuffer;
    std::unique_ptr<Shader> m_Shader;

    std::string m_VertexShaderSourcePath;
    std::string m_FragmentShaderSourcePath;

    std::string m_VertexShaderSource;
    std::string m_FragmentShaderSource;

    std::vector<float> m_Vertices;
    std::vector<unsigned int> m_Indices;

    glm::mat4 m_Projection;
    glm::mat4 m_View;
    glm::mat4 m_Model;

    void InitializeSceneElement();
};


#endif //SLIMSHADY_SCENEELEMENT_H
