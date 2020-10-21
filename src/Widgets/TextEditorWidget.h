#ifndef SLIMSHADY_TEXTEDITORWIDGET_H
#define SLIMSHADY_TEXTEDITORWIDGET_H


#include <ImGuiColorTextEdit/TextEditor.h>
#include "Widget.h"
#include "../Configurations.h"

class TextEditorWidget : public Widget {
  private:
    TextEditor m_Editor;
    TextEditor::LanguageDefinition m_Lang = TextEditor::LanguageDefinition::GLSL();
    TextEditor::ErrorMarkers m_Markers;
    TextEditor::Coordinates m_CursorPosition = m_Editor.GetCursorPosition();
    const char* m_FileToEditPath = R"(src/res/shaders/Fragment.shader)";
    static const char* m_VertexShaderSource;
    static const char* m_FragmentShaderSource;

  public:
    TextEditorWidget();
    ~TextEditorWidget();

    static const char* GetVertexShaderSource();
    static const char* GetFragmentShaderSource();

    void OnUpdate(float deltaTime) override;
    void OnRender() override;
    void OnImGuiRender() override;
    void RenderWidget() override;

    static std::string ReadFile(const char* filePath);
};


#endif //SLIMSHADY_TEXTEDITORWIDGET_H
