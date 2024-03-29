#include <imgui/imgui.h>
#include <iostream>
#include "SceneEditorWidget.h"
#include "ConsoleWidget.h"
#include "../Camera.h"
#include "FileBrowserWidget.h"

SceneEditorWidget::SceneEditorWidget(Context context, UniformManager& uniformManager) :
        m_Context(context),
        m_UniformManager(uniformManager){
}

SceneEditorWidget::~SceneEditorWidget() = default;

void SceneEditorWidget::OnUpdate(float deltaTime) {
}

void SceneEditorWidget::OnRender() {
  glClearColor(m_ClearColor[0], m_ClearColor[1], m_ClearColor[2], m_ClearColor[3]);
}

void SceneEditorWidget::OnImGuiRender() {
  FileBrowserWidget* fileBrowserWidget = m_Context.widgetBroker.GetWidget<FileBrowserWidget>(WidgetType::FILE_BROWSER);
  ImGui::ColorEdit4("Clear color", m_ClearColor, 1);

  ImGui::InputText("Element name", m_SceneElementNameInputBuffer, IM_ARRAYSIZE(m_SceneElementNameInputBuffer));
  if(ImGui::Button("Add to scene")) {
    InsertElement();
  }
  if (ImGui::Button("Select")) {
    fileBrowserWidget->OpenFileBrowser(FileBrowserWidget::Texture);
  }
  ImGui::SameLine();
  ImGui::Text("%s", fileBrowserWidget->QueryFileBrowser(FileBrowserWidget::Texture).c_str());

  //Show scene tree view
  for (auto sceneElement = m_Context.scene.begin(); sceneElement != m_Context.scene.end(); sceneElement++) {
    m_currentSceneElement = *sceneElement;
    if (ImGui::TreeNode(m_currentSceneElement->GetSceneName().c_str())) {
      if (ImGui::Button("Save")) {
        Save();
      }
      ImGui::SameLine();
      if (ImGui::Button("Recompile")) {
        Recompile();
      }
      ImGui::SameLine();
      //Delete element from tree view
      if (ImGui::Button("Delete")) {
        FileManager::DeleteVertexAndFragmentShaderFilesByName(m_currentSceneElement->GetSceneName());
        FileManager::DeleteFileByPath(
                Configurations::VertexAttributeFilePath + m_currentSceneElement->GetSceneName() + ".vrtxatrb");
        sceneElement = m_Context.scene.erase(sceneElement);

        if (sceneElement == m_Context.scene.end()) {
          ImGui::TreePop();
          break;
        }
      }

      if (ImGui::TreeNode("Vertices")) {
        std::vector<float>& vertices = m_currentSceneElement->GetVertices();

        if (ImGui::Button("Add")) {
          vertices
          .insert(vertices.end(), {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f});
        }
        ImGui::SameLine();
        if (ImGui::Button("Delete")) {
          if(m_currentSceneElement->GetVertices().size() >= 8) {
            m_currentSceneElement->GetVertices().resize(vertices.size() - 8);
          }
        }

        for(int i = 0; i < vertices.size(); i+=8) {
          std::stringstream vertexCoordinatelabel;
          std::stringstream colorCoordinateLabel;
          std::stringstream textureCoordinateLabel;
          vertexCoordinatelabel << "Vertex #"  << i / 8 << std::endl;
          colorCoordinateLabel << "Color #"  << i / 8 << std::endl;
          textureCoordinateLabel << "Texture #"  << i / 8 << std::endl;

          //First 3 values are the position coordinates
          ImGui::InputFloat3(vertexCoordinatelabel.str().c_str(), &vertices[i]);
          //3rd to 6th values are the color coordinates
          ImGui::InputFloat3(colorCoordinateLabel.str().c_str(), &vertices[i + 3]);
          //7th to 8th values are the texture coordinates
          ImGui::InputFloat2(textureCoordinateLabel.str().c_str(), &vertices[i + 6]);
          ImGui::Separator();
        }

        ImGui::TreePop();
      }

      if (ImGui::TreeNode("Indices")) {
        std::vector<uint32_t>& indices = m_currentSceneElement->GetIndices();

        if (ImGui::Button("Add")) {
          indices.emplace_back(0);
        }
        ImGui::SameLine();
        if (ImGui::Button("Delete")) {
          if (!indices.empty()) {
            indices.pop_back();
          }
        }

        for (int i = 0; i < indices.size(); i++) {
          std::stringstream label;
          label << "Index #"  << i << std::endl;
          ImGui::InputInt(label.str().c_str(), (int*)&indices[i]);

        }
        ImGui::TreePop();
      }

      if (ImGui::TreeNode("Textures")) {
        std::list<std::string>& texturePaths = m_currentSceneElement->GetTexturePaths();

        if (ImGui::Button("Add")) {
          texturePaths.emplace_back(fileBrowserWidget->QueryFileBrowser(FileBrowserWidget::Texture));
        }
        ImGui::SameLine();
        if (ImGui::Button("Delete")) {
          if (!texturePaths.empty()) {
            texturePaths.pop_back();
          }
        }
        int counter = 0;
        for(auto const& texturePath : texturePaths) {
          ImGui::Text("Id: %d Name: %s", counter, FileManager::GetTextureFileNameFromPath(texturePath).c_str());
          counter ++;
        }
        ImGui::TreePop();
      }

      if (ImGui::TreeNode("Uniforms")) {
        std::map<std::string, int>& uniforms = m_UniformManager.GetUniformLocationCache();

        for(auto const& uniform : uniforms) {
          if (uniform.first.find(m_currentSceneElement->GetSceneName()) != std::string::npos) {
            ImGui::Text("Id: %d Name: %s", uniform.second, uniform.first.c_str());
          }
        }

        ImGui::TreePop();
      }

      ImGui::Separator();
      ImGui::TreePop();
    }
  }
  ImGui::Separator();
  if (ImGui::TreeNode("Tasks")) {
    for (auto const& element : m_Context.taskScene) {
      ImGui::Text("%s", element->GetSceneName().c_str());
    }
    ImGui::TreePop();
  }
  if( Configurations::IsDebugEnabled) {
    ImGui::Text("Debug:");

    ImGui::Text("Camera coords: %f %f %f", Camera::Position.x, Camera::Position.y, Camera::Position.z);

    TextEditorWidget* textEditorWidget = m_Context.widgetBroker.GetWidget<TextEditorWidget>(WidgetType::TEXT_EDITOR);

    ImGui::Text("CurrentSceneElement: %s", textEditorWidget->GetCurrentSceneElement()->GetSceneName().c_str());
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
  TextEditorWidget* textEditorWidget = m_Context.widgetBroker.GetWidget<TextEditorWidget>(WidgetType::TEXT_EDITOR);

  std::replace(
          m_Context.scene.begin(),
          m_Context.scene.end(),
          m_currentSceneElement,
          new SceneElement(
                  m_currentSceneElement->GetSceneName(),
                  m_currentSceneElement->GetShaderSource(ShaderType::VERTEX),
                  m_currentSceneElement->GetShaderSource(ShaderType::FRAGMENT),
                  m_currentSceneElement->GetVertices(),
                  m_currentSceneElement->GetIndices(),
                  m_currentSceneElement->GetTexturePaths()));
  m_UniformManager.GetUniformLocationCache().clear();
  textEditorWidget->SetCurrentSceneElement(
          *std::find_if(m_Context.scene.begin(), m_Context.scene.end(), [&](SceneElement* element) {
      return (element->GetSceneName() == m_currentSceneElement->GetSceneName());
  }));
}

void SceneEditorWidget::Save() {
  AttributeFile vertexAttributeFile;
  std::vector<float> vertices = m_currentSceneElement->GetVertices();

  vertexAttributeFile.Vertices = vertices;
  vertexAttributeFile.Indices = m_currentSceneElement->GetIndices();
  vertexAttributeFile.texturePaths = m_currentSceneElement->GetTexturePaths();

  std::string vertexAttributeFileAsString = FileManager::ConvertVertexAttributeFileToString(vertexAttributeFile);
  FileManager::UpdateFile(
          FileManager::GetVertexAttributeFilePath(m_currentSceneElement->GetSceneName()),
          vertexAttributeFileAsString);
}

void SceneEditorWidget::InsertElement() {
  if (strlen(m_SceneElementNameInputBuffer) == 0) {
    ConsoleWidget::LogMessage("Scene element name cannot be empty.");
    return;
  }

  m_Context.scene.emplace_back(new SceneElement(m_SceneElementNameInputBuffer));
}
