#ifndef SLIMSHADY_SCENELOADER_H
#define SLIMSHADY_SCENELOADER_H


#include "SceneElement.h"
#include "Frameworks/Context.h"

class SceneLoader {
  private:
    Context m_Context;

  public:
    explicit SceneLoader(Context context);
    ~SceneLoader();

    void InitializeScene();
};


#endif //SLIMSHADY_SCENELOADER_H
