#include <imgui/imgui.h>

#include <iostream>

#include "ConsoleWidget.h"
#include "../Frameworks/Clock.h"
#include "SceneEditorWidget.h"
#include "../Camera.h"

std::list<std::string> ConsoleWidget::m_ConsoleLogBuffer;

ConsoleWidget::ConsoleWidget(Context& context) : context(context){}

ConsoleWidget::~ConsoleWidget()
{
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

  for (auto const& consoleLog : m_ConsoleLogBuffer) {
    ImGui::Text("%s", consoleLog.c_str());
  }

  ImGui::EndChild();
  ImGui::Separator();

  //Append and handle the user input when the user presses enter
  if(ImGui::InputText("<", m_ConsoleInputBuffer, IM_ARRAYSIZE(m_ConsoleInputBuffer), ImGuiInputTextFlags_EnterReturnsTrue)) {
    LogInput(m_ConsoleInputBuffer);
    HandleInput(m_ConsoleInputBuffer);
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

void ConsoleWidget::HandleInput(const std::string& logInput)
{
  if(logInput == "clear") {
    m_ConsoleLogBuffer.clear();
  }
  if(logInput == "3dcamera") {
    Camera::Is3DCameraEnabled = !Camera::Is3DCameraEnabled;
    (Camera::Is3DCameraEnabled) ? LogMessage("3D camera is enabled") : LogMessage("3D camera is disabled");
  }
  if(logInput == "reset camera") {
    Camera::ResetCameraPosition();
    LogMessage("Camera position was reset");
  }
}

// Append the user input to the console log then empty the input buffer.
void ConsoleWidget::LogInput(const std::string& logInput)
{
  std::string preparedString = Clock::GetCurrentTimeAsString() + ": " + m_ConsoleInputBuffer + "\n";

  m_ConsoleLogBuffer.emplace_back(preparedString);
}

//Append a line to the console log.
void ConsoleWidget::LogMessage(const std::string& logMessage)
{
  std::string preparedString = Clock::GetCurrentTimeAsString() + ": " + logMessage + "\n";

  m_ConsoleLogBuffer.emplace_back(preparedString);
}
