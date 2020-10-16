#include <glew.h>

#include <fstream>
#include <iostream>
#include <sstream>

#include "Shader.h"

Shader::Shader(const std::string& file_path)
        : m_RendererID(0)
{
  m_VertexShaderFilePath = R"(res/shaders/Vertex.shader)";
  m_FragmentShaderFilePath = R"(res/shaders/Fragment.Shader)";

  m_RendererID = CreateShader(
          ParseShader(m_VertexShaderFilePath),
          ParseShader(m_FragmentShaderFilePath));
}

Shader::~Shader()
{
  glDeleteProgram(m_RendererID);
}

std::string Shader::ParseShader(const std::string& shaderFilePath)
{
  std::ifstream shaderStream(shaderFilePath);
  std::string line;
  std::stringstream stringStream[1];

  while (getline(shaderStream, line)) {
    stringStream[0] << line << '\n';
  }

  return stringStream[0].str();
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
    std::cout << "Failed to" << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << "compile shader!" << std::endl;
    std::cout << message << std::endl;
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
    std::cout << "Warning: uniform '" << name << "doesn't exist!" << std::endl;

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

