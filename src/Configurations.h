#ifndef SLIMSHADY_CONFIGURATIONS_H
#define SLIMSHADY_CONFIGURATIONS_H


class Configurations {
  private:
    static bool m_IsDebugEnabled;
    static const char* VertexShaderSourcePath;
    static const char* FragmentShaderSourcePath;

  public:
    static bool GetIsDebugEnabled();
    static const char* GetVertexShaderSourcePath();
    static const char* GetFragmentShaderSourcePath();
};

#endif //SLIMSHADY_CONFIGURATIONS_H
