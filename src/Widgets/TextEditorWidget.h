#ifndef SLIMSHADY_TEXTEDITORWIDGET_H
#define SLIMSHADY_TEXTEDITORWIDGET_H


#include <ImGuiColorTextEdit/TextEditor.h>
#include "Widget.h"
#include "../Frameworks/Configurations.h"
#include "../ShaderType.h"
#include "../SceneElement.h"

class TextEditorWidget : public Widget {
  private:
    TextEditor m_Editor;
    TextEditor::LanguageDefinition m_Lang = TextEditor::LanguageDefinition::GLSL();
    TextEditor::ErrorMarkers m_Markers;
    TextEditor::Coordinates m_CursorPosition = m_Editor.GetCursorPosition();
    std::string m_FileToEditPath;
    std::string m_CurrentShaderSource;
    SceneElement* m_CurrentSceneElement;
    ShaderType m_CurrentShaderType;
public:
    void SetCurrentSceneElement(SceneElement *currentSceneElement);

private:

  public:
    TextEditorWidget();
    ~TextEditorWidget();

    std::string GetEditorText();

    void SetEditorText(const std::string& text, ShaderType shaderType, const std::string& filePath);
    void Save();

    void OnUpdate(float deltaTime) override;
    void OnRender() override;
    void OnImGuiRender() override;
    void RenderWidget() override;

    void SetCurrentShaderType(ShaderType shaderType);
};


#endif //SLIMSHADY_TEXTEDITORWIDGET_H
