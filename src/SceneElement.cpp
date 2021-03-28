#include <glm/ext/matrix_transform.hpp>
#include <fstream>
#include "SceneElement.h"
#include "Renderer.h"
#include "Frameworks/Configurations.h"
#include "Widgets/ConsoleWidget.h"
#include "Camera.h"

SceneElement::SceneElement() {
  m_SceneName = "Default";
}

SceneElement::SceneElement(const std::string& sceneElementName) : m_SceneName(sceneElementName)
{
  auto vertexShaderFile = FileManager::CreateShaderFile(sceneElementName, ShaderType::VERTEX);
  auto fragmentShaderFile = FileManager::CreateShaderFile(sceneElementName, ShaderType::FRAGMENT);
  auto vertexAttributeFile = FileManager::CreateVertexAttributeFile(sceneElementName);

  m_VertexShaderSource = vertexShaderFile.ShaderSource;
  m_VertexShaderSourcePath = vertexShaderFile.Path;

  m_FragmentShaderSource = fragmentShaderFile.ShaderSource;
  m_FragmentShaderSourcePath = fragmentShaderFile.Path;

  m_Vertices = vertexAttributeFile.Vertices;
  m_Indices = vertexAttributeFile.Indices;

  InitializeSceneElement();
}

SceneElement::SceneElement(const std::string &sceneName,
                           const std::string &vertexShaderSource,
                           const std::string &fragmentShaderSource,
                           const FileManager::VertexAttributeFile& vertexAttributeFile) :
        m_SceneName(sceneName),
        m_VertexShaderSource(vertexShaderSource),
        m_FragmentShaderSource(fragmentShaderSource),
        m_Vertices(vertexAttributeFile.Vertices),
        m_Indices(vertexAttributeFile.Indices),
        m_TexturePaths(vertexAttributeFile.texturePaths)
{
  InitializeSceneElement();
}

SceneElement::SceneElement(const std::string& sceneName,
                           const std::string& vertexShaderSource,
                           const std::string& fragmentShaderSource,
                           std::vector<float> vertices,
                           std::vector<uint32_t> indices,
                           std::list<std::string> texturePaths) :
        m_SceneName(sceneName),
        m_VertexShaderSource(vertexShaderSource),
        m_FragmentShaderSource(fragmentShaderSource),
        m_Vertices(vertices),
        m_Indices(indices),
        m_TexturePaths(texturePaths)
{
  InitializeSceneElement();
}

SceneElement::~SceneElement() = default;

void SceneElement::InitializeSceneElement() {
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  m_Shader = std::make_unique<Shader>(m_VertexShaderSource, m_FragmentShaderSource);

  if (Configurations::IsDebugEnabled) {
    ConsoleWidget::LogMessage("Successfully created shader.");
  }

  m_VertexArrayObject = std::make_unique<VertexArray>();

  if (Configurations::IsDebugEnabled) {
    ConsoleWidget::LogMessage("Successfully created vertex array.");
  }

  //number of rows * length of rows
  m_VertexBuffer = std::make_unique<VertexBuffer>((float*)&m_Vertices[0], (m_Vertices.size() / 8) * 8 * sizeof(float));

  if (Configurations::IsDebugEnabled) {
    ConsoleWidget::LogMessage("Successfully created vertex buffer.");
  }

  VertexBufferLayout layout;
  //Vertex coordinates
  layout.Push<float>(3);
  //Color coordinates
  layout.Push<float>(3);
  //Texture coordinates
  layout.Push<float>(2);

  if (Configurations::IsDebugEnabled) {
    ConsoleWidget::LogMessage("Successfully initialized vertex layout");
  }

  m_VertexArrayObject->AddBuffer(*m_VertexBuffer, layout);

  if (Configurations::IsDebugEnabled) {
    ConsoleWidget::LogMessage("Successfully added buffer to vertex array object.");
  }

  m_IndexBuffer = std::make_unique<IndexBuffer>((uint32_t*)&m_Indices[0], m_Indices.size());

  if (Configurations::IsDebugEnabled) {
    ConsoleWidget::LogMessage("Successfully initialized index buffer");
  }

  m_Texture = std::make_unique<Texture>(m_TexturePaths);
}

std::string SceneElement::GetSceneName() {
  return m_SceneName;
}


std::string SceneElement::GetShaderSourcePath(ShaderType shaderType) {
  switch(shaderType) {
    case ShaderType::VERTEX:
      return m_VertexShaderSourcePath;
    case ShaderType::FRAGMENT:
      return m_FragmentShaderSourcePath;
  }
}

std::string SceneElement::GetShaderSource(ShaderType shaderType) {
  switch(shaderType) {
    case ShaderType::VERTEX:
      return m_VertexShaderSource;
    case ShaderType::FRAGMENT:
      return m_FragmentShaderSource;
  }
}

void SceneElement::SetShaderSource(const std::string& source, ShaderType shaderType) {
  switch(shaderType) {
    case ShaderType::VERTEX:
      m_VertexShaderSource = source;
      break;
    case ShaderType::FRAGMENT:
      m_FragmentShaderSource = source;
      break;
  }
}

std::vector<float>& SceneElement::GetVertices() {
  return m_Vertices;
}

std::vector<uint32_t>& SceneElement::GetIndices() {
  return m_Indices;
}

const std::unique_ptr<VertexArray> &SceneElement::GetVertexArray() const {
    return m_VertexArrayObject;
}

const std::unique_ptr<IndexBuffer> &SceneElement::GetIndexBuffer() const {
    return m_IndexBuffer;
}

const std::unique_ptr<VertexBuffer> &SceneElement::GetVertexBuffer() const {
    return m_VertexBuffer;
}

const std::unique_ptr<Shader> &SceneElement::GetShader() const {
    return m_Shader;
}

const std::unique_ptr<Texture> &SceneElement::GetTexture() const {
    return m_Texture;
}

const std::list<std::string> &SceneElement::GetTexturePaths() const {
    return m_TexturePaths;
}
