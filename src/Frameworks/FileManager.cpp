#include <regex>
#include "FileManager.h"
#include "../Widgets/ConsoleWidget.h"
#include "Configurations.h"

FileManager::FileManager() = default;

FileManager::~FileManager() = default;

std::string FileManager::GetShaderFilePath(const std::string &sceneElementName, ShaderType shaderType) {
  if (shaderType == ShaderType::VERTEX) {
    return Configurations::VertexShaderFilesPath + sceneElementName + "_Vertex.shader";
  } else if(shaderType == ShaderType::FRAGMENT) {
    return Configurations::FragmentShaderFilesPath + sceneElementName + "_Fragment.shader";
  }
}

std::string FileManager::GetShaderFileNameFromPath(const std::string &filePath) {
  std::regex fileNameRegex(".*Shaders\\/(\\w+)_(Vertex|Fragment)*.");
  std::smatch match;

  std::regex_search(filePath.begin(), filePath.end(), match, fileNameRegex);

  return match[1];
}

ShaderType FileManager::GetShaderTypeFromPath(const std::string& filePath) {
  if (filePath.find("Vertex") != std::string::npos) {
    return ShaderType::VERTEX;
  }
  else if (filePath.find("Fragment") != std::string::npos) {
    return ShaderType::FRAGMENT;
  }
  else {
    return ShaderType::NONE;
  }
}

FileManager::ShaderFile FileManager::CreateShaderFile(const std::string &sceneElementName, ShaderType shaderType)
{
  FileManager::ShaderFile shaderFile;
  std::string filePath;
  shaderFile.Name = sceneElementName;

  switch(shaderType){
    case ShaderType::VERTEX:
      filePath = Configurations::VertexShaderFilesPath;
      filePath += sceneElementName + "_Vertex.shader";
      break;
    case ShaderType::FRAGMENT:
      filePath = Configurations::FragmentShaderFilesPath;
      filePath += sceneElementName + "_Fragment.shader";
      break;
  }

  shaderFile.Path = filePath;
  std::ofstream shaderFileStream(filePath);

  switch(shaderType){
    case ShaderType::VERTEX:
      shaderFile.Type = ShaderType::VERTEX;
      shaderFile.ShaderSource = Configurations::DefaultVertexShaderSource;

      shaderFileStream << shaderFile.ShaderSource;
      break;
    case ShaderType::FRAGMENT:
      shaderFile.Type = ShaderType::FRAGMENT;
      shaderFile.ShaderSource = Configurations::DefaultFragmentShaderSource;

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
  return Configurations::VertexAttributeFilePath + sceneElementName + ".vrtxatrb";
}

FileManager::VertexAttributeFile FileManager::CreateVertexAttributeFile(const std::string& sceneElementName) {
  VertexAttributeFile vertexAttributeFile;
  std::string filePath;

  filePath += Configurations::VertexAttributeFilePath + sceneElementName + ".vrtxatrb";
  vertexAttributeFile.Path = filePath;

  std::ofstream shaderFileStream(filePath);

  shaderFileStream << Configurations::DefaultVertexAttributeData;

  shaderFileStream.close();

  vertexAttributeFile.Vertices.insert(vertexAttributeFile.Vertices.end(), {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f});
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
    else if (line.find("#texture") != std::string::npos) {
      type = VertexAttributeType::TEXTURE;
      continue;
    }

    switch (type) {
      case VertexAttributeType::VERTEX: {
        std::stringstream vertexStringStream(line);
        std::string vertexLine;

        while (std::getline(vertexStringStream, vertexLine, ' ')) {
          vertexAttributeFile.Vertices.emplace_back(std::stof(vertexLine));
        }

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

      case VertexAttributeType::TEXTURE: {
        vertexAttributeFile.texturePaths.emplace_back(line);
        break;
      }
    }
  }

  return vertexAttributeFile;
}

std::string FileManager::ConvertVertexAttributeFileToString(FileManager::VertexAttributeFile vertexAttributeFile) {
  std::stringstream vertexAttributeFileStringStream;
  vertexAttributeFileStringStream << "#vertex" << std::endl;
  uint32_t counter = 0;

  std::for_each(vertexAttributeFile.Vertices.begin(), vertexAttributeFile.Vertices.end(), [&](float vertex) {
    vertexAttributeFileStringStream << vertex << " ";
    counter++;

    if(counter == 8) {
      counter = 0;
      vertexAttributeFileStringStream << std::endl;
    }
  });
  vertexAttributeFileStringStream << "#index" << std::endl;

  std::for_each(vertexAttributeFile.Indices.begin(), vertexAttributeFile.Indices.end(), [&](uint32_t index) {
    vertexAttributeFileStringStream << index << " ";
  });
  // Remove the last separator before terminating the line;
  vertexAttributeFileStringStream.seekp(-1, std::ios_base::end);
  vertexAttributeFileStringStream << std::endl;

  vertexAttributeFileStringStream << "#textures" << std::endl;

  std::for_each(vertexAttributeFile.texturePaths.begin(), vertexAttributeFile.texturePaths.end(), [&](const std::string& texturePath) {
    vertexAttributeFileStringStream << texturePath << std::endl;
  });

  return vertexAttributeFileStringStream.str();
}
