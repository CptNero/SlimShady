#ifndef SLIMSHADY_CONTEXT_H
#define SLIMSHADY_CONTEXT_H

#include <unordered_map>
#include "../Widgets/WidgetBroker.h"
#include "../SceneElement.h"

class Context {
public:
    Context(WidgetBroker& widgetBroker, std::list<SceneElement*>& scene, std::list<SceneElement*>& taskScene);

    WidgetBroker& widgetBroker;

    std::list<SceneElement*>& scene;
    std::list<SceneElement*>& taskScene;
};


#endif //SLIMSHADY_CONTEXT_H
