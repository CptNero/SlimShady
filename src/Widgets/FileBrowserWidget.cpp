#include <imgui/imgui.h>
#include "FileBrowserWidget.h"
#include "SceneEditorWidget.h"

FileBrowserWidget::FileBrowserWidget(Context context) : m_Context(context) {
  m_FileBrowser.SetTitle("File Browser");
  m_FileBrowser.SetTypeFilters({".shader", ".jpg", ".png", ".bmp"});
  m_FileBrowser.SetPwd(std::filesystem::current_path().append("Resources"));
}

FileBrowserWidget::~FileBrowserWidget() {

}

void FileBrowserWidget::OnUpdate(float deltaTime) {
}

void FileBrowserWidget::OnRender() {
  CheckForPath();
}

void FileBrowserWidget::OnImGuiRender() {
  m_FileBrowser.Display();
}

void FileBrowserWidget::RenderWidget() {
  OnUpdate(0.0f);
  OnRender();
  OnImGuiRender();
}

void FileBrowserWidget::OpenFileBrowser(FileBrowserAccess type) {
  switch(type) {
    case FileBrowserAccess::VertexShader: {
      m_FileBrowser.SetPwd(std::filesystem::path(Configurations::VertexShaderFilesPath));
      m_FileBrowser.SetTypeFilters({".shader"});
      break;
    }
    case FileBrowserAccess::FragmentShader: {
      m_FileBrowser.SetPwd(std::filesystem::path(Configurations::FragmentShaderFilesPath));
      m_FileBrowser.SetTypeFilters({".shader"});
      break;
    }
    case FileBrowserAccess::Texture: {
      m_FileBrowser.SetPwd(std::filesystem::path(Configurations::TextureFilesPath));
      m_FileBrowser.SetTypeFilters({".jpg", ".png", ".bmp"});
      break;
    }
    case FileBrowserAccess::Task: {
      m_FileBrowser.SetPwd(std::filesystem::path(Configurations::TaskFilesPath));
      m_FileBrowser.SetTypeFilters({".jpg", ".png", ".bmp"});
      break;
    }
  }

  m_LastOpenedBy = type;
  m_FileBrowser.Open();
}

std::string FileBrowserWidget::QueryFileBrowser(FileBrowserAccess type) {
  return m_PathCollection[type];
}

void FileBrowserWidget::CheckForPath() {
  if (m_FileBrowser.HasSelected()) {
    m_PathCollection[m_LastOpenedBy] = m_FileBrowser.GetSelected().string();
    m_FileBrowser.ClearSelected();
  }
}
