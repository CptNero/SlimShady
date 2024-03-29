#ifndef SLIMSHADY_SCENEELEMENT_H
#define SLIMSHADY_SCENEELEMENT_H


#include <memory>
#include <map>
#include <list>
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "Shader.h"
#include "ShaderType.h"
#include "Frameworks/FileManager.h"
#include "Texture.h"

class SceneElement {
  public:
    // Constructor used for loading from file
    SceneElement(const std::string& sceneName,
                 const std::string& vertexShaderSource,
                 const std::string& fragmentShaderSource,
                 const AttributeFile& vertexAttributeFile);

    // Constructor used for recompiling element
    SceneElement(const std::string& sceneName,
                 const std::string& vertexShaderSource,
                 const std::string& fragmentShaderSource,
                 std::vector<float> vertices,
                 std::vector<uint32_t > indices,
                 std::list<std::string> texturePaths);

    // Constructor used for creating new element from scratch
    SceneElement(const std::string& sceneElementName);
    //Constructor used for adding an element to the normal scene through the task loader.
    SceneElement(const std::string& sceneElementName, AttributeFile attributeFile);
    SceneElement();
    ~SceneElement();

    std::string GetSceneName();

    const std::unique_ptr<VertexArray> &GetVertexArray() const;
    const std::unique_ptr<IndexBuffer> &GetIndexBuffer() const;
    const std::unique_ptr<VertexBuffer> &GetVertexBuffer() const;
    const std::unique_ptr<Shader> &GetShader() const;
    const std::unique_ptr<Texture> &GetTexture() const;
    std::list<std::string> &GetTexturePaths();

    std::string GetShaderSourcePath(ShaderType shaderType);
    std::string GetShaderSource(ShaderType shaderType);
    std::vector<float>& GetVertices();
    std::vector<uint32_t>& GetIndices();

    void SetShaderSource(const std::string& source, ShaderType shaderType);

  private:
    std::string m_SceneName;

    std::unique_ptr<VertexArray> m_VertexArray;
    std::unique_ptr<IndexBuffer> m_IndexBuffer;
    std::unique_ptr<VertexBuffer> m_VertexBuffer;
    std::unique_ptr<Shader> m_Shader;
    std::unique_ptr<Texture> m_Texture;

    std::string m_VertexShaderSourcePath;
    std::string m_FragmentShaderSourcePath;

    std::string m_VertexShaderSource;
    std::string m_FragmentShaderSource;

    std::vector<float> m_Vertices;
    std::vector<uint32_t> m_Indices;
    std::list<std::string> m_TexturePaths;

    void InitializeSceneElement();
};


#endif //SLIMSHADY_SCENEELEMENT_H
