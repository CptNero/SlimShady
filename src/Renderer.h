#ifndef SLIMSHADY_RENDERER_H
#define SLIMSHADY_RENDERER_H


#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Frameworks/Context.h"

class Renderer {
private:
    Context m_Context;

    glm::mat4 m_Projection;
    glm::mat4 m_View;
    glm::mat4 m_Model;

public:
    explicit Renderer(Context context);
    ~Renderer();

    void Clear() const;
    void Draw(std::list<SceneElement*>& scene);
    void UpdateUniforms(SceneElement& sceneElement);
};


#endif //SLIMSHADY_RENDERER_H
