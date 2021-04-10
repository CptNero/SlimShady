#ifndef SLIMSHADY_WIDGETBROKER_H
#define SLIMSHADY_WIDGETBROKER_H

#include <string>
#include <map>
#include <memory>
#include <assert.h>
#include "Widget.h"
#include "../WidgetType.h"

class WidgetBroker {

public:
    WidgetBroker() = default;

    ~WidgetBroker() = default;

    template <typename WidgetT, typename... Arguments>
    Widget* MakeWidget(WidgetType id, Arguments&&... ArgumentValues) {
      assert(m_Widgets.count(id) != 1);
      m_Widgets[id] = std::make_unique<WidgetT>(std::forward<Arguments>(ArgumentValues)...);
      return m_Widgets[id].get();
    }

    template <typename WidgetT>
    WidgetT* GetWidget(WidgetType id) {
      return (WidgetT*)m_Widgets[id].get();
    }
private:
    std::map<WidgetType, std::unique_ptr<Widget>> m_Widgets;
};


#endif //SLIMSHADY_WIDGETBROKER_H
