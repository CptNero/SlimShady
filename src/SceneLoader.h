#ifndef SLIMSHADY_SCENELOADER_H
#define SLIMSHADY_SCENELOADER_H


#include "SceneElement.h"

class SceneLoader {
  private:
    std::unordered_map<std::string, SceneElement*>* m_Scene;

  public:
    explicit SceneLoader(std::unordered_map<std::string, SceneElement*>* scene);
    ~SceneLoader();

    void InitializeScene();
};


#endif //SLIMSHADY_SCENELOADER_H
