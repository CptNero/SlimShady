#ifndef SLIMSHADY_WIDGETBROKER_H
#define SLIMSHADY_WIDGETBROKER_H

#include <string>
#include <map>
#include <assert.h>
#include "Widget.h"

class WidgetBroker {

public:
    WidgetBroker() = default;

    ~WidgetBroker() = default;

    template <typename WidgetType, typename... Arguments>
    Widget* MakeWidget(std::string id, Arguments&&... ArgumentValues) {
      assert(m_Widgets.count(id) != 1);
      m_Widgets[id] = std::make_unique<WidgetType>(std::forward<Arguments>(ArgumentValues)...);
      return m_Widgets[id].get();
    }

    template <typename WidgetType>
    WidgetType* GetWidget(std::string id) {
      return (WidgetType*)m_Widgets[id].get();
    }
private:
    std::map<std::string, std::unique_ptr<Widget>> m_Widgets;
};


#endif //SLIMSHADY_WIDGETBROKER_H
