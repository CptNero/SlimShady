#include <GL/glew.h>
#include "Renderer.h"
#include "Camera.h"
#include "Frameworks/Configurations.h"
#include "Widgets/SceneEditorWidget.h"
#include "Widgets/TaskWidget.h"

Renderer::Renderer(Context context) :
  m_Context(context),
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

    UpdateUniforms(*sceneElement);

    sceneElement->m_Shader->Bind();
    sceneElement->m_VertexArrayObject->Bind();
    sceneElement->m_IndexBuffer->Bind();
    sceneElement->m_Texture->Bind();

    glDrawElements(GL_TRIANGLES, sceneElement->m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);
    ((TaskWidget*) m_Context.widgetBroker.GetWidget("TaskWidget"))->RenderIntoTexture(sceneElement->m_IndexBuffer->GetCount());
  }
}

void Renderer::UpdateUniforms(SceneElement& sceneElement) {
  m_Projection = glm::perspective(glm::radians(Camera::Zoom), Configurations::ScreenWidth / Configurations::ScreenHeight, 0.1f, 100.f);
  m_View = Camera::GetViewMatrix();

  sceneElement.m_Shader->SetUniformMat4f("u_Projection", m_Projection);
  sceneElement.m_Shader->SetUniformMat4f("u_Model", m_Model);
  sceneElement.m_Shader->SetUniformMat4f("u_View", m_View);

  int counter = 0;
  for(auto const& texturePath : sceneElement.m_TexturePaths) {
    std::stringstream samplerName;
    samplerName << "u_Texture" << counter;
    if (!sceneElement.m_Texture->m_TextureIds.empty()) {
      sceneElement.m_Shader->SetUniform1i(samplerName.str(), counter);
    }
    counter++;
  }

  sceneElement.m_Shader->SetUniform1f("u_Time", glfwGetTime());
  sceneElement.m_Shader->SetUniform1i("u_Width", Configurations::ScreenWidth);
  sceneElement.m_Shader->SetUniform1i("u_Height", Configurations::ScreenHeight);
}


