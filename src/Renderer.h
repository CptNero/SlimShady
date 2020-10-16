#ifndef SLIMSHADY_RENDERER_H
#define SLIMSHADY_RENDERER_H


#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

class Renderer {
private:
public:
    Renderer();
    ~Renderer();

    void Clear() const;
    void Draw(const VertexArray& vertexArray, const IndexBuffer& indexBuffer, const Shader& shader);
};


#endif //SLIMSHADY_RENDERER_H
