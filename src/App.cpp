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
#include "SceneElement.h"
#include "Widgets/SceneEditorWidget.h"
#include "SceneLoader.h"

int main() {
  GLFWwindow *window;

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
    std::cout << "Couldn't initialize glew." << std::endl;
  }

  {
    glEnable(GL_BLEND);

    if (Configurations::GetIsDebugEnabled()) {
      glEnable(GL_DEBUG_OUTPUT);
      glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
      glDisable(GL_DEBUG_SEVERITY_NOTIFICATION);
      glDebugMessageCallback(ErrorHandler::MessageCallback, nullptr);
    }

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init((char *)glGetString(GL_NUM_SHADING_LANGUAGE_VERSIONS));

    auto scene = new std::unordered_map<std::string, SceneElement*>;
    auto sceneLoader = new SceneLoader(scene);
    sceneLoader->InitializeScene();

    auto renderer = new Renderer();

    auto consoleWidget = new ConsoleWidget;
    auto textEditorWidget = new TextEditorWidget;
    auto sceneEditorWidget = new SceneEditorWidget(scene, textEditorWidget);

    ConsoleWidget::LogMessage("Successfully initialized.");

    while (!glfwWindowShouldClose(window)) {
      glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
      renderer->Clear();

      ImGui_ImplOpenGL3_NewFrame();
      ImGui_ImplGlfw_NewFrame();
      ImGui::NewFrame();

      //Initialize widgets
      consoleWidget->RenderWidget();
      textEditorWidget->RenderWidget();
      sceneEditorWidget->RenderWidget();


      glClear(GL_COLOR_BUFFER_BIT);

      if (!scene->empty()) {
        for (auto const &sceneElement2 : *scene) {
          sceneElement2.second->Draw();
        }
      }

      ImGui::Render();
      ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

      glfwSwapBuffers(window);

      glfwPollEvents();
    }
    delete renderer;
    delete consoleWidget;
    delete textEditorWidget;
    delete sceneLoader;
    delete sceneEditorWidget;
    delete scene;
  }

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwTerminate();
  return 0;
}

