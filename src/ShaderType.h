#ifndef SLIMSHADY_SHADERTYPE_H
#define SLIMSHADY_SHADERTYPE_H

enum class ShaderType : uint32_t
{
    NONE = 0,
    VERTEX = 1,
    FRAGMENT = 2
};

inline std::string ShaderTypes[] = {"None", "Vertex", "Fragment"};

#endif //SLIMSHADY_SHADERTYPE_H
