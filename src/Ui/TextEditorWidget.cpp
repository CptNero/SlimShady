#include <imgui/imgui.h>

#include <fstream>

#include "TextEditorWidget.h"

TextEditorWidget::TextEditorWidget() {
  m_Editor.SetLanguageDefinition(m_Lang);
  m_Markers.insert(std::make_pair<int, std::string>(6, "Example error here:\nInclude file not found: \"TextEditor.h\""));
  m_Markers.insert(std::make_pair<int, std::string>(41, "Another example error"));
  m_Editor.SetErrorMarkers(m_Markers);

  {
    std::ifstream t(m_FileToEditPath);
    if (t.good())
    {
      std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
      m_Editor.SetText(str);
    }
  }
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
        auto textToSave = m_Editor.GetText();
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

void TextEditorWidget::InitializeWidget() {
  OnUpdate(0.0f);
  OnRender();
  ImGui::Begin("Editor", nullptr, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_MenuBar);
  OnImGuiRender();
  ImGui::End();
}
