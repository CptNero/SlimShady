#include <unordered_map>
#include <filesystem>
#include "SceneLoader.h"
#include "Frameworks/Configurations.h"
#include "Frameworks/FileManager.h"

SceneLoader::SceneLoader(Context context) : m_Context(context) {}

SceneLoader::~SceneLoader() = default;

void SceneLoader::InitializeScene()
{
  for (const auto& file : std::filesystem::directory_iterator(Configurations::VertexShaderFilesPath)) {
    std::string filePath = file.path().string();
    std::string fileName = FileManager::GetShaderFileNameFromPath(filePath);

    m_Context.scene[fileName] = new SceneElement(fileName,
                                        FileManager::ReadFile(FileManager::GetShaderFilePath(fileName, ShaderType::VERTEX)),
                                        FileManager::ReadFile(FileManager::GetShaderFilePath(fileName, ShaderType::FRAGMENT)),
                                        FileManager::ConvertStringToVertexAttributeFile(
                                                FileManager::ReadFile(
                                                        FileManager::GetVertexAttributeFilePath(fileName))));

  }
}