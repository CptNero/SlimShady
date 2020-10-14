#include <imgui/imgui.h>

#include <iostream>
#include <chrono>

#include "ConsoleWidget.h"
#include "../Clock.h"

ConsoleWidget::ConsoleWidget() {

}

ConsoleWidget::~ConsoleWidget() {

}

void ConsoleWidget::OnUpdate(float deltaTime) {
}

void ConsoleWidget::OnRender() {
}

void ConsoleWidget::OnImGuiRender() {
  ImGui::BeginChild("ConsoleLog", ImVec2(ImGui::GetWindowContentRegionWidth(), 260), true);
  ImGui::Text("%s", m_ConsoleLogBuffer);
  ImGui::EndChild();
  ImGui::Separator();

  //Append and handle the user input when the user presses enter
  if(ImGui::InputText("<", m_ConsoleInputBuffer, IM_ARRAYSIZE(m_ConsoleInputBuffer), ImGuiInputTextFlags_EnterReturnsTrue)) {
    HandleInput(m_ConsoleInputBuffer);
    LogInput(m_ConsoleInputBuffer);
  };
}

void ConsoleWidget::InitializeWidget() {
  OnUpdate(0.0f);
  OnRender();
  ImGui::Begin("Console");
  OnImGuiRender();
  ImGui::End();
}

void ConsoleWidget::HandleInput(char* logInput) {
  //Todo
}

// Append the user input to the console log then empty the input buffer.
void ConsoleWidget::LogInput(char *logInput) {
  strcpy_s(m_ConsoleLogBuffer,
           (std::string(m_ConsoleLogBuffer) + Clock::GetCurrentTimeAsString() + ": " + m_ConsoleInputBuffer + "\n").c_str());
  strcpy_s(m_ConsoleInputBuffer, "");
}

//Append a line to the console log.
void ConsoleWidget::LogMessage(char *logMessage) {
  strcpy_s(m_ConsoleLogBuffer,
           (std::string(m_ConsoleLogBuffer) + Clock::GetCurrentTimeAsString() + ": " + m_ConsoleInputBuffer + "\n").c_str());
}
