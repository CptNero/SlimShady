#ifndef SLIMSHADY_CONTEXT_H
#define SLIMSHADY_CONTEXT_H

#include <unordered_map>
#include "../Widgets/WidgetBroker.h"
#include "../SceneElement.h"

class Context {
public:
    Context(WidgetBroker& widgetBroker, std::list<SceneElement*>& scene);

    WidgetBroker& widgetBroker;
    std::list<SceneElement*>& scene;
};


#endif //SLIMSHADY_CONTEXT_H
