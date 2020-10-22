#include <glew.h>

#include <fstream>

#include "Shader.h"
#include "Configurations.h"
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

unsigned int Shader::CompileShader(unsigned int type, const std::string &source)
{
  unsigned int id = glCreateShader(type);
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

    if (Configurations::GetIsDebugEnabled()) {
      ConsoleWidget::LogMessage(std::string("Failed to compile ") +
      (type == GL_VERTEX_SHADER ? "vertex " : "fragment ") + "shader" + "\n" + message);
    }
    glDeleteShader(id);
    return 0;
  }

  return id;
}

unsigned int Shader::CreateShader(const std::string &vertexShader, const std::string &fragmentShader)
{
  unsigned int program = glCreateProgram();
  unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
  unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

  glAttachShader(program, vs);
  glAttachShader(program, fs);
  glLinkProgram(program);

  int success;
  char infoLog[512];
  glGetProgramiv(program, GL_LINK_STATUS, &success);

  if(!success) {
    if (Configurations::GetIsDebugEnabled()) {
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

void Shader::SetUniform4f(const std::string &name, float v0, float v1, float v2, float v3)
{
  glUniform4f(GetUniformLocation(name), v0, v1, v2, v3);
}

int Shader::GetUniformLocation(const std::string &name)
{
  if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
    return m_UniformLocationCache[name];

  int location = glGetUniformLocation(m_RendererID, name.c_str());
  if (location == -1)
    if (Configurations::GetIsDebugEnabled()) {
      ConsoleWidget::LogMessage(std::string("Couldn't find uniform " + name));
    }

  m_UniformLocationCache[name] = location;

  return location;
}

void Shader::SetUniform1i(const std::string &name, int value)
{
  glUniform1i(GetUniformLocation(name), value);
}

void Shader::SetUniformMat4f(const std::string &name, const glm::mat4 &matrix)
{
  glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
}


