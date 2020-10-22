#ifndef SLIMSHADY_RENDEREDCONTENT_H
#define SLIMSHADY_RENDEREDCONTENT_H


#include <memory>
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "Shader.h"

class RenderedContent {
  public:
    RenderedContent(const std::string& vertexShaderSource, const std::string& fragmentShaderSource);
    ~RenderedContent();

    void Draw();

  private:
    std::unique_ptr<VertexArray> m_VertexArrayObject;
    std::unique_ptr<IndexBuffer> m_IndexBuffer;
    std::unique_ptr<VertexBuffer> m_VertexBuffer;
    std::unique_ptr<Shader> m_Shader;

    glm::mat4 m_Proj, m_View;
    glm::vec3 m_TranslationA, m_TranslationB;

    std::string m_VertexShaderSource;
    std::string m_FragmentShaderSource;
};


#endif //SLIMSHADY_RENDEREDCONTENT_H
