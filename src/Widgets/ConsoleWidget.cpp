#include <imgui/imgui.h>

#include <iostream>
#include <chrono>

#include "ConsoleWidget.h"
#include "../Clock.h"

char* ConsoleWidget::m_ConsoleLogBuffer = "";

ConsoleWidget::ConsoleWidget() = default;

ConsoleWidget::~ConsoleWidget()
{
  delete m_ConsoleLogBuffer;
}

void ConsoleWidget::OnUpdate(float deltaTime)
{
}

void ConsoleWidget::OnRender()
{
}

void ConsoleWidget::OnImGuiRender()
{
  ImGui::BeginChild("ConsoleLog", ImVec2(ImGui::GetWindowContentRegionWidth(), 260), true);
  ImGui::Text("%s", m_ConsoleLogBuffer);
  ImGui::EndChild();
  ImGui::Separator();

  //Append and handle the user input when the user presses enter
  if(ImGui::InputText("<", m_ConsoleInputBuffer, IM_ARRAYSIZE(m_ConsoleInputBuffer), ImGuiInputTextFlags_EnterReturnsTrue)) {
    HandleInput(m_ConsoleInputBuffer);
    LogInput();
  };
}

void ConsoleWidget::RenderWidget()
{
  OnUpdate(0.0f);
  OnRender();
  ImGui::Begin("Console");
  OnImGuiRender();
  ImGui::End();
}

void ConsoleWidget::HandleInput(char* logInput)
{
  //Todo
}

// Append the user input to the console log then empty the input buffer.
void ConsoleWidget::LogInput()
{
  std::string preparedString = std::string(m_ConsoleLogBuffer) + Clock::GetCurrentTimeAsString() + ": " + m_ConsoleInputBuffer + "\n";

  m_ConsoleLogBuffer = new char[strlen(preparedString.c_str()) + 1];

  strcpy_s(m_ConsoleLogBuffer, strlen(preparedString.c_str()) + 1 ,preparedString.c_str());
  strcpy_s(m_ConsoleInputBuffer, "");
}

//Append a line to the console log.
void ConsoleWidget::LogMessage(const std::string& logMessage)
{
  std::string preparedString = std::string(m_ConsoleLogBuffer) + Clock::GetCurrentTimeAsString() + ": " + logMessage + "\n";

  m_ConsoleLogBuffer = new char[strlen(preparedString.c_str()) + 1];

  strcpy_s(m_ConsoleLogBuffer, strlen(preparedString.c_str()) + 1, preparedString.c_str());
}