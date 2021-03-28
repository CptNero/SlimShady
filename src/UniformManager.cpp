//
// Created by ravai on 11/03/2021.
//

#include <GL/glew.h>
#include "UniformManager.h"

UniformManager::UniformManager(Context context) : m_Context(context) {

}

UniformManager::~UniformManager() {

}

int UniformManager::GetUniformLocation(const std::string &name, uint32_t rendererId)
{
  std::map<std::string, int>::const_iterator it = m_UniformLocationCache.find(name);
  if (it == m_UniformLocationCache.end()) {
    int location = glGetUniformLocation(rendererId, name.c_str());
    m_UniformLocationCache[name] = location;

    return location;
  }

  return it->second;
}

void UniformManager::SetUniform1i(uint32_t rendererId, const std::string &name, int value)
{
  glUniform1i(GetUniformLocation(name, rendererId), value);
}

void UniformManager::SetUniform1f(uint32_t rendererId, const std::string &name, float value) {
  glUniform1f(GetUniformLocation(name, rendererId), value);
}

void UniformManager::SetUniform4f(uint32_t rendererId, const std::string &name, float v0, float v1, float v2, float v3)
{
  glUniform4f(GetUniformLocation(name, rendererId), v0, v1, v2, v3);
}

void UniformManager::SetUniformMat4f(uint32_t rendererId, const std::string &name, const glm::mat4 &matrix)
{
  glUniformMatrix4fv(GetUniformLocation(name, rendererId), 1, GL_FALSE, &matrix[0][0]);
}
