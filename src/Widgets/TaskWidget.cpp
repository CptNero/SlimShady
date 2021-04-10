#include <stb_image/stb_image.h>
#include <stb_image/stb_image_write.h>
#include "TaskWidget.h"
#include "SceneEditorWidget.h"
#include "../Frameworks/ImageComparator.h"
#include "FileBrowserWidget.h"
#include "ConsoleWidget.h"
#include "cereal/archives/json.hpp"

TaskWidget::TaskWidget(Context& context) :
  m_Context(context),
  m_RenderedFrameBuffer(Configurations::ScreenWidth, Configurations::ScreenHeight),
  m_TaskFrameBuffer(Configurations::ScreenWidth, Configurations::ScreenHeight)
{
}

TaskWidget::~TaskWidget() = default;

void TaskWidget::OnUpdate(float deltaTime) {
}

void TaskWidget::OnRender() {
}

void TaskWidget::OnImGuiRender() {
  if (ImGui::BeginMenuBar()) {
    if (ImGui::BeginMenu("Actions"))  {
      if (ImGui::MenuItem("Export task")) {
        AttributeFile attributeFile;
        auto elementQuery = std::find_if(m_Context.scene.begin(), m_Context.scene.end(), [&](SceneElement* element) {
            return (element->GetSceneName() == m_SelectedTaskName);
        });

        if(elementQuery != m_Context.scene.end()) {
          SceneElement* element = *elementQuery;

          attributeFile.Path = element->GetShaderSourcePath(ShaderType::VERTEX);
          attributeFile.vertexShaderSource = element->GetShaderSource(ShaderType::VERTEX);
          attributeFile.fragmentShaderSource = element->GetShaderSource(ShaderType::FRAGMENT);
          attributeFile.Vertices = element->GetVertices();
          attributeFile.Indices = element->GetIndices();
          attributeFile.texturePaths = element->GetTexturePaths();

          std::ofstream fileStream(Configurations::TaskFilesPath + element->GetSceneName() + ".json");
          {
            cereal::JSONOutputArchive outputArchive(fileStream);
            outputArchive(attributeFile);
          }

          fileStream.close();
        }
      }

      if (ImGui::MenuItem("Select task")) {
        ((FileBrowserWidget*) m_Context.widgetBroker.GetWidget<FileBrowserWidget>(WidgetType::FILE_BROWSER))->OpenFileBrowser(FileBrowserWidget::Task);
      }

      if (ImGui::MenuItem("Change task")) {
        LoadTask(m_Context.widgetBroker.GetWidget<FileBrowserWidget>(WidgetType::FILE_BROWSER)->QueryFileBrowser(FileBrowserWidget::Task));
      }

      if (ImGui::MenuItem("Change layout")) {
        m_Layout = !m_Layout;
      }

      if (ImGui::MenuItem("Compare images")) {
        std::vector<uint8_t> taskTextureData = GetTextureData(m_TaskFrameBuffer.GetTexture());
        std::vector<uint8_t> renderedTextureData = GetTextureData(m_RenderedFrameBuffer.GetTexture());

        ImageComparator imageComparator;
        std::stringstream ssimStringStream;
        std::stringstream csdStringStream;
        m_SSIMComparisonResult = (imageComparator.CalculateSSIM(taskTextureData, renderedTextureData) / 4.0) * 100.0;
        m_ChiSquareDistanceResult = imageComparator.CalculateChiSquareDistance(taskTextureData, renderedTextureData);

        ssimStringStream << "Comparison result with SSIM: "  << m_SSIMComparisonResult << "% similarity";
        csdStringStream << "Comparison result with chi square distance: " << m_ChiSquareDistanceResult << " (Lower value is better)";

        ConsoleWidget::LogMessage(ssimStringStream.str());
        ConsoleWidget::LogMessage(csdStringStream.str());

        if (m_SSIMComparisonResult > 70.0) {
          ConsoleWidget::LogMessage("Successfully completed the task!");
        }
      }
      ImGui::EndMenu();
    }
    ImGui::EndMenuBar();
  }

  // If layout is true render horizontal layout otherwise render vertical layout
  ImGui::PushItemWidth(200.0f);

  if (ImGui::BeginCombo("Task name", m_SelectedTaskName.c_str())) {
    bool is_selected = true;
    for(auto element : m_Context.scene) {
      if (ImGui::Selectable(element->GetSceneName().c_str())) {
        m_SelectedTaskName = element->GetSceneName();
      }
    }
    ImGui::EndCombo();
  }

  ImGui::PopItemWidth();
  if (m_Layout) {
    ImGui::Text("Target image:");
    ImGui::SameLine();
    ImGui::Image((ImTextureID)m_TaskFrameBuffer.GetTexture(), ImVec2(600, 400), ImVec2(0, 1), ImVec2(1, 0));
    ImGui::SameLine();
    ImGui::Text("Your image:");
    ImGui::SameLine();
    ImGui::Image((ImTextureID)m_RenderedFrameBuffer.GetTexture(), ImVec2(600, 400), ImVec2(0, 1), ImVec2(1, 0));
  } else {
    ImGui::Text("Target image:");
    ImGui::Image((ImTextureID)m_TaskFrameBuffer.GetTexture(), ImVec2(600, 400), ImVec2(0, 1), ImVec2(1, 0));
    ImGui::Text("Your image:");
    ImGui::Image((ImTextureID)m_RenderedFrameBuffer.GetTexture(), ImVec2(600, 400), ImVec2(0, 1), ImVec2(1, 0));
  }

}

void TaskWidget::RenderWidget() {
  OnUpdate(0.0f);
  OnRender();
  ImGui::Begin("Task", nullptr, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_MenuBar);
  OnImGuiRender();
  ImGui::End();
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

void TaskWidget::LoadTask(const std::string& filePath) {
  std::fstream fileStream(filePath);
  AttributeFile attributeFile;
  std::string fileName = FileManager::GetTaskFileNameFromPath(filePath);

  {
    cereal::JSONInputArchive inputArchive(fileStream);
    inputArchive(attributeFile);
  }

  m_Context.taskScene.emplace_back(new SceneElement(
          fileName,
          attributeFile.vertexShaderSource,
          attributeFile.fragmentShaderSource,
          attributeFile
          ));
  auto element = std::find_if(m_Context.scene.begin(), m_Context.scene.end(), [&](SceneElement* element) {
      return (element->GetSceneName() == fileName);
  });
  if (element == m_Context.scene.end()) {
    m_Context.scene.emplace_back(new SceneElement(
            fileName,
            attributeFile
    ));
  }
  fileStream.close();
}

void TaskWidget::LoadTaskImage(const std::string &filePath) {
  unsigned char* image_data = stbi_load(filePath.c_str(), &m_TaskImageWidth, &m_TaskImageHeight, nullptr, 4);

  //glGenTextures(1, &m_TaskTexture);
  //glBindTexture(GL_TEXTURE_2D, m_TaskTexture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_TaskImageWidth, m_TaskImageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
  stbi_image_free(image_data);
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

FrameBuffer& TaskWidget::GetTaskFrameBuffer() {
  return m_TaskFrameBuffer;
}

FrameBuffer& TaskWidget::GetRenderedFrameBuffer() {
  return m_RenderedFrameBuffer;
}