#include <imgui/imgui.h>
#include <iostream>
#include "SceneEditorWidget.h"
#include "ConsoleWidget.h"
#include "../Frameworks/ShaderFileManager.h"
#include "WidgetBroker.h"

SceneEditorWidget::SceneEditorWidget(std::unordered_map<std::string, SceneElement*>* scene) :
        m_Scene(scene) {}

SceneEditorWidget::~SceneEditorWidget() = default;

void SceneEditorWidget::OnUpdate(float deltaTime) {
}

void SceneEditorWidget::OnRender() {
  glClearColor(m_ClearColor[0], m_ClearColor[1], m_ClearColor[2], m_ClearColor[3]);
}

void SceneEditorWidget::OnImGuiRender() {
  ImGui::ColorEdit4("Clear color", m_ClearColor, 1);

  ImGui::InputText("Element name", m_SceneElementNameInputBuffer, IM_ARRAYSIZE(m_SceneElementNameInputBuffer));
  if(ImGui::Button("Add to scene")) {
    InsertElement();
  }
  //Show scene tree view
  for (auto sceneNameAndElement = m_Scene->begin(); sceneNameAndElement != m_Scene->end(); sceneNameAndElement++) {
    if (ImGui::TreeNode(sceneNameAndElement->first.c_str())) {
      m_currentIteratedSceneElementName = sceneNameAndElement->first;
      m_currentIteratedSceneElement = sceneNameAndElement->second;
      TextEditorWidget* textEditorWidget = (TextEditorWidget*) WidgetBroker::GetWidget("TextEditor");

      if (ImGui::Button("Vertex")) {

        textEditorWidget->SetCurrentSceneElement(m_currentIteratedSceneElement);
        textEditorWidget->SetCurrentShaderType(ShaderType::VERTEX);
        textEditorWidget->SetEditorText(
                m_currentIteratedSceneElement->GetShaderSource(ShaderType::VERTEX),
                ShaderType::VERTEX,
                ShaderFileManager::GetShaderFilePath(sceneNameAndElement->first, ShaderType::VERTEX));
      }
      ImGui::SameLine();
      if (ImGui::Button("Fragment")) {
        textEditorWidget->SetCurrentSceneElement(m_currentIteratedSceneElement);
        textEditorWidget->SetCurrentShaderType(ShaderType::FRAGMENT);
        textEditorWidget->SetEditorText(
                m_currentIteratedSceneElement->GetShaderSource(ShaderType::FRAGMENT),
                ShaderType::FRAGMENT,
                ShaderFileManager::GetShaderFilePath(sceneNameAndElement->first, ShaderType::FRAGMENT));
      }
      ImGui::SameLine();
      if (ImGui::Button("Recompile")) {
        Recompile();
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
      if (ImGui::TreeNode("Vertices")) {

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

void SceneEditorWidget::Recompile() {
  TextEditorWidget* textEditorWidget = (TextEditorWidget*) WidgetBroker::GetWidget("TextEditor");

  (*m_Scene)[m_currentIteratedSceneElementName] = new SceneElement(
          m_currentIteratedSceneElementName,
          m_currentIteratedSceneElement->GetShaderSource(ShaderType::VERTEX),
          m_currentIteratedSceneElement->GetShaderSource(ShaderType::FRAGMENT));
  textEditorWidget->SetCurrentSceneElement((*m_Scene)[m_currentIteratedSceneElementName]);
}

void SceneEditorWidget::InsertElement() {
  if (strlen(m_SceneElementNameInputBuffer) == 0) {
    ConsoleWidget::LogMessage("Scene element name cannot be empty.");
    return;
  }

  m_Scene->insert(std::make_pair(m_SceneElementNameInputBuffer, new SceneElement(m_SceneElementNameInputBuffer)));
}
