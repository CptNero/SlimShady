#ifndef SLIMSHADY_SCENEELEMENT_H
#define SLIMSHADY_SCENEELEMENT_H


#include <memory>
#include <map>
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "Shader.h"
#include "ShaderType.h"
#include "Frameworks/FileManager.h"

class SceneElement {
  public:
    // Constructor used for loading from file
    SceneElement(const std::string& sceneName,
                 const std::string& vertexShaderSource,
                 const std::string& fragmentShaderSource,
                 FileManager::VertexAttributeFile vertexAttributeFile);

    // Constructor used for recompiling element
    SceneElement(const std::string& sceneName,
                 const std::string& vertexShaderSource,
                 const std::string& fragmentShaderSource,
                 std::vector<float> vertices,
                 std::vector<uint32_t > indices);

    // Constructor used for creating new element from scratch
    SceneElement(const std::string& sceneElementName);
    ~SceneElement();

    std::string GetSceneName();
    std::string GetShaderSourcePath(ShaderType shaderType);
    std::string GetShaderSource(ShaderType shaderType);
    std::vector<float> GetVertices();
    std::vector<uint32_t> GetIndices();

    std::vector<float> m_Vertices;
    std::vector<uint32_t> m_Indices;

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

    std::string m_VertexAttributePath;

    glm::mat4 m_Projection;
    glm::mat4 m_View;
    glm::mat4 m_Model;

    void InitializeSceneElement();
};


#endif //SLIMSHADY_SCENEELEMENT_H
