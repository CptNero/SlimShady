#ifndef SLIMSHADY_CONSOLEWIDGET_H
#define SLIMSHADY_CONSOLEWIDGET_H


#include "Widget.h"
#include "../Frameworks/Context.h"

class ConsoleWidget : public Widget {
public:
    ConsoleWidget(Context& context);
    ~ConsoleWidget();

    void OnUpdate(float deltaTime) override;
    void OnRender() override;
    void OnImGuiRender() override;
    void RenderWidget() override;

    void LogInput(const std::string& logInput);
    void static LogMessage(const std::string& logMessage);
    void HandleInput(const std::string& logInput);

private:
    Context& context;

    static std::list<std::string> m_ConsoleLogBuffer;
    char m_ConsoleInputBuffer[1024] = "";
};


#endif //SLIMSHADY_CONSOLEWIDGET_H
