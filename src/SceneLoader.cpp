#include <unordered_map>
#include <filesystem>
#include "SceneLoader.h"
#include "Configurations.h"
#include "Frameworks/ShaderFileManager.h"

SceneLoader::SceneLoader(std::unordered_map<std::string, SceneElement*>* scene) : m_Scene(scene) {}

SceneLoader::~SceneLoader() = default;

void SceneLoader::InitializeScene()
{
  for (const auto& file : std::filesystem::directory_iterator(Configurations::GetVertexShaderFilesPath())) {
    std::string filePath = file.path().string();
    std::string fileName = ShaderFileManager::GetShaderFileNameFromPath(filePath);

    (*m_Scene)[fileName] = new SceneElement(fileName,
                                            ShaderFileManager::ReadShaderFile(ShaderFileManager::GetShaderFilePath(fileName, ShaderType::VERTEX)),
                                            ShaderFileManager::ReadShaderFile(ShaderFileManager::GetShaderFilePath(fileName, ShaderType::FRAGMENT)));
  }
}