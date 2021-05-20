#ifndef SLIMSHADY_ATTRIBUTEFILE_H
#define SLIMSHADY_ATTRIBUTEFILE_H

#endif //SLIMSHADY_ATTRIBUTEFILE_H

#include <cereal/types/string.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/list.hpp>

struct AttributeFile {
    std::string Path;
    std::string vertexShaderSource;
    std::string fragmentShaderSource;
    std::vector<float> Vertices;
    std::vector<uint32_t> Indices;
    std::list<std::string> texturePaths;

    template<class Archive>
    void serialize(Archive& archive){
      archive(Path,
              vertexShaderSource,
              fragmentShaderSource,
              Vertices,
              Indices,
              texturePaths);
    }
};