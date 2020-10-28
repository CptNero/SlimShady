#include "Configurations.h"

bool Configurations::m_IsDebugEnabled{true};
const std::string Configurations::DefaultVertexShaderSource{R"(
#version 330 core

layout(location = 0) in vec4 position;

uniform mat4 u_MVP;

void main()
{
    gl_Position = position;
})"};
const std::string Configurations::DefaultFragmentShaderSource{R"(
#version 330 core

precision mediump float;

#define BLACK vec3(0.0, 0.0, 0.0)
#define GREEN vec3(0.2, 1.0, 0.3)

void main()
{
    vec2 uv = floor(gl_FragCoord.xy / 20.0);
    vec4 color;
    bool isEven = mod(uv.x + uv.y, 2.0) == 0.0;
    if(isEven) {
        color = vec4(BLACK, 1.0);
    }
    else {
        color = vec4(GREEN, 1.0);
    }
    gl_FragColor = color;
}
)"};

bool Configurations::GetIsDebugEnabled()
{
  return m_IsDebugEnabled;
}

std::string Configurations::GetDefaultVertexShaderSource() {
  return DefaultVertexShaderSource;
}

std::string Configurations::GetDefaultFragmentShaderSource() {
  return DefaultFragmentShaderSource;
}
