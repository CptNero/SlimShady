#ifndef SLIMSHADY_SHADERFILEMANAGER_H
#define SLIMSHADY_SHADERFILEMANAGER_H


#include <string>
#include "../ShaderTypes.h"

class ShaderFileManager {
  private:
      struct ShaderFile {
          std::string Name;
          std::string ShaderSource;
          std::string Path;
          ShaderType ShaderType;
      };
  public:
      ShaderFileManager();
      ~ShaderFileManager();

      static std::string GetShaderFilePath(const std::string& sceneElementName, ShaderType shaderType);
      static std::string GetShaderFileNameFromPath(const std::string& filePath);
      static std::string ReadShaderFile(const std::string& filePath);
      static ShaderFile CreateShaderFile(const std::string& sceneElementName, ShaderType shaderType);
      static void UpdateShaderFile(const std::string& filePath, const std::string& shaderSource);
      static void DeleteShaderFileByPath(const std::string& filePath);
      static void DeleteVertexAndFragmentShaderFilesyName(const std::string& sceneElementName);
  };

#endif //SLIMSHADY_SHADERFILEMANAGER_H
