#ifndef SLIMSHADY_CONFIGURATIONS_H
#define SLIMSHADY_CONFIGURATIONS_H


#include <string>

namespace Configurations {
    namespace {
        bool IsDebugEnabled = true;
        const std::string VertexShaderFilesPath = R"(Resources/VertexShaders/)";
        const std::string FragmentShaderFilesPath = R"(Resources/FragmentShaders/)";
        const std::string TextureFilesPath = R"(Resources/Textures/)";
        const std::string TaskFilesPath = R"(Resources/Tasks/)";
        const std::string DefaultTaskFilePath = R"(Resources/Tasks/default.png)";
        const std::string DefaultVertexShaderSource = R"(
#version 410 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec3 ourColor;
out vec2 TexCoord;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

void main()
{
}
)";
        const std::string DefaultFragmentShaderSource = R"(
#version 410 core

uniform float u_Time;
uniform int u_Width;
uniform int u_Height;

void main()
{
}
)";

        const std::string VertexAttributeFilePath = R"(Resources/VertexAttributes/)";
        const std::string DefaultVertexAttributeData = R"(
#vertex
0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0
#index
0
#textures
)";

        float ScreenWidth = 1920.0f;
        float ScreenHeight = 1080.0f;
    }
};

#endif //SLIMSHADY_CONFIGURATIONS_H
