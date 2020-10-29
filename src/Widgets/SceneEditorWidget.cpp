#include <imgui/imgui.h>
#include "SceneEditorWidget.h"
#include "ConsoleWidget.h"
#include "../Frameworks/ShaderFileManager.h"

SceneEditorWidget::SceneEditorWidget(std::unordered_map<std::string, SceneElement*>* scene, TextEditorWidget* textEditorWidget) :
        m_Scene(scene),
        m_TextEditorWidget(textEditorWidget) {}

SceneEditorWidget::~SceneEditorWidget() = default;

void SceneEditorWidget::OnUpdate(float deltaTime) {
}

void SceneEditorWidget::OnRender() {
  glClearColor(m_ClearColor[0], m_ClearColor[1], m_ClearColor[2], m_ClearColor[3]);
}

void SceneEditorWidget::OnImGuiRender() {
  ImGui::ColorEdit4("Clear color", m_ClearColor, 1);

  ImGui::InputText("Element Name", m_SceneElementNameInputBuffer, IM_ARRAYSIZE(m_SceneElementNameInputBuffer));
  if(ImGui::Button("Add to scene")) {
    InsertElement();
  }
  //Show scene tree view
  for (auto sceneNameAndElement = m_Scene->begin(); sceneNameAndElement != m_Scene->end(); sceneNameAndElement++) {
    if (ImGui::TreeNode(sceneNameAndElement->first.c_str())) {
      auto sceneElement = sceneNameAndElement->second;

      if (ImGui::Button("Vertex")) {

        m_TextEditorWidget->SetEditorText(
                sceneElement->GetShaderSource(ShaderType::VERTEX),
                ShaderType::VERTEX,
                ShaderFileManager::GetShaderFilePath(sceneNameAndElement->first, ShaderType::VERTEX));
      }
      ImGui::SameLine();
      if (ImGui::Button("Fragment")) {
        m_TextEditorWidget->SetEditorText(
                sceneElement->GetShaderSource(ShaderType::FRAGMENT),
                ShaderType::FRAGMENT,
                ShaderFileManager::GetShaderFilePath(sceneNameAndElement->first, ShaderType::FRAGMENT));
      }
      ImGui::SameLine();
      //Delete element from tree view
      if (ImGui::Button("Delete")) {
        ShaderFileManager::DeleteVertexAndFragmentShaderFilesyName(sceneNameAndElement->first);
        sceneNameAndElement = m_Scene->erase(sceneNameAndElement);

        if (sceneNameAndElement == m_Scene->end()) {
          ImGui::TreePop();
          break;
        }
      }
      ImGui::Separator();
      ImGui::TreePop();
    }
  }
}

void SceneEditorWidget::RenderWidget() {
  OnUpdate(0.0f);
  OnRender();
  ImGui::Begin("Scene Editor");
  OnImGuiRender();
  ImGui::End();
}

void SceneEditorWidget::InsertElement() {
  if (strlen(m_SceneElementNameInputBuffer) == 0) {
    ConsoleWidget::LogMessage("Scene element name cannot be empty.");
    return;
  }

  m_Scene->insert(std::make_pair(m_SceneElementNameInputBuffer, new SceneElement(m_SceneElementNameInputBuffer)));
}
