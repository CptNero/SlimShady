#include <stb_image/stb_image.h>
#include <stb_image/stb_image_write.h>
#include "TaskWidget.h"
#include "SceneEditorWidget.h"
#include "../Frameworks/ImageComparator.h"
#include "FileBrowserWidget.h"
#include "ConsoleWidget.h"

TaskWidget::TaskWidget(Context& context) :
  m_Context(context)
{
  InitializeFrameBuffer();
  LoadTaskImage(Configurations::DefaultTaskFilePath);
}

TaskWidget::~TaskWidget() {

}

void TaskWidget::OnUpdate(float deltaTime) {
}

void TaskWidget::OnRender() {
}

void TaskWidget::OnImGuiRender() {
  if (ImGui::BeginMenuBar()) {
    if (ImGui::BeginMenu("Actions"))  {
      if (ImGui::MenuItem("Export task")) {
        ExportImage(GetTextureData(m_RenderedTexture), m_TaskNameInputBuffer);
      }

      if (ImGui::MenuItem("Select task")) {
        ((FileBrowserWidget*) m_Context.widgetBroker.GetWidget<FileBrowserWidget>("FileBrowser"))->OpenFileBrowser(FileBrowserWidget::Task);
      }

      if (ImGui::MenuItem("Change task image")) {
        LoadTaskImage(((FileBrowserWidget*) m_Context.widgetBroker.GetWidget<FileBrowserWidget>("FileBrowser"))->QueryFileBrowser(FileBrowserWidget::Task));
      }

      if (ImGui::MenuItem("Change layout")) {
        m_Layout = !m_Layout;
      }

      if (ImGui::MenuItem("Compare images")) {
        ImageComparator imageComparator(GetTextureData(m_TaskTexture), GetTextureData(m_RenderedTexture));
        m_ComparisonResult = imageComparator.CompareImages();
      }
      ImGui::EndMenu();
    }
    ImGui::EndMenuBar();
  }

  // If layout is true render horizontal layout otherwise render vertical layout
  ImGui::PushItemWidth(200.0f);
  ImGui::InputText("Task name", m_TaskNameInputBuffer, IM_ARRAYSIZE(m_TaskNameInputBuffer));
  ImGui::PopItemWidth();
  if (m_Layout) {
    ImGui::Text("Target image:");
    ImGui::SameLine();
    ImGui::Image((ImTextureID)m_TaskTexture, ImVec2(600, 400), ImVec2(0, 1), ImVec2(1, 0));
    ImGui::SameLine();
    ImGui::Text("Your image:");
    ImGui::SameLine();
    ImGui::Image((ImTextureID)m_RenderedTexture, ImVec2(600, 400), ImVec2(0, 1), ImVec2(1, 0));
    ImGui::Text("Comparison result: %f percent similarity", (m_ComparisonResult / 4.0) * 100.0);
  } else {
    ImGui::Text("Target image:");
    ImGui::Image((ImTextureID)m_TaskTexture, ImVec2(600, 400), ImVec2(0, 1), ImVec2(1, 0));
    ImGui::Text("Your image:");
    ImGui::Image((ImTextureID)m_RenderedTexture, ImVec2(600, 400), ImVec2(0, 1), ImVec2(1, 0));
    ImGui::Text("Comparison result: %f percent similarity", (m_ComparisonResult / 4.0) * 100.0);
  }

}

void TaskWidget::RenderWidget() {
  OnUpdate(0.0f);
  OnRender();
  ImGui::Begin("Task", nullptr, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_MenuBar);
  OnImGuiRender();
  ImGui::End();
}

void TaskWidget::InitializeFrameBuffer() {
  glGenFramebuffers(1, &m_FrameBuffer);
  glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer);

  glGenTextures(1, &m_RenderedTexture);
  glBindTexture(GL_TEXTURE_2D, m_RenderedTexture);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Configurations::ScreenWidth, Configurations::ScreenHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_RenderedTexture, 0);

  glGenRenderbuffers(1, &m_DepthRenderBuffer);
  glBindRenderbuffer(GL_RENDERBUFFER, m_DepthRenderBuffer);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, Configurations::ScreenWidth, Configurations::ScreenHeight);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_DepthRenderBuffer);

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    std::cout << "Framebuffer is not complete!" << std::endl;
  }

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

std::vector<uint8_t> TaskWidget::GetTextureData(GLuint texture) {
  std::vector<uint8_t> data;
  int width, height;
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texture);

  glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
  glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);

  data.resize(width * height * 4);

  glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, &data[0]);

  glDisable(GL_TEXTURE_2D);
  return data;
}

void TaskWidget::LoadTaskImage(const std::string &filePath) {
  unsigned char* image_data = stbi_load(filePath.c_str(), &m_TaskImageWidth, &m_TaskImageHeight, nullptr, 4);

  glGenTextures(1, &m_TaskTexture);
  glBindTexture(GL_TEXTURE_2D, m_TaskTexture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_TaskImageWidth, m_TaskImageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
  stbi_image_free(image_data);
}

void TaskWidget::RenderIntoTexture(uint32_t indexBufferSize) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glDrawElements(GL_TRIANGLES, indexBufferSize, GL_UNSIGNED_INT, nullptr);
}

void TaskWidget::ExportImage(std::vector<uint8_t> textureData, const std::string& name) {
  if (name.empty()) {
    ConsoleWidget::LogMessage("Task name cannot be empty!");
    return;
  }

  std::stringstream filename;
  filename << Configurations::TaskFilesPath << name << ".png";

  stbi_write_png(filename.str().c_str(),
                 Configurations::ScreenWidth,
                 Configurations::ScreenHeight,
                 4,
                 &textureData[0],
                 4 * Configurations::ScreenWidth);
}

void TaskWidget::BindFrameBuffer() {
  glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer);
  glEnable(GL_DEPTH_TEST);
}

void TaskWidget::UnBindFrameBuffer() {
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glDisable(GL_DEPTH_TEST);
}
