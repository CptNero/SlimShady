#include <GL/glew.h>
#include "Renderer.h"
#include "Frameworks/Configurations.h"
#include "Widgets/SceneEditorWidget.h"
#include "Widgets/TaskWidget.h"
#include "UniformManager.h"

Renderer::Renderer(Context context, UniformManager& uniformManager) :
  m_Context(context),
  m_UniformManager(uniformManager),
  m_Model(glm::mat4(1.0f)),
  m_Projection(glm::perspective(glm::radians(500.0f), Configurations::ScreenWidth / Configurations::ScreenHeight, 0.1f, 100.f)),
  m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -3.0f))) {}

Renderer::~Renderer() = default;

void Renderer::Clear() const {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::Draw(std::list<SceneElement*>& scene) {
  Camera::UpdateCameraTime();
  for (auto  const &sceneElement : scene) {

    sceneElement->m_Shader->Bind();
    sceneElement->m_VertexArrayObject->Bind();
    sceneElement->m_IndexBuffer->Bind();
    sceneElement->m_Texture->Bind();

    UpdateUniforms(*sceneElement);

    glDrawElements(GL_TRIANGLES, sceneElement->m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);

  }
  m_Context.widgetBroker.GetWidget<TaskWidget>("TaskWidget")->BindFrameBuffer();
  for (auto const &sceneElement: scene) {
    sceneElement->m_Shader->Bind();
    sceneElement->m_VertexArrayObject->Bind();
    sceneElement->m_IndexBuffer->Bind();
    sceneElement->m_Texture->Bind();

    m_Context.widgetBroker.GetWidget<TaskWidget>("TaskWidget")->RenderIntoTexture(sceneElement->m_IndexBuffer->GetCount());
  }
  m_Context.widgetBroker.GetWidget<TaskWidget>("TaskWidget")->UnBindFrameBuffer();
}

void Renderer::UpdateUniforms(SceneElement& sceneElement) {
  std::string name = sceneElement.GetSceneName();
  uint32_t rendererId = sceneElement.m_Shader->GetRendererId();

  m_Projection = glm::perspective(glm::radians(Camera::Zoom), Configurations::ScreenWidth / Configurations::ScreenHeight, 0.1f, 100.f);
  m_View = Camera::GetViewMatrix();

  m_UniformManager.SetUniformMat4f(rendererId, "u_" + name + "Projection", m_Projection);
  m_UniformManager.SetUniformMat4f(rendererId, "u_" + name + "Model", m_Model);
  m_UniformManager.SetUniformMat4f(rendererId,"u_" + name + "View", m_View);

  m_UniformManager.SetUniform1f(rendererId, "u_" + name + "Time", glfwGetTime());
  m_UniformManager.SetUniform1i(rendererId, "u_" + name + "Width", Configurations::ScreenWidth);
  m_UniformManager.SetUniform1i(rendererId, "u_" + name + "Height", Configurations::ScreenHeight);

  int counter = 0;
  for(auto const& texturePath : sceneElement.m_TexturePaths) {
    std::stringstream samplerName;
    samplerName << "u_" + name + "Texture" << counter;
    if (!sceneElement.m_Texture->m_TextureIds.empty()) {
      m_UniformManager.SetUniform1i(rendererId, samplerName.str(), counter);
    }
    counter++;
  }
}

