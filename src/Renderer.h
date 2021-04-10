#ifndef SLIMSHADY_RENDERER_H
#define SLIMSHADY_RENDERER_H


#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Frameworks/Context.h"
#include "UniformManager.h"

class Renderer {
public:
    Renderer(Context context, UniformManager& uniformManager);
    ~Renderer();

    void Clear() const;
    void Draw(std::list<SceneElement*>& scene, std::list<SceneElement*> taskScene);
    void UpdateUniforms(SceneElement& sceneElement);

private:
    Context m_Context;
    UniformManager& m_UniformManager;

    glm::mat4 m_Projection;
    glm::mat4 m_View;
    glm::mat4 m_Model;
};


#endif //SLIMSHADY_RENDERER_H
