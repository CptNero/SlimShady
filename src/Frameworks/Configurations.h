#ifndef SLIMSHADY_CONFIGURATIONS_H
#define SLIMSHADY_CONFIGURATIONS_H


#include <string>

namespace Configurations {
    namespace {
        bool IsDebugEnabled = true;
        const std::string VertexShaderFilesPath = R"(src/Resources/VertexShaders\)";
        const std::string FragmentShaderFilesPath = R"(src/Resources/FragmentShaders\)";
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

        const std::string VertexAttributeFilePath = R"(src/Resources/VertexAttributes\)";
        const std::string DefaultVertexAttributeData = R"(
#vertex
0.0 0.0 0.0
#index
0
)";

        float ScreenWidth = 1920.0f;
        float ScreenHeight = 1080.0f;
    }
};

#endif //SLIMSHADY_CONFIGURATIONS_H
