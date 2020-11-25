#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <fstream>
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

  InitializeSceneElement();
}

SceneElement::SceneElement(const std::string& sceneName,
                           const std::string& vertexShaderSource,
                           const std::string& fragmentShaderSource) :
        m_SceneName(sceneName), m_VertexShaderSource(vertexShaderSource), m_FragmentShaderSource(fragmentShaderSource),
        m_Model(glm::mat4(1.0f)),
        m_Projection(glm::perspective(glm::radians(500.0f), Configurations::GetScreenWidth() / Configurations::GetScreenHeight(), 0.1f, 100.f)),
        m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -3.0f)))
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

  m_VertexBuffer = std::make_unique<VertexBuffer>(positions, 4 * 3 * sizeof(float));

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

  m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 6);

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
