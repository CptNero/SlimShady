#ifndef SLIMSHADY_TEXTEDITORWIDGET_H
#define SLIMSHADY_TEXTEDITORWIDGET_H


#include <ImGuiColorTextEdit/TextEditor.h>
#include "Widget.h"

class TextEditorWidget : public Widget {
  private:
    TextEditor m_Editor;
    TextEditor::LanguageDefinition m_Lang = TextEditor::LanguageDefinition::GLSL();
    TextEditor::ErrorMarkers m_Markers;
    const char* m_FileToEditPath = R"(legacy/res/shaders/Basic.shader)";
    TextEditor::Coordinates m_CursorPosition = m_Editor.GetCursorPosition();


  public:
    TextEditorWidget();
    ~TextEditorWidget();


    void OnUpdate(float deltaTime) override;
    void OnRender() override;
    void OnImGuiRender() override;
    void InitializeWidget() override;
};


#endif //SLIMSHADY_TEXTEDITORWIDGET_H
