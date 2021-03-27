#include <GL/glew.h>

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

  // At this point we can query the valid/usabe uniforms it will not change
  // By pre-loading and directy returning the position below cpu usage was reduced from ~60% to ~30% on a laptop
  GLint count = 0;
  glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &count);
  printf("Active Uniforms: %d\n", count);

  for (int i = 0; i < count; i++)
  {
    GLint size; // size of the variable
    GLenum type; // type of the variable (float, vec3 or mat4, etc)

    const GLsizei bufSize = 16; // maximum name length
    GLchar name[bufSize]; // variable name in GLSL
    GLsizei length; // name length

    glGetActiveUniform(program, (GLuint)i, bufSize, &length, &size, &type, name);

    m_UniformLocationCache[std::string(name)] = glGetUniformLocation(program, name);
    printf("Uniform #%d Type: %u Name: %s loc: %d\n", i, type, name, m_UniformLocationCache[name] );
  }

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

int Shader::GetUniformLocation(const std::string &name) {
//    std::map<std::string, int>::const_iterator it = m_UniformLocationCache.find(name);
//    if (it == m_UniformLocationCache.end()) {
//        return -1;
//    }
//
//    return it->second;
}
/* // Always checking and reading the uniform name is a big perf hit.
   // find and [] does the same thing (lookup) and thus there will be two lookup in the unordered_map, big perf hit.
  if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
    return m_UniformLocationCache[name];

  int location = glGetUniformLocation(m_RendererID, name.c_str());
  if (location == -1)
    if (Configurations::IsDebugEnabled) {
      ConsoleWidget::LogMessage(std::string("Couldn't find uniform " + name));
    }

  m_UniformLocationCache[name] = location;

  return location;
}
*/

void Shader::SetUniform1i(const std::string &name, int value)
{
  glUniform1i(GetUniformLocation(name), value);
}

void Shader::SetUniform1f(const std::string &name, float value) {
  glUniform1f(GetUniformLocation(name), value);
}

void Shader::SetUniformMat4f(const std::string &name, const glm::mat4 &matrix)
{
  glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
}


