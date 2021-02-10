#ifndef SLIMSHADY_FILEMANAGER_H
#define SLIMSHADY_FILEMANAGER_H


#include <string>
#include "../ShaderType.h"
#include "../VertexAttributeType.h"

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
    struct VertexAttributeFile {
        std::string Path;
        std::vector<glm::vec3> Vertices;
        std::vector<uint32_t> Indices;
    };

    static std::string ReadFile(const std::string &filePath);

    static void UpdateFile(const std::string &filePath, const std::string &fileContent);

    static void DeleteFileByPath(const std::string &filePath);

    static std::string GetShaderFilePath(const std::string &sceneElementName, ShaderType shaderType);

    static std::string GetShaderFileNameFromPath(const std::string &filePath);

    static ShaderFile CreateShaderFile(const std::string &sceneElementName, ShaderType shaderType);

    static void DeleteVertexAndFragmentShaderFilesByName(const std::string &sceneElementName);

    static std::string GetVertexAttributeFilePath(const std::string &sceneElementName);

    static std::string GetVertexAttributeFileNameFromPath(const std::string &filePath);

    static VertexAttributeFile CreateVertexAttributeFile(const std::string &sceneElementName);

    static VertexAttributeFile ConvertStringToVertexAttributeFile(const std::string &vertexAttributeData);

    static std::string ConvertVertexAttributeFileToString(VertexAttributeFile vertexAttributeFile);

};

#endif //SLIMSHADY_FILEMANAGER_H
