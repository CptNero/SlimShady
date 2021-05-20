#ifndef SLIMSHADY_TASKWIDGET_H
#define SLIMSHADY_TASKWIDGET_H


#include <string>
#include <GL/glew.h>
#include "Widget.h"
#include "../Frameworks/Context.h"
#include "../FrameBuffer.h"

class TaskWidget : public Widget {
public:
    TaskWidget(Context& context);
    ~TaskWidget();
    void LoadTask(const std::string& filePath);
    void LoadTaskImage(const std::string& filePath);
    FrameBuffer& GetTaskFrameBuffer();
    FrameBuffer& GetRenderedFrameBuffer();

private:
    Context& m_Context;
    FrameBuffer m_RenderedFrameBuffer;
    FrameBuffer m_TaskFrameBuffer;
    int m_TaskImageWidth;
    int m_TaskImageHeight;
    bool m_Layout = false;
    float m_ChiSquareDistanceResult = 0.0f;
    double m_SSIMComparisonResult = 0.0f;
    std::string m_SelectedTaskName;

    void OnUpdate(float deltaTime) override;
    void OnRender() override;
    void OnImGuiRender() override;
    void RenderWidget() override;

    std::vector<uint8_t> GetTextureData(GLuint texture);
    void ExportImage(std::vector<uint8_t> textureData, const std::string& name);
};


#endif //SLIMSHADY_TASKWIDGET_H
