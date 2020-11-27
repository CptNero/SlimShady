#ifndef SLIMSHADY_WIDGETBROKER_H
#define SLIMSHADY_WIDGETBROKER_H

#include <string>
#include <map>
#include <assert.h>
#include "Widget.h"

class WidgetBroker {

public:
    WidgetBroker() = default;
    ~WidgetBroker() {
      for(auto widgetPointer = m_Widgets.begin(); widgetPointer != m_Widgets.end(); widgetPointer++) {
        widgetPointer->second.release();
      }
    }

    template <typename WidgetType, typename... Arguments>
    static Widget* MakeWidget(std::string id, Arguments&&... ArgumentValues) {
      assert(m_Widgets.count(id) != 1);
      m_Widgets[id] = std::make_unique<WidgetType>(std::forward<Arguments>(ArgumentValues)...);
      return m_Widgets[id].get();
    }

    static Widget* GetWidget(std::string id) {
      return m_Widgets[id].get();
    }
private:
    inline static std::map<std::string, std::unique_ptr<Widget>> m_Widgets;
};


#endif //SLIMSHADY_WIDGETBROKER_H
