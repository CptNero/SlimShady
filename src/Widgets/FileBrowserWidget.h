
#ifndef SLIMSHADY_FILEBROWSERWIDGET_H
#define SLIMSHADY_FILEBROWSERWIDGET_H

#include "imfilebrowser/imfilebrowser.h"
#include "Widget.h"
#include "../Frameworks/Context.h"

class FileBrowserWidget : public Widget {
public:
    enum FileBrowserAccess : int {
        None = 0,
        VertexShader = 1,
        FragmentShader = 2,
        Texture = 3,
        Task = 4,
    };

    ImGui::FileBrowser m_FileBrowser;
    FileBrowserAccess m_LastOpenedBy;

    FileBrowserWidget(Context context);
    ~FileBrowserWidget();

    void OpenFileBrowser(FileBrowserAccess type);
    std::string QueryFileBrowser(FileBrowserAccess type);
    bool HasSelected(FileBrowserAccess type);

private:
    Context m_Context;
    std::unordered_map<FileBrowserAccess, std::string> m_PathCollection;

    void OnUpdate(float deltaTime) override;
    void OnRender() override;
    void OnImGuiRender() override;
    void RenderWidget() override;
    void CheckForPath();
};


#endif //SLIMSHADY_FILEBROWSERWIDGET_H
