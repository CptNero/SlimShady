#include <fstream>
#include <filesystem>
#include <iostream>
#include "ShaderFileManager.h"
#include "Configurations.h"
#include "../Widgets/ConsoleWidget.h"

ShaderFileManager::ShaderFileManager() = default;

ShaderFileManager::~ShaderFileManager() = default;

std::string ShaderFileManager::GetShaderFilePath(const std::string &sceneElementName, ShaderType shaderType) {
  if (shaderType == ShaderType::VERTEX) {
    return Configurations::GetVertexShaderFilesPath() + sceneElementName + "_Vertex.shader";
  } else if(shaderType == ShaderType::FRAGMENT) {
    return Configurations::GetFragmentShaderFilesPath() + sceneElementName + "_Fragment.shader";
  }
}

std::string ShaderFileManager::GetShaderFileNameFromPath(const std::string &filePath) {
  return filePath.substr(filePath.find('\\') + 1, filePath.find_last_of('_') - filePath.find('\\') - 1);
}

ShaderFileManager::ShaderFile ShaderFileManager::CreateShaderFile(const std::string &sceneElementName, ShaderType shaderType)
{
  ShaderFileManager::ShaderFile shaderFile;
  std::string filePath;
  shaderFile.Name = sceneElementName;

  switch(shaderType){
    case ShaderType::VERTEX:
      filePath = Configurations::GetVertexShaderFilesPath();
      filePath += sceneElementName + "_Vertex.shader";
      break;
    case ShaderType::FRAGMENT:
      filePath = Configurations::GetFragmentShaderFilesPath();
      filePath += sceneElementName + "_Fragment.shader";
      break;
  }

  shaderFile.Path = filePath;
  std::ofstream shaderFileStream(filePath);

  switch(shaderType){
    case ShaderType::VERTEX:
      shaderFile.ShaderType = ShaderType::VERTEX;
      shaderFile.ShaderSource = Configurations::GetDefaultVertexShaderSource();

      shaderFileStream << shaderFile.ShaderSource;
      break;
    case ShaderType::FRAGMENT:
      shaderFile.ShaderType = ShaderType::FRAGMENT;
      shaderFile.ShaderSource = Configurations::GetDefaultFragmentShaderSource();

      shaderFileStream << shaderFile.ShaderSource;
      break;
  }

  shaderFileStream.close();

  ConsoleWidget::LogMessage("Successfully created " + filePath);

  return shaderFile;
}

std::string ShaderFileManager::ReadShaderFile(const std::string &filePath) {
  std::ifstream shaderFile(filePath);
  std::string line;
  std::string shaderSource;

  while (std::getline(shaderFile, line)) {
    shaderSource += line + "\n";
  }

  return shaderSource;
}

void ShaderFileManager::UpdateShaderFile(const std::string &filePath, const std::string &shaderSource) {
  if(!std::filesystem::exists(filePath)) {
    std::cout << "File does not exist at: " << filePath << std::endl;
    return;
  }

  std::ofstream shaderFile(filePath);
  shaderFile << shaderSource;
  shaderFile.close();
}

void ShaderFileManager::DeleteShaderFileByPath(const std::string &filePath) {
  std::remove(filePath.c_str());
}

void ShaderFileManager::DeleteVertexAndFragmentShaderFilesyName(const std::string &sceneElementName) {
  DeleteShaderFileByPath(GetShaderFilePath(sceneElementName, ShaderType::VERTEX));
  DeleteShaderFileByPath(GetShaderFilePath(sceneElementName, ShaderType::FRAGMENT));
}
