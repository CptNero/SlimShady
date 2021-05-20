#include <GL/glew.h>
#include "Renderer.h"
#include "Frameworks/Configurations.h"
#include "Widgets/SceneEditorWidget.h"
#include "Widgets/TaskWidget.h"
#include "UniformManager.h"

Renderer::Renderer(Context context, UniformManager& renderedUniformManager, UniformManager& taskUniformManager) :
  m_Context(context),
  m_RenderedUniformManager(renderedUniformManager),
  m_TaskUniformManger(taskUniformManager),
  m_Model(glm::mat4(1.0f)),
  m_Projection(glm::perspective(glm::radians(500.0f), Configurations::ScreenWidth / Configurations::ScreenHeight, 0.1f, 100.f)),
  m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -3.0f))) {}

Renderer::~Renderer() = default;

void Renderer::Clear() const {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::Draw(std::list<SceneElement*>& scene, std::list<SceneElement*> taskScene) {
  //Render into main window
  glClear(GL_COLOR_BUFFER_BIT);
  Camera::UpdateCameraTime();
  for (auto  const &sceneElement : scene) {

    sceneElement->GetShader()->Bind();
    sceneElement->GetVertexArray()->Bind();
    sceneElement->GetIndexBuffer()->Bind();
    sceneElement->GetTexture()->Bind();

    UpdateUniforms(*sceneElement, m_RenderedUniformManager);

    glDrawElements(GL_TRIANGLES, sceneElement->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);

  }
  //Render into rendered image texture
  m_Context.widgetBroker.GetWidget<TaskWidget>(WidgetType::TASK)->GetRenderedFrameBuffer().Bind();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  for (auto const &sceneElement : scene) {
    sceneElement->GetShader()->Bind();
    sceneElement->GetVertexArray()->Bind();
    sceneElement->GetVertexBuffer()->Bind();
    sceneElement->GetTexture()->Bind();

    UpdateUniforms(*sceneElement, m_RenderedUniformManager);

    glDrawElements(GL_TRIANGLES, sceneElement->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
  }
  m_Context.widgetBroker.GetWidget<TaskWidget>(WidgetType::TASK)->GetRenderedFrameBuffer().UnBind();

  //Render into task image texture
  m_Context.widgetBroker.GetWidget<TaskWidget>(WidgetType::TASK)->GetTaskFrameBuffer().Bind();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  for (auto const& sceneElement : taskScene) {
    sceneElement->GetShader()->Bind();
    sceneElement->GetVertexArray()->Bind();
    sceneElement->GetVertexBuffer()->Bind();
    sceneElement->GetTexture()->Bind();

    UpdateUniforms(*sceneElement, m_TaskUniformManger);

    glDrawElements(GL_TRIANGLES, sceneElement->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
  }

  m_Context.widgetBroker.GetWidget<TaskWidget>(WidgetType::TASK)->GetTaskFrameBuffer().UnBind();
}

void Renderer::UpdateUniforms(SceneElement& sceneElement, UniformManager& uniformManager) {
  std::string name = sceneElement.GetSceneName();
  uint32_t rendererId = sceneElement.GetShader()->GetRendererId();

  m_Projection = glm::perspective(glm::radians(Camera::Zoom), Configurations::ScreenWidth / Configurations::ScreenHeight, 0.1f, 100.f);
  m_View = Camera::GetViewMatrix();

  uniformManager.SetUniformMat4f(rendererId, "u_" + name + "Projection", m_Projection);
  uniformManager.SetUniformMat4f(rendererId, "u_" + name + "Model", m_Model);
  uniformManager.SetUniformMat4f(rendererId,"u_" + name + "View", m_View);

  uniformManager.SetUniform1f(rendererId, "u_" + name + "Time", glfwGetTime());
  uniformManager.SetUniform1i(rendererId, "u_" + name + "Width", Configurations::ScreenWidth);
  uniformManager.SetUniform1i(rendererId, "u_" + name + "Height", Configurations::ScreenHeight);

  int counter = 0;
  for(auto const& texturePath : sceneElement.GetTexturePaths()) {
    std::stringstream samplerName;
    samplerName << "u_" + name + "Texture" << counter;
    if (!sceneElement.GetTexture()->GetTextureIds().empty()) {
      uniformManager.SetUniform1i(rendererId, samplerName.str(), counter);
    }
    counter++;
  }
}

