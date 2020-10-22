#include <imgui/imgui.h>

#include <fstream>
#include <filesystem>
#include <iostream>

#include "TextEditorWidget.h"
#include "../VertexArray.h"
#include "ConsoleWidget.h"


TextEditorWidget::TextEditorWidget()
{
  m_Editor.SetLanguageDefinition(m_Lang);
  m_Editor.SetErrorMarkers(m_Markers);
}

TextEditorWidget::~TextEditorWidget() = default;

void TextEditorWidget::OnUpdate(float deltaTime) {

}

void TextEditorWidget::OnRender() {
}

void TextEditorWidget::OnImGuiRender() {
  if (ImGui::BeginMenuBar())
  {
    if (ImGui::BeginMenu("File"))
    {
      if (ImGui::MenuItem("Save"))
      {
        if(!std::filesystem::exists(m_FileToEditPath))
          std::cout << "File does not exist at:" << m_FileToEditPath << std::endl;

        std::ofstream file;
        file.open(m_FileToEditPath);
        file << m_Editor.GetText();
        file.close();

        switch(m_CurrentShaderType) {
          case ShaderType::VERTEX:
            m_VertexShaderSource = m_Editor.GetText();
            break;

          case ShaderType::FRAGMENT:
            m_FragmentShaderSource = m_Editor.GetText();
            break;
        };
      }
      if (ImGui::MenuItem("Vertex"))
      {
        m_FileToEditPath = Configurations::GetVertexShaderSourcePath();
        m_CurrentShaderType = ShaderType::VERTEX;
        m_Editor.SetText(ReadFile(m_FileToEditPath));
      }
      if (ImGui::MenuItem("Fragment"))
      {
        m_FileToEditPath = Configurations::GetFragmentShaderSourcePath();
        m_CurrentShaderType = ShaderType::FRAGMENT;
        m_Editor.SetText(ReadFile(m_FileToEditPath));
      }
      ImGui::MenuItem("Quit");
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
              m_Editor.GetLanguageDefinition().mName.c_str(), m_FileToEditPath);

  m_Editor.Render("TextEditor");
}

void TextEditorWidget::RenderWidget() {
  OnUpdate(0.0f);
  OnRender();
  ImGui::Begin("Editor", nullptr, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_MenuBar);
  OnImGuiRender();
  ImGui::End();
}

const char* TextEditorWidget::ReadFile(const char* filePath) {

  if(!std::filesystem::exists(filePath)) {
    if (Configurations::GetIsDebugEnabled()) {
      ConsoleWidget::LogMessage(std::string("File at: ") + filePath + " couldn't be found.");
    }
  }

  std::ifstream file(filePath);
  if (file.good())
  {
    auto* file_content = new std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    return file_content->c_str();
  }

  if (Configurations::GetIsDebugEnabled()) {
    ConsoleWidget::LogMessage(std::string("Failed to read file"));
  }

  return "";
}

std::string TextEditorWidget::GetVertexShaderSource() {
  return m_VertexShaderSource;
}

std::string TextEditorWidget::GetFragmentShaderSource() {
  return m_FragmentShaderSource;
}
