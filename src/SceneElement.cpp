#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <fstream>
#include "SceneElement.h"
#include "Renderer.h"
#include "Configurations.h"
#include "Widgets/ConsoleWidget.h"

SceneElement::SceneElement(const std::string& sceneName) :
  m_sceneName(sceneName),
  m_TranslationA(0, 0, 0), m_TranslationB(0, 0, 0),
  m_Proj(glm::ortho(0.0f, 1920.0f, 0.0f, 1080.0f, -1.0f, 1.0f)),
  m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))){
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

  CreateShaderFile(ShaderType::VERTEX);
  CreateShaderFile(ShaderType::FRAGMENT);

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

void SceneElement::CreateShaderFile(ShaderType shaderType) {
  std::string fileName = "src/res/shaders/" + m_sceneName;

  switch(shaderType){
    case ShaderType::VERTEX:
      fileName += "_Vertex.shader";
      m_VertexShaderSourcePath = fileName;
      break;
    case ShaderType::FRAGMENT:
      fileName += "_Fragment.shader";
      m_FragmentShaderSourcePath = fileName;
      break;
  }

  std::ofstream shaderFile(fileName);

  switch(shaderType){
    case ShaderType::VERTEX:
      shaderFile << Configurations::GetDefaultVertexShaderSource();
      m_VertexShaderSource = Configurations::GetDefaultVertexShaderSource();
      break;
    case ShaderType::FRAGMENT:
      shaderFile << Configurations::GetDefaultFragmentShaderSource();
      m_FragmentShaderSource = Configurations::GetDefaultFragmentShaderSource();
      break;
  }

  shaderFile.close();

  ConsoleWidget::LogMessage("Successfully created " + fileName);
}
