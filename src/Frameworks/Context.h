#ifndef SLIMSHADY_CONTEXT_H
#define SLIMSHADY_CONTEXT_H

#include "../Widgets/WidgetBroker.h"

class Context {
public:
    Context(WidgetBroker &widgetBroker);

    WidgetBroker& widgetBroker;
};


#endif //SLIMSHADY_CONTEXT_H
