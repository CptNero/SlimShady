#ifndef SLIMSHADY_SCENEEDITORWIDGET_H
#define SLIMSHADY_SCENEEDITORWIDGET_H


#include <list>
#include "Widget.h"
#include "../SceneElement.h"
#include "TextEditorWidget.h"

class SceneEditorWidget : public Widget {
  private:
    char m_SceneElementNameInputBuffer[1024] = "";
    float m_ClearColor[4] = { 0.2f, 0.3f, 0.8f, 1.0f };
    std::unordered_map<std::string, SceneElement*>* m_Scene;
    SceneElement* m_currentIteratedSceneElement;
    std::string m_currentIteratedSceneElementName;

  public:
    SceneEditorWidget(std::unordered_map<std::string, SceneElement*>* scene);
    ~SceneEditorWidget();

    void OnUpdate(float deltaTime) override;
    void OnRender() override;
    void OnImGuiRender() override;
    void RenderWidget() override;

    void Recompile();
    void InsertElement();
};


#endif //SLIMSHADY_SCENEEDITORWIDGET_H
