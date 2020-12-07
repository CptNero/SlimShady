#include "Configurations.h"

bool Configurations::m_IsDebugEnabled{true};
const std::string Configurations::m_VertexShaderFilesPath = R"(src/Resources/VertexShaders\)";
const std::string Configurations::m_FragmentShaderFilesPath = R"(src/Resources/FragmentShaders\)";
const std::string Configurations::m_DefaultVertexShaderSource{R"(
#version 330 core

void main()
{
}
)"};
const std::string Configurations::m_DefaultFragmentShaderSource{R"(
#version 330 core

void main()
{
}
)"};

const std::string Configurations::m_VertexAttributeFilePath = R"(src/Resources/VertexAttributes\)";

const std::string Configurations::m_DefaultVertexAttributeData = R"(
#vertex
0.0 0.0 0.0
#index
0
)";

const float Configurations::m_ScreenWidth = 1920;
const float Configurations::m_ScreenHeight = 1080;

bool Configurations::GetIsDebugEnabled()
{
  return m_IsDebugEnabled;
}

std::string Configurations::GetVertexShaderFilesPath() {
  return m_VertexShaderFilesPath;
}

std::string Configurations::GetFragmentShaderFilesPath() {
  return m_FragmentShaderFilesPath;
}

std::string Configurations::GetDefaultVertexShaderSource() {
  return m_DefaultVertexShaderSource;
}

std::string Configurations::GetDefaultFragmentShaderSource() {
  return m_DefaultFragmentShaderSource;
}

std::string Configurations::GetVertexAttributeFilePath() {
  return m_VertexAttributeFilePath;
}

std::string Configurations::GetDefaultVertexAttributeData() {
  return m_DefaultVertexAttributeData;
}

float Configurations::GetScreenHeight() {
  return m_ScreenHeight;
}

float Configurations::GetScreenWidth() {
  return m_ScreenWidth;
}
