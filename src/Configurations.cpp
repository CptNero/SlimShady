#include "Configurations.h"

bool Configurations::m_IsDebugEnabled{true};
const char* Configurations::VertexShaderSourcePath{R"(src/res/shaders/Vertex.shader)"};
const char* Configurations::FragmentShaderSourcePath{R"(src/res/shaders/Fragment.shader)"};

bool Configurations::GetIsDebugEnabled()
{
  return m_IsDebugEnabled;
}

const char* Configurations::GetVertexShaderSourcePath() {
  return VertexShaderSourcePath;
}

const char* Configurations::GetFragmentShaderSourcePath() {
  return FragmentShaderSourcePath;
}
