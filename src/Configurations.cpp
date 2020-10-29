#include "Configurations.h"

bool Configurations::m_IsDebugEnabled{true};
const std::string Configurations::m_VertexShaderFilesPath = R"(src/res/VertexShaders\)";
const std::string Configurations::m_FragmentShaderFilesPath = R"(src/res/FragmentShaders\)";
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
