#ifndef SLIMSHADY_CONFIGURATIONS_H
#define SLIMSHADY_CONFIGURATIONS_H


#include <string>

namespace Configurations {
    namespace {
        bool IsDebugEnabled = true;
        const std::string VertexShaderFilesPath = R"(Resources/VertexShaders\)";
        const std::string FragmentShaderFilesPath = R"(Resources/FragmentShaders\)";
        const std::string TextureFilesPath = R"(Resources/Textures\)";
        const std::string TaskFilesPath = R"(Resources/Tasks\)";
        const std::string DefaultTaskFilePath = R"(Resources/Tasks/default.png)";
        const std::string DefaultVertexShaderSource = R"(
#version 330 core

void main()
{
}
)";
        const std::string DefaultFragmentShaderSource = R"(
#version 330 core

void main()
{
}
)";

        const std::string VertexAttributeFilePath = R"(Resources/VertexAttributes\)";
        const std::string DefaultVertexAttributeData = R"(
#vertex
0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0
#index
0
)";

        float ScreenWidth = 1920.0f;
        float ScreenHeight = 1080.0f;
    }
};

#endif //SLIMSHADY_CONFIGURATIONS_H
