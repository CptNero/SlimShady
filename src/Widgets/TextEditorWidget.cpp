#include <imgui/imgui.h>
#include <iostream>

#include "TextEditorWidget.h"
#include "FileBrowserWidget.h"


TextEditorWidget::TextEditorWidget(Context context) : m_Context(context)
{
  m_SourceFileWasChanged = false;
  m_Editor.SetLanguageDefinition(m_Lang);
  m_Editor.SetErrorMarkers(m_Markers);

  if (!context.scene.empty()) {
    SetDefaultSourceFile();
  } else {
    m_CurrentSceneElement = new SceneElement();
    m_CurrentShaderType = ShaderType::NONE;
  }
}

TextEditorWidget::~TextEditorWidget() = default;

void TextEditorWidget::OnUpdate(float deltaTime) {

}

void TextEditorWidget::OnRender() {
}

void TextEditorWidget::OnImGuiRender() {
  FileBrowserWidget* fileBrowser = m_Context.widgetBroker.GetWidget<FileBrowserWidget>(WidgetType::FILE_BROWSER);

  if (ImGui::BeginMenuBar())
  {
    if (ImGui::BeginMenu("File"))
    {
      if (ImGui::MenuItem("Open Vertex Shaders", "Ctrl-O")) {
        fileBrowser->OpenFileBrowser(FileBrowserWidget::VertexShader);
        m_SourceFileWasChanged = true;
      }

      if (ImGui::MenuItem("Open Fragment Shaders", "Ctrl-O")) {
        fileBrowser->OpenFileBrowser(FileBrowserWidget::FragmentShader);
        m_SourceFileWasChanged = true;
      }

      if (ImGui::MenuItem("Save", "Ctrl-S"))
      {
        Save();
      }

      ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Edit"))
    {
      bool ro = m_Editor.IsReadOnly();
      if (ImGui::MenuItem("Read-only mode", nullptr, &ro))
        m_Editor.SetReadOnly(ro);
      ImGui::Separator();

      if (ImGui::MenuItem("Undo", "ALT-Backspace", nullptr, !ro && m_Editor.CanUndo()))
        m_Editor.Undo();
      if (ImGui::MenuItem("Redo", "Ctrl-Y", nullptr, !ro && m_Editor.CanRedo()))
        m_Editor.Redo();

      ImGui::Separator();

      if (ImGui::MenuItem("Copy", "Ctrl-C", nullptr, m_Editor.HasSelection()))
        m_Editor.Copy();
      if (ImGui::MenuItem("Cut", "Ctrl-X", nullptr, !ro && m_Editor.HasSelection()))
        m_Editor.Cut();
      if (ImGui::MenuItem("Delete", "Del", nullptr, !ro && m_Editor.HasSelection()))
        m_Editor.Delete();
      if (ImGui::MenuItem("Paste", "Ctrl-V", nullptr, !ro && ImGui::GetClipboardText() != nullptr))
        m_Editor.Paste();

      ImGui::Separator();

      if (ImGui::MenuItem("Select all", nullptr, nullptr))
        m_Editor.SetSelection(TextEditor::Coordinates(), TextEditor::Coordinates(m_Editor.GetTotalLines(), 0));

      ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("View"))
    {
      if (ImGui::MenuItem("Dark palette"))
        m_Editor.SetPalette(TextEditor::GetDarkPalette());
      if (ImGui::MenuItem("Light palette"))
        m_Editor.SetPalette(TextEditor::GetLightPalette());
      if (ImGui::MenuItem("Retro blue palette"))
        m_Editor.SetPalette(TextEditor::GetRetroBluePalette());
      ImGui::EndMenu();
    }
    ImGui::EndMenuBar();
  }

  ImGui::Text("%6d/%-6d %6d lines  | %s | %s | %s | %s", m_CursorPosition.mLine + 1, m_CursorPosition.mColumn + 1, m_Editor.GetTotalLines(),
              m_Editor.IsOverwrite() ? "Ovr" : "Ins",
              m_Editor.CanUndo() ? "*" : " ",
              m_Editor.GetLanguageDefinition().mName.c_str(), (ShaderTypes[static_cast<int>(m_CurrentShaderType)]).c_str());
  ImGui::Text("%s", m_FileToEditPath.c_str());

  if (m_SourceFileWasChanged &&
     (fileBrowser->HasSelected(FileBrowserWidget::VertexShader) ||
      fileBrowser->HasSelected(FileBrowserWidget::FragmentShader))) {
    ChangeSourceFile();
  }

  m_Editor.Render("TextEditor");
}

void TextEditorWidget::RenderWidget() {
  OnUpdate(0.0f);
  OnRender();
  ImGui::Begin("Code Editor", nullptr, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_MenuBar);
  OnImGuiRender();
  ImGui::End();
}

void TextEditorWidget::Save() {
  std::string editorText = m_Editor.GetText();

  //Remove the new useless end line that the editor adds
  editorText.pop_back();

  FileManager::UpdateFile(m_FileToEditPath, editorText);
  m_CurrentSceneElement->SetShaderSource(editorText, m_CurrentShaderType);
}

std::string TextEditorWidget::GetEditorText() {
  return m_Editor.GetText();
}

void TextEditorWidget::SetEditorText(const std::string& text, ShaderType shaderType, const std::string& filePath) {
  m_FileToEditPath = filePath;
  m_CurrentShaderType = shaderType;
  m_Editor.SetText(text);
}

void TextEditorWidget::SetCurrentSceneElement(SceneElement* currentSceneElement) {
  m_CurrentSceneElement = currentSceneElement;
}

void TextEditorWidget::SetCurrentShaderType(ShaderType shaderType) {
  m_CurrentShaderType = shaderType;
}

void TextEditorWidget::ChangeSourceFile() {
  FileBrowserWidget* fileBrowser = m_Context.widgetBroker.GetWidget<FileBrowserWidget>(WidgetType::FILE_BROWSER);

  std::string filePath = fileBrowser->QueryFileBrowser(fileBrowser->m_LastOpenedBy);
  std::string sceneElementName = FileManager::GetShaderFileNameFromPath(filePath);
  ShaderType shaderType = FileManager::GetShaderTypeFromPath(filePath);

  auto elementQuery = std::find_if(m_Context.scene.begin(), m_Context.scene.end(), [&](SceneElement* element) {
      return ((element->GetSceneName()) == sceneElementName);
  });

  if (!(elementQuery == m_Context.scene.end())) {
    SceneElement* sceneElement = *elementQuery;

    SetCurrentSceneElement(sceneElement);
    SetCurrentShaderType(shaderType);
    SetEditorText(sceneElement->GetShaderSource(shaderType), shaderType, filePath);

    m_SourceFileWasChanged = false;
  }
}

void TextEditorWidget::SetDefaultSourceFile() {
  SceneElement* sceneElement = *m_Context.scene.begin();
  ShaderType shaderType = ShaderType::FRAGMENT;

  SetCurrentSceneElement(sceneElement);
  SetCurrentShaderType(shaderType);
  SetEditorText(sceneElement->GetShaderSource(shaderType),
                shaderType,
                sceneElement->GetShaderSourcePath(shaderType));
}

SceneElement *TextEditorWidget::GetCurrentSceneElement() const {
    return m_CurrentSceneElement;
}

