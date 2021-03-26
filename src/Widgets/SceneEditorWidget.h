#ifndef SLIMSHADY_SCENEEDITORWIDGET_H
#define SLIMSHADY_SCENEEDITORWIDGET_H


#include <list>
#include <functional>
#include <sstream>
#include "Widget.h"
#include "../SceneElement.h"
#include "TextEditorWidget.h"
#include "../Frameworks/Context.h"
#include "../UniformManager.h"
#include <glm/gtc/type_ptr.hpp>

class SceneEditorWidget : public Widget {
  private:
    Context m_Context;
    UniformManager& m_UniformManager;

    char m_SceneElementNameInputBuffer[1024] = "";
    float m_ClearColor[4] = { 0.2f, 0.3f, 0.8f, 1.0f };
    SceneElement* m_currentSceneElement;

  public:
    SceneEditorWidget(Context context, UniformManager& uniformManager);
    ~SceneEditorWidget();

    void OnUpdate(float deltaTime) override;
    void OnRender() override;
    void OnImGuiRender() override;
    void RenderWidget() override;

    void Recompile();
    void Save();
    void InsertElement();
};


#endif //SLIMSHADY_SCENEEDITORWIDGET_H
