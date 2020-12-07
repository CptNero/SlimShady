#ifndef SLIMSHADY_CONFIGURATIONS_H
#define SLIMSHADY_CONFIGURATIONS_H


#include <string>

class Configurations {
  private:
    static bool m_IsDebugEnabled;
    static const std::string m_VertexShaderFilesPath;
    static const std::string m_FragmentShaderFilesPath;
    static const std::string m_DefaultVertexShaderSource;
    static const std::string m_DefaultFragmentShaderSource;

    static const std::string m_VertexAttributeFilePath;
    static const std::string m_DefaultVertexAttributeData;

    static const float m_ScreenWidth;
    static const float m_ScreenHeight;

  public:
    static bool GetIsDebugEnabled();
    static std::string GetVertexShaderFilesPath();
    static std::string GetFragmentShaderFilesPath();
    static std::string GetDefaultVertexShaderSource();
    static std::string GetDefaultFragmentShaderSource();

    static std::string GetVertexAttributeFilePath();
    static std::string GetDefaultVertexAttributeData();

    static float GetScreenHeight();
    static float GetScreenWidth();
};

#endif //SLIMSHADY_CONFIGURATIONS_H
