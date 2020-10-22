#ifndef SLIMSHADY_TEXTEDITORWIDGET_H
#define SLIMSHADY_TEXTEDITORWIDGET_H


#include <ImGuiColorTextEdit/TextEditor.h>
#include "Widget.h"
#include "../Configurations.h"
#include "../ShaderTypes.h"

class TextEditorWidget : public Widget {
  private:
    TextEditor m_Editor;
    TextEditor::LanguageDefinition m_Lang = TextEditor::LanguageDefinition::GLSL();
    TextEditor::ErrorMarkers m_Markers;
    TextEditor::Coordinates m_CursorPosition = m_Editor.GetCursorPosition();
    const char* m_FileToEditPath = "";
    std::string m_VertexShaderSource = ReadFile(Configurations::GetVertexShaderSourcePath());
    std::string m_FragmentShaderSource = ReadFile(Configurations::GetFragmentShaderSourcePath());
    ShaderType m_CurrentShaderType = ShaderType::NONE;

  public:
    TextEditorWidget();
    ~TextEditorWidget();

    std::string GetVertexShaderSource();
    std::string GetFragmentShaderSource();

    void OnUpdate(float deltaTime) override;
    void OnRender() override;
    void OnImGuiRender() override;
    void RenderWidget() override;

    static const char* ReadFile(const char* filePath);
};


#endif //SLIMSHADY_TEXTEDITORWIDGET_H