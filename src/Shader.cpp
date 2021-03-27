#include <glew.h>

#include <fstream>

#include "Shader.h"
#include "Frameworks/Configurations.h"
#include "Widgets/ConsoleWidget.h"

Shader::Shader(const std::string& vertexShaderSource, const std::string& fragmentShaderSource)
        : m_RendererID(0)
{
  m_RendererID = CreateShader(
          vertexShaderSource,
          fragmentShaderSource);
}

Shader::~Shader()
{
  glDeleteProgram(m_RendererID);
}

uint32_t Shader::CompileShader(uint32_t type, const std::string &source)
{
  uint32_t id = glCreateShader(type);
  const char *src = source.c_str();
  glShaderSource(id, 1, &src, nullptr);
  glCompileShader(id);

  int result;
  glGetShaderiv(id, GL_COMPILE_STATUS, &result);
  if (result == GL_FALSE)
  {
    int length;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
    char *message = (char *) alloca(length * sizeof(char));
    glGetShaderInfoLog(id, length, &length, message);

    if (Configurations::IsDebugEnabled) {
      ConsoleWidget::LogMessage(std::string("Failed to compile ") +
      (type == GL_VERTEX_SHADER ? "vertex " : "fragment ") + "shader" + "\n" + message);
    }
    glDeleteShader(id);
    return 0;
  }

  return id;
}

uint32_t Shader::CreateShader(const std::string &vertexShader, const std::string &fragmentShader)
{
  uint32_t program = glCreateProgram();
  uint32_t vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
  uint32_t fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

  glAttachShader(program, vs);
  glAttachShader(program, fs);
  glLinkProgram(program);

  int success;
  char infoLog[512];
  glGetProgramiv(program, GL_LINK_STATUS, &success);

  if(!success) {
    if (Configurations::IsDebugEnabled) {
      ConsoleWidget::LogMessage(std::string("Failed to create shader: ") + infoLog);
    }
  }

  glValidateProgram(program);

  glDeleteShader(vs);
  glDeleteShader(fs);

  return program;
}

void Shader::Bind() const
{
  glUseProgram(m_RendererID);
}

void Shader::UnBind() const
{
  glUseProgram(0);
}

uint32_t Shader::GetRendererId() const {
  return m_RendererID;
}



