#ifndef SLIMSHADY_SCENEEDITORWIDGET_H
#define SLIMSHADY_SCENEEDITORWIDGET_H


#include <list>
#include <functional>
#include <sstream>
#include "Widget.h"
#include "../SceneElement.h"
#include "TextEditorWidget.h"
#include <glm/gtc/type_ptr.hpp>

class SceneEditorWidget : public Widget {
  private:
    char m_SceneElementNameInputBuffer[1024] = "";
    float m_ClearColor[4] = { 0.2f, 0.3f, 0.8f, 1.0f };
    std::unordered_map<std::string, SceneElement*>* m_Scene;
    SceneElement* m_currentIteratedSceneElement;
    std::string m_currentIteratedSceneElementName;

    std::map<int, glm::vec3> m_Vertices;
    glm::vec3 m_VertexInput = glm::vec3(0.0f,0.0f,0.0f);
    uint32_t m_VertexEditIndex = 0;

    std::map<int, int> m_Indices;
    int m_IndexInput = 0;
    uint32_t m_IndexEditIndex = 0;

  public:
    SceneEditorWidget(std::unordered_map<std::string, SceneElement*>* scene);
    ~SceneEditorWidget();

    void OnUpdate(float deltaTime) override;
    void OnRender() override;
    void OnImGuiRender() override;
    void RenderWidget() override;

    void Recompile();
    void InsertElement();

    template<typename CollectionType>
    void CollectionEditor(
            std::map<int, CollectionType>&& collection,
            std::string&& name,
            CollectionType&& input,
            uint32_t&& indexer,
            std::function<void()> inputField,
            std::function<void()> printValues) {
      if(ImGui::TreeNode(name.c_str())) {
        if (ImGui::Button("Add")) {
          int key = collection.size();
          collection[key] = input;
        }
        ImGui::SameLine();
        if (ImGui::Button("Edit")) {
          collection[indexer] = input;
        }
        ImGui::SameLine();
        if (ImGui::Button("Delete")) {
          if (collection.size() >= 0) {
            collection.erase(std::prev(collection.end()));
          }
        }
        ImGui::SameLine();
        if (ImGui::Button("Up")) {
          if (indexer > 0)
            indexer--;
        }
        ImGui::SameLine();
        if (ImGui::Button("Down")) {
          if(indexer < collection.size() - 1)
            indexer++;
        }
        inputField();
        printValues();

        ImGui::TreePop();
      }
    }
};


#endif //SLIMSHADY_SCENEEDITORWIDGET_H
