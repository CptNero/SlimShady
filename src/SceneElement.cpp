#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <fstream>
#include <algorithm>
#include "SceneElement.h"
#include "Renderer.h"
#include "Frameworks/Configurations.h"
#include "Widgets/ConsoleWidget.h"
#include "Frameworks/ShaderFileManager.h"
#include "Camera.h"

SceneElement::SceneElement(const std::string& sceneElementName) :
        m_SceneName(sceneElementName),
        m_Model(glm::mat4(1.0f)),
        m_Projection(glm::perspective(glm::radians(45.0f), 1920.0f / 1080.0f, 0.1f, 100.f)),
        m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -3.0f)))
{
  auto vertexShaderFile = ShaderFileManager::CreateShaderFile(sceneElementName, ShaderType::VERTEX);
  auto fragmentShaderFile = ShaderFileManager::CreateShaderFile(sceneElementName, ShaderType::FRAGMENT);

  m_VertexShaderSource = vertexShaderFile.ShaderSource;
  m_VertexShaderSourcePath = vertexShaderFile.Path;

  m_FragmentShaderSource = fragmentShaderFile.ShaderSource;
  m_FragmentShaderSourcePath = fragmentShaderFile.Path;

  m_Vertices.emplace_back(0.0f);
  m_Vertices.emplace_back(0.0f);
  m_Vertices.emplace_back(0.0f);
  m_Indices.emplace_back(0);

  InitializeSceneElement();
}

SceneElement::SceneElement(const std::string& sceneName,
                           const std::string& vertexShaderSource,
                           const std::string& fragmentShaderSource,
                           std::map<int, glm::vec3> vertices,
                           std::map<int, int> indices) :
        m_SceneName(sceneName), m_VertexShaderSource(vertexShaderSource), m_FragmentShaderSource(fragmentShaderSource),
        m_Model(glm::mat4(1.0f)),
        m_Projection(glm::perspective(glm::radians(500.0f), Configurations::GetScreenWidth() / Configurations::GetScreenHeight(), 0.1f, 100.f)),
        m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -3.0f)))
{
  std::for_each(vertices.begin(), vertices.end(), [&](std::pair<int, glm::vec3> vertex) {
      m_Vertices.emplace_back(vertex.second.x);
      m_Vertices.emplace_back(vertex.second.y);
      m_Vertices.emplace_back(vertex.second.z);
  });

  std::for_each(indices.begin(), indices.end(), [&](std::pair<int, int> index) {
      m_Indices.emplace_back(index.second);
  });

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
  m_Shader->SetUniform1i("u_Time", glfwGetTime());
  renderer.Draw(*m_VertexArrayObject, *m_IndexBuffer, *m_Shader);
}

void SceneElement::InitializeSceneElement() {

  float positions[] = {
          -0.5f, -0.5f, 0.0f,
          0.5f, -0.5f, 0.0f,
          0.5f,  0.5f, 0.0f,
          -0.5f, 0.5f, 0.0f,
  };

  unsigned int indices[] = {
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

  m_IndexBuffer = std::make_unique<IndexBuffer>((unsigned int*)&m_Indices[0], m_Indices.size());

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
