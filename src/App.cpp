#include <glew.h>
#include <glfw3.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_impl_glfw.h>

#include <iostream>

#include "ErrorHandler.h"
#include "Configurations.h"
#include "Widgets/ConsoleWidget.h"
#include "Widgets/TextEditorWidget.h"
#include "Renderer.h"
#include "RenderedContent.h"

int main() {
  GLFWwindow *window;
  const auto *configurations = new Configurations;

  if (!glfwInit())
    return -1;

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

  window = glfwCreateWindow(1920, 1080, "SlimShady", NULL, NULL);
  if (!window)
  {
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);

  glfwSwapInterval(1);

  if (glewInit() != GLEW_OK)
  {
    std::cout << "Error!" << std::endl;
  }
  {
    glEnable(GL_BLEND);

    if (configurations->GetIsDebugEnabled()) {
      glEnable(GL_DEBUG_OUTPUT);
      glDebugMessageCallback(ErrorHandler::MessageCallback, nullptr);
    }

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init((char *)glGetString(GL_NUM_SHADING_LANGUAGE_VERSIONS));

    auto renderer = new Renderer();

    auto consoleWidget = new ConsoleWidget;
    auto textEditorWidget = new TextEditorWidget;

    auto renderedContent = new RenderedContent;

    float m_ClearColor[4] = { 0.2f, 0.3f, 0.8f, 1.0f };

    while (!glfwWindowShouldClose(window)) {
      glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
      renderer->Clear();

      ImGui_ImplOpenGL3_NewFrame();
      ImGui_ImplGlfw_NewFrame();
      ImGui::NewFrame();

      //Initialize widgets
      consoleWidget->RenderWidget();
      textEditorWidget->RenderWidget();

      glClearColor(m_ClearColor[0], m_ClearColor[1], m_ClearColor[2], m_ClearColor[3]);
      glClear(GL_COLOR_BUFFER_BIT);

      ImGui::ColorEdit4("Clear color", m_ClearColor, 1);

      //Content
      renderedContent->Draw();

      ImGui::Render();
      ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

      glfwSwapBuffers(window);

      glfwPollEvents();
    }
    delete consoleWidget;
    delete textEditorWidget;
  }

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwTerminate();
  return 0;
}

