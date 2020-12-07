#include <fstream>
#include <filesystem>
#include <iostream>
#include <sstream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "FileManager.h"
#include "Configurations.h"
#include "../Widgets/ConsoleWidget.h"

FileManager::FileManager() = default;

FileManager::~FileManager() = default;

std::string FileManager::GetShaderFilePath(const std::string &sceneElementName, ShaderType shaderType) {
  if (shaderType == ShaderType::VERTEX) {
    return Configurations::GetVertexShaderFilesPath() + sceneElementName + "_Vertex.shader";
  } else if(shaderType == ShaderType::FRAGMENT) {
    return Configurations::GetFragmentShaderFilesPath() + sceneElementName + "_Fragment.shader";
  }
}

std::string FileManager::GetShaderFileNameFromPath(const std::string &filePath) {
  return filePath.substr(filePath.find('\\') + 1, filePath.find_last_of('_') - filePath.find('\\') - 1);
}

FileManager::ShaderFile FileManager::CreateShaderFile(const std::string &sceneElementName, ShaderType shaderType)
{
  FileManager::ShaderFile shaderFile;
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

std::string FileManager::ReadFile(const std::string &filePath) {
  std::ifstream file(filePath);
  std::string line;
  std::string fileContent;

  while (std::getline(file, line)) {
    fileContent += line + "\n";
  }

  return fileContent;
}

void FileManager::UpdateFile(const std::string &filePath, const std::string &fileContent) {
  if(!std::filesystem::exists(filePath)) {
    std::cout << "File does not exist at: " << filePath << std::endl;
    return;
  }

  std::ofstream file(filePath);
  file << fileContent;
  file.close();
}

void FileManager::DeleteFileByPath(const std::string &filePath) {
  std::remove(filePath.c_str());
}

void FileManager::DeleteVertexAndFragmentShaderFilesByName(const std::string &sceneElementName) {
  DeleteFileByPath(GetShaderFilePath(sceneElementName, ShaderType::VERTEX));
  DeleteFileByPath(GetShaderFilePath(sceneElementName, ShaderType::FRAGMENT));
}

std::string FileManager::GetVertexAttributeFilePath(const std::string& sceneElementName) {
  return Configurations::GetVertexAttributeFilePath() + sceneElementName + ".vrtxatrb";
}

std::string FileManager::GetVertexAttributeFileNameFromPath(const std::string &filePath) {
  return filePath.substr(filePath.find('\\') + 1, filePath.find_last_of('.') - filePath.find('\\') - 1);
}

FileManager::VertexAttributeFile FileManager::CreateVertexAttributeFile(const std::string& sceneElementName) {
  VertexAttributeFile vertexAttributeFile;
  std::string filePath;

  filePath += Configurations::GetVertexAttributeFilePath() + sceneElementName + ".vrtxatrb";
  vertexAttributeFile.Path = filePath;

  std::ofstream shaderFileStream(filePath);

  shaderFileStream << Configurations::GetDefaultVertexAttributeData();

  shaderFileStream.close();

  vertexAttributeFile.Vertices.emplace_back(glm::vec3(0.0f, 0.0f, 0.0f));
  vertexAttributeFile.Indices.emplace_back(0);

  return vertexAttributeFile;
}

FileManager::VertexAttributeFile FileManager::ConvertStringToVertexAttributeFile(const std::string& vertexAttributeData) {
  VertexAttributeFile vertexAttributeFile;
  std::stringstream vertexAttributeFileStringStream(vertexAttributeData);
  std::string line;
  VertexAttributeType type = VertexAttributeType::NONE;

  while (std::getline(vertexAttributeFileStringStream, line, '\n')) {
    if (line.find("#vertex") != std::string::npos) {
      type = VertexAttributeType::VERTEX;
      continue;
    } else if (line.find("#index") != std::string::npos) {
      type = VertexAttributeType::INDEX;
      continue;
    }

    switch (type) {
      case VertexAttributeType::VERTEX: {
        std::stringstream vertexStringStream(line);
        std::string vertexLine;
        std::vector<float> vertexVector;

        while (std::getline(vertexStringStream, vertexLine, ' ')) {
          vertexVector.emplace_back(std::stof(vertexLine));
        }

        vertexAttributeFile.Vertices.emplace_back(glm::make_vec3(&vertexVector[0]));
        break;
      }

      case VertexAttributeType::INDEX: {
        std::stringstream indexStringStream(line);
        std::string indexLine;
        while (std::getline(indexStringStream, indexLine, ' ')) {
          vertexAttributeFile.Indices.emplace_back(std::stoi(indexLine));
        }
        break;
      }
    }
  }

  return vertexAttributeFile;
}

std::string FileManager::ConvertVertexAttributeFileToString(FileManager::VertexAttributeFile vertexAttributeFile) {
  std::stringstream vertexAttributeFileStringStream;
  vertexAttributeFileStringStream << "#vertex" << std::endl;

  std::for_each(vertexAttributeFile.Vertices.begin(), vertexAttributeFile.Vertices.end(), [&](glm::vec3 vertex) {
    vertexAttributeFileStringStream << vertex.x << " " << vertex.y << " " << vertex.z << std::endl;
  });
  vertexAttributeFileStringStream << "#index" << std::endl;

  std::for_each(vertexAttributeFile.Indices.begin(), vertexAttributeFile.Indices.end(), [&](uint32_t index) {
    vertexAttributeFileStringStream << index << " ";
  });
  // Remove the last separator before terminating the line;
  vertexAttributeFileStringStream.seekp(-1, std::ios_base::end);
  vertexAttributeFileStringStream << std::endl;

  return vertexAttributeFileStringStream.str();
}
