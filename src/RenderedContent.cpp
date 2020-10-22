#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "RenderedContent.h"
#include "Renderer.h"
#include "Configurations.h"
#include "Widgets/ConsoleWidget.h"

RenderedContent::RenderedContent(const std::string& vertexShaderSource, const std::string& fragmentShaderSource) :
  m_VertexShaderSource(vertexShaderSource), m_FragmentShaderSource(fragmentShaderSource),
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


RenderedContent::~RenderedContent() = default;

void RenderedContent::Draw() {
  Renderer renderer;

  {
    glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationA);
    glm::mat4 mvp = m_Proj * m_View * model;
    m_Shader->Bind();
    m_Shader->SetUniformMat4f("u_MVP", mvp);
    renderer.Draw(*m_VertexArrayObject, *m_IndexBuffer, *m_Shader);
  }

//  {
//    glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationB);
//    glm::mat4 mvp = m_Proj * m_View * model;
//    m_Shader->Bind();
//    m_Shader->SetUniformMat4f("u_MVP", mvp);
//    renderer.Draw(*m_VertexArrayObject, *m_IndexBuffer, *m_Shader);
//  }
}
