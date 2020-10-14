#ifndef SLIMSHADY_CONSOLEWIDGET_H
#define SLIMSHADY_CONSOLEWIDGET_H


#include "Widget.h"

class ConsoleWidget : public Widget {
  private:
    char m_ConsoleInputBuffer[1024] = "";
    char m_ConsoleLogBuffer[1024] = "";

  public:
    ConsoleWidget();
    ~ConsoleWidget();

    void OnUpdate(float deltaTime) override;
    void OnRender() override;
    void OnImGuiRender() override;
    void InitializeWidget() override;

    void LogInput(char* logInput);
    void LogMessage(char* logMessage);
    void HandleInput(char* logInput);
};


#endif //SLIMSHADY_CONSOLEWIDGET_H
