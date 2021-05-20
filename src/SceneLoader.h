#ifndef SLIMSHADY_SCENELOADER_H
#define SLIMSHADY_SCENELOADER_H


#include "SceneElement.h"
#include "Frameworks/Context.h"

class SceneLoader {
  public:
    explicit SceneLoader(Context context);
    ~SceneLoader();

    void InitializeScene();

  private:
    Context m_Context;
};


#endif //SLIMSHADY_SCENELOADER_H
