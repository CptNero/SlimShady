#ifndef SLIMSHADY_TASKWIDGET_H
#define SLIMSHADY_TASKWIDGET_H


#include <string>
#include <glew.h>
#include "Widget.h"
#include "../Frameworks/Context.h"

class TaskWidget : public Widget {
public:
    TaskWidget(Context& context);
    ~TaskWidget();
    void RenderIntoTexture(uint32_t indexBufferSize);
    void LoadTaskImage(const std::string& filePath);

private:
    Context& m_Context;
    GLuint m_TaskTexture;
    GLuint m_RenderedTexture;
    GLuint m_FrameBuffer;
    GLuint m_DepthRenderBuffer;
    int m_TaskImageWidth;
    int m_TaskImageHeight;
    bool m_Layout = true;
    float m_ComparisonResult = 0.0f;
    char m_TaskNameInputBuffer[1024] = "";

    void OnUpdate(float deltaTime) override;
    void OnRender() override;
    void OnImGuiRender() override;
    void RenderWidget() override;

    void InitializeFrameBuffer();
    std::vector<uint8_t> GetTextureData(GLuint texture);
    void ExportImage(std::vector<uint8_t> textureData);
};


#endif //SLIMSHADY_TASKWIDGET_H
