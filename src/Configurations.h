#ifndef SLIMSHADY_CONFIGURATIONS_H
#define SLIMSHADY_CONFIGURATIONS_H


#include <string>

class Configurations {
  private:
    static bool m_IsDebugEnabled;
    static const std::string DefaultVertexShaderSource;
    static const std::string DefaultFragmentShaderSource;

  public:
    static bool GetIsDebugEnabled();
    static std::string GetDefaultVertexShaderSource();
    static std::string GetDefaultFragmentShaderSource();
};

#endif //SLIMSHADY_CONFIGURATIONS_H
