#ifndef SLIMSHADY_UNIFORMMANAGER_H
#define SLIMSHADY_UNIFORMMANAGER_H


#include <map>
#include "Camera.h"
#include "Frameworks/Context.h"

class UniformManager {
  public:
    UniformManager(Context context);
    ~UniformManager();

    std::map<std::string, int> &GetUniformLocationCache();

    void SetUniform1i(uint32_t rendererId, const std::string& name, int value);
    void SetUniform1f(uint32_t rendererId, const std::string& name, float value);
    void SetUniform4f(uint32_t rendererId, const std::string& name, float v0, float  v1, float f2, float f3);
    void SetUniformMat4f(uint32_t rendererId, const std::string& name, const glm::mat4 &matrix);

  private:
    Context m_Context;
    std::map<std::string, int> m_UniformLocationCache;

    int GetUniformLocation(const std::string& name, uint32_t rendererId);
};


#endif //SLIMSHADY_UNIFORMMANAGER_H
