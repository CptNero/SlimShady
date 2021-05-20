#ifndef SLIMSHADY_FILEMANAGER_H
#define SLIMSHADY_FILEMANAGER_H


#include <string>
#include <fstream>
#include <filesystem>
#include <iostream>
#include <sstream>
#include <vector>
#include <list>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../ShaderType.h"
#include "../VertexAttributeType.h"
#include "../AttributeFile.h"

class FileManager {

public:
    FileManager();

    ~FileManager();

    struct ShaderFile {
        std::string Name;
        std::string ShaderSource;
        std::string Path;
        ShaderType Type;
    };


    static std::string ReadFile(const std::string &filePath);

    static void UpdateFile(const std::string &filePath, const std::string &fileContent);

    static void DeleteFileByPath(const std::string& filePath);

    static std::string GetShaderFilePath(const std::string &sceneElementName, ShaderType shaderType);

    static std::string GetShaderFileNameFromPath(const std::string &filePath);

    static std::string GetTextureFileNameFromPath(const std::string& filePath);

    static std::string GetTaskFileNameFromPath(const std::string& filePath);

    static ShaderType GetShaderTypeFromPath(const std::string& filePath);

    static ShaderFile CreateShaderFile(const std::string &sceneElementName, ShaderType shaderType);

    static void DeleteVertexAndFragmentShaderFilesByName(const std::string &sceneElementName);

    static std::string GetVertexAttributeFilePath(const std::string &sceneElementName);

    static AttributeFile CreateVertexAttributeFile(const std::string &sceneElementName);

    static AttributeFile ConvertStringToVertexAttributeFile(const std::string &vertexAttributeData, const std::string& filePath);

    static std::string ConvertVertexAttributeFileToString(AttributeFile vertexAttributeFile);

    static void ExportTaskFile(std::string name,
                               std::string vertexShaderSource,
                               std::string fragmentShaderSource,
                               AttributeFile file,
                               std::vector<uint8_t> taskImageData);
};

#endif //SLIMSHADY_FILEMANAGER_H
