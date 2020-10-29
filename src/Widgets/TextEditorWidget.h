#ifndef SLIMSHADY_TEXTEDITORWIDGET_H
#define SLIMSHADY_TEXTEDITORWIDGET_H


#include <ImGuiColorTextEdit/TextEditor.h>
#include "Widget.h"
#include "../Frameworks/Configurations.h"
#include "../ShaderTypes.h"

class TextEditorWidget : public Widget {
  private:
    TextEditor m_Editor;
    TextEditor::LanguageDefinition m_Lang = TextEditor::LanguageDefinition::GLSL();
    TextEditor::ErrorMarkers m_Markers;
    TextEditor::Coordinates m_CursorPosition = m_Editor.GetCursorPosition();
    std::string m_FileToEditPath;
    std::string m_CurrentShaderSource;
    ShaderType m_CurrentShaderType = ShaderType::NONE;

  public:
    TextEditorWidget();
    ~TextEditorWidget();

    std::string GetEditorText();

    void SetEditorText(const std::string& text, ShaderType shaderType, const std::string& filePath);

    void OnUpdate(float deltaTime) override;
    void OnRender() override;
    void OnImGuiRender() override;
    void RenderWidget() override;
};


#endif //SLIMSHADY_TEXTEDITORWIDGET_H
