#include <unordered_map>
#include <filesystem>
#include "SceneLoader.h"
#include "Frameworks/Configurations.h"
#include "Frameworks/FileManager.h"

SceneLoader::SceneLoader(std::unordered_map<std::string, SceneElement*>* scene) : m_Scene(scene) {}

SceneLoader::~SceneLoader() = default;

void SceneLoader::InitializeScene()
{
  for (const auto& file : std::filesystem::directory_iterator(Configurations::VertexShaderFilesPath)) {
    std::string filePath = file.path().string();
    std::string fileName = FileManager::GetShaderFileNameFromPath(filePath);

    (*m_Scene)[fileName] = new SceneElement(fileName,
                                            FileManager::ReadFile(FileManager::GetShaderFilePath(fileName, ShaderType::VERTEX)),
                                            FileManager::ReadFile(FileManager::GetShaderFilePath(fileName, ShaderType::FRAGMENT)),
                                            FileManager::ConvertStringToVertexAttributeFile(
                                                    FileManager::ReadFile(
                                                            FileManager::GetVertexAttributeFilePath(fileName))));
  }
}