#include <imgui/imgui.h>
#include <iostream>
#include "SceneEditorWidget.h"
#include "ConsoleWidget.h"
#include "WidgetBroker.h"

SceneEditorWidget::SceneEditorWidget(Context context) :
        m_Context(context) {
}

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
  for (auto sceneNameAndElement = m_Context.scene.begin(); sceneNameAndElement != m_Context.scene.end(); sceneNameAndElement++) {
    if (ImGui::TreeNode(sceneNameAndElement->first.c_str())) {
      m_currentIteratedSceneElementName = sceneNameAndElement->first;
      m_currentIteratedSceneElement = sceneNameAndElement->second;
      TextEditorWidget* textEditorWidget = (TextEditorWidget*) m_Context.widgetBroker.GetWidget("TextEditor");

      if (ImGui::Button("Vertex")) {

        textEditorWidget->SetCurrentSceneElement(m_currentIteratedSceneElement);
        textEditorWidget->SetCurrentShaderType(ShaderType::VERTEX);
        textEditorWidget->SetEditorText(
                m_currentIteratedSceneElement->GetShaderSource(ShaderType::VERTEX),
                ShaderType::VERTEX,
                FileManager::GetShaderFilePath(sceneNameAndElement->first, ShaderType::VERTEX));
      }
      ImGui::SameLine();
      if (ImGui::Button("Fragment")) {
        textEditorWidget->SetCurrentSceneElement(m_currentIteratedSceneElement);
        textEditorWidget->SetCurrentShaderType(ShaderType::FRAGMENT);
        textEditorWidget->SetEditorText(
                m_currentIteratedSceneElement->GetShaderSource(ShaderType::FRAGMENT),
                ShaderType::FRAGMENT,
                FileManager::GetShaderFilePath(sceneNameAndElement->first, ShaderType::FRAGMENT));
      }
      ImGui::SameLine();
      if (ImGui::Button("Save")) {
        Save();
      }
      if (ImGui::Button("Recompile")) {
        Recompile();
      }
      ImGui::SameLine();
      //Delete element from tree view
      if (ImGui::Button("Delete")) {
        FileManager::DeleteVertexAndFragmentShaderFilesByName(sceneNameAndElement->first);
        sceneNameAndElement = m_Context.scene.erase(sceneNameAndElement);

        if (sceneNameAndElement == m_Context.scene.end()) {
          ImGui::TreePop();
          break;
        }
      }

      if (ImGui::TreeNode("Vertices")) {
        if (ImGui::Button("Add")) {
          m_currentIteratedSceneElement->m_Vertices
          .insert(m_currentIteratedSceneElement->m_Vertices.end(), {0.0f, 0.0f, 0.0f});
        }
        ImGui::SameLine();
        if (ImGui::Button("Delete")) {
          m_currentIteratedSceneElement->m_Vertices.pop_back();
          m_currentIteratedSceneElement->m_Vertices.pop_back();
          m_currentIteratedSceneElement->m_Vertices.pop_back();
        }

        for(int i = 0; i < m_currentIteratedSceneElement->m_Vertices.size(); i+=3) {
          std::stringstream label;
          label << "Vertex #"  << i / 3 << std::endl;
          ImGui::InputFloat3(label.str().c_str(), &m_currentIteratedSceneElement->m_Vertices[i]);
        }

        ImGui::TreePop();
      }

      if (ImGui::TreeNode("Indices")) {
        if (ImGui::Button("Add")) {
          m_currentIteratedSceneElement->m_Indices.emplace_back(0);
        }
        ImGui::SameLine();
        if (ImGui::Button("Delete")) {
          m_currentIteratedSceneElement->m_Indices.pop_back();
        }

        for (int i = 0; i < m_currentIteratedSceneElement->m_Indices.size(); i++) {
          std::stringstream label;
          label << "Index #"  << i << std::endl;
          ImGui::InputInt(label.str().c_str(), (int*)&m_currentIteratedSceneElement->m_Indices[i]);

        }
        ImGui::TreePop();
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
  TextEditorWidget* textEditorWidget = (TextEditorWidget*) m_Context.widgetBroker.GetWidget("TextEditor");

  m_Context.scene[m_currentIteratedSceneElementName] = new SceneElement(
          m_currentIteratedSceneElementName,
          m_currentIteratedSceneElement->GetShaderSource(ShaderType::VERTEX),
          m_currentIteratedSceneElement->GetShaderSource(ShaderType::FRAGMENT),
          m_currentIteratedSceneElement->m_Vertices,
          m_currentIteratedSceneElement->m_Indices);
  textEditorWidget->SetCurrentSceneElement(m_Context.scene[m_currentIteratedSceneElementName]);
}

void SceneEditorWidget::Save() {
  FileManager::VertexAttributeFile vertexAttributeFile;
  std::vector<float> vertices = m_currentIteratedSceneElement->GetVertices();
  std::vector<glm::vec3> vertexVector;

  for(int i = 0; i < m_currentIteratedSceneElement->GetVertices().size(); i+=3) {
    vertexVector.emplace_back(glm::vec3(vertices[i],vertices[i+1],vertices[i+2]));
  }

  vertexAttributeFile.Vertices = vertexVector;
  vertexAttributeFile.Indices = m_currentIteratedSceneElement->GetIndices();

  std::string vertexAttributeFileAsString = FileManager::ConvertVertexAttributeFileToString(vertexAttributeFile);
  FileManager::UpdateFile(
          FileManager::GetVertexAttributeFilePath(m_currentIteratedSceneElement->GetSceneName()),
          vertexAttributeFileAsString);
}

void SceneEditorWidget::InsertElement() {
  if (strlen(m_SceneElementNameInputBuffer) == 0) {
    ConsoleWidget::LogMessage("Scene element name cannot be empty.");
    return;
  }

  m_Context.scene.insert(std::make_pair(m_SceneElementNameInputBuffer, new SceneElement(m_SceneElementNameInputBuffer)));
}
