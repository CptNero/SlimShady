#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <fstream>
#include "SceneElement.h"
#include "Renderer.h"
#include "Frameworks/Configurations.h"
#include "Widgets/ConsoleWidget.h"
#include "Frameworks/ShaderFileManager.h"

SceneElement::SceneElement(const std::string& sceneElementName) :
  m_sceneName(sceneElementName),
  m_TranslationA(0, 0, 0), m_TranslationB(0, 0, 0),
  m_Proj(glm::ortho(0.0f, 1920.0f, 0.0f, 1080.0f, -1.0f, 1.0f)),
  m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))){

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
  m_sceneName(sceneName), m_VertexShaderSource(vertexShaderSource), m_FragmentShaderSource(fragmentShaderSource),
  m_TranslationA(0, 0, 0), m_TranslationB(0, 0, 0),
  m_Proj(glm::ortho(0.0f, 1920.0f, 0.0f, 1080.0f, -1.0f, 1.0f)),
  m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))){
  InitializeSceneElement();
}


SceneElement::~SceneElement() = default;

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

void SceneElement::Draw() {
  Renderer renderer;

  glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationA);
  glm::mat4 mvp = m_Proj * m_View * model;
  m_Shader->Bind();
  m_Shader->SetUniformMat4f("u_MVP", mvp);
  renderer.Draw(*m_VertexArrayObject, *m_IndexBuffer, *m_Shader);
}

void SceneElement::InitializeSceneElement() {
  glm::vec3 translationA(200, 200, 0);
  glm::vec3 translationB(400, 200, 0);

  float positions[] = {
          -0.5f, -0.5f, 1.0f,
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

  VertexArray va;
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

