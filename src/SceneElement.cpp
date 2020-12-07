#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <fstream>
#include <algorithm>
#include <iostream>
#include "SceneElement.h"
#include "Renderer.h"
#include "Frameworks/Configurations.h"
#include "Widgets/ConsoleWidget.h"
#include "Camera.h"

SceneElement::SceneElement(const std::string &sceneName,
                           const std::string &vertexShaderSource,
                           const std::string &fragmentShaderSource,
                           FileManager::VertexAttributeFile vertexAttributeFile) :
        m_SceneName(sceneName), m_VertexShaderSource(vertexShaderSource), m_FragmentShaderSource(fragmentShaderSource),
        m_Model(glm::mat4(1.0f)),
        m_Projection(glm::perspective(glm::radians(500.0f), Configurations::GetScreenWidth() / Configurations::GetScreenHeight(), 0.1f, 100.f)),
        m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -3.0f)))
{
  std::for_each(vertexAttributeFile.Vertices.begin(), vertexAttributeFile.Vertices.end(), [&](glm::vec3 vertex) {
      m_Vertices.emplace_back(vertex.x);
      m_Vertices.emplace_back(vertex.y);
      m_Vertices.emplace_back(vertex.z);
  });

  m_Indices = vertexAttributeFile.Indices;
  InitializeSceneElement();
}

SceneElement::SceneElement(const std::string& sceneElementName) :
        m_SceneName(sceneElementName),
        m_Model(glm::mat4(1.0f)),
        m_Projection(glm::perspective(glm::radians(45.0f), 1920.0f / 1080.0f, 0.1f, 100.f)),
        m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -3.0f)))
{
  auto vertexShaderFile = FileManager::CreateShaderFile(sceneElementName, ShaderType::VERTEX);
  auto fragmentShaderFile = FileManager::CreateShaderFile(sceneElementName, ShaderType::FRAGMENT);
  auto vertexAttributeFile = FileManager::CreateVertexAttributeFile(sceneElementName);

  m_VertexShaderSource = vertexShaderFile.ShaderSource;
  m_VertexShaderSourcePath = vertexShaderFile.Path;
  m_VertexAttributePath = vertexAttributeFile.Path;

  m_FragmentShaderSource = fragmentShaderFile.ShaderSource;
  m_FragmentShaderSourcePath = fragmentShaderFile.Path;

  std::for_each(vertexAttributeFile.Vertices.begin(), vertexAttributeFile.Vertices.end(), [&](glm::vec3 vertex) {
      m_Vertices.emplace_back(vertex.x);
      m_Vertices.emplace_back(vertex.y);
      m_Vertices.emplace_back(vertex.z);
  });

  std::for_each(vertexAttributeFile.Indices.begin(), vertexAttributeFile.Indices.end(), [&](uint32_t index) {
      m_Indices.emplace_back(index);
  });

  InitializeSceneElement();
}

SceneElement::SceneElement(const std::string& sceneName,
                           const std::string& vertexShaderSource,
                           const std::string& fragmentShaderSource,
                           std::vector<float> vertices,
                           std::vector<uint32_t> indices) :
        m_SceneName(sceneName), m_VertexShaderSource(vertexShaderSource), m_FragmentShaderSource(fragmentShaderSource),
        m_Model(glm::mat4(1.0f)),
        m_Projection(glm::perspective(glm::radians(500.0f), Configurations::GetScreenWidth() / Configurations::GetScreenHeight(), 0.1f, 100.f)),
        m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -3.0f))),
        m_Vertices(vertices),
        m_Indices(indices)
{
  InitializeSceneElement();
}


SceneElement::~SceneElement() = default;

void SceneElement::Draw() {
  Renderer renderer;

  Camera::UpdateCameraTime();
  m_View = Camera::GetViewMatrix();
  m_Projection = glm::perspective(glm::radians(Camera::Zoom), Configurations::GetScreenWidth() / Configurations::GetScreenHeight(), 0.1f, 100.f);
  glm::mat4 viewProjectionMatrix = m_Projection * m_View * m_Model;
  m_Shader->Bind();
  m_Shader->SetUniformMat4f("u_MVP", viewProjectionMatrix);
  m_Shader->SetUniform1f("u_Time", glfwGetTime());
  m_Shader->SetUniform1i("u_Width", Configurations::GetScreenWidth());
  m_Shader->SetUniform1i("u_Height", Configurations::GetScreenHeight());
  renderer.Draw(*m_VertexArrayObject, *m_IndexBuffer, *m_Shader);
}

void SceneElement::InitializeSceneElement() {

  float positions[] = {
          -0.5f, -0.5f, 0.0f,
          0.5f, -0.5f, 0.0f,
          0.5f,  0.5f, 0.0f,
          -0.5f, 0.5f, 0.0f,
  };

  uint32_t indices[] = {
          0, 1, 2,
          2, 3, 0
  };

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  m_Shader = std::make_unique<Shader>(m_VertexShaderSource, m_FragmentShaderSource);

  if (Configurations::GetIsDebugEnabled()) {
    ConsoleWidget::LogMessage("Successfully created shader.");
  }

  m_VertexArrayObject = std::make_unique<VertexArray>();

  if (Configurations::GetIsDebugEnabled()) {
    ConsoleWidget::LogMessage("Successfully created vertex array.");
  }

  m_VertexBuffer = std::make_unique<VertexBuffer>((float*)&m_Vertices[0], (m_Vertices.size() / 3) * 3 * sizeof(float));

  if (Configurations::GetIsDebugEnabled()) {
    ConsoleWidget::LogMessage("Successfully created vertex buffer.");
  }

  VertexBufferLayout layout;
  layout.Push<float>(3);

  if (Configurations::GetIsDebugEnabled()) {
    ConsoleWidget::LogMessage("Successfully initialized vertex layout");
  }

  m_VertexArrayObject->AddBuffer(*m_VertexBuffer, layout);

  if (Configurations::GetIsDebugEnabled()) {
    ConsoleWidget::LogMessage("Successfully added buffer to vertex array object.");
  }

  m_IndexBuffer = std::make_unique<IndexBuffer>((uint32_t*)&m_Indices[0], m_Indices.size());

  if (Configurations::GetIsDebugEnabled()) {
    ConsoleWidget::LogMessage("Successfully initialized index buffer");
  }

  m_Shader->Bind();
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

std::vector<float> SceneElement::GetVertices() {
  return m_Vertices;
}

std::vector<uint32_t> SceneElement::GetIndices() {
  return m_Indices;
}
