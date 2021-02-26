#include <glew.h>
#include <glfw3.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_impl_glfw.h>

#include <iostream>

#include "Frameworks/ErrorHandler.h"
#include "Frameworks/Configurations.h"
#include "Widgets/ConsoleWidget.h"
#include "Widgets/TextEditorWidget.h"
#include "Renderer.h"
#include "Widgets/SceneEditorWidget.h"
#include "SceneLoader.h"
#include "Camera.h"
#include "Frameworks/InputHandler.h"
#include "Widgets/FileBrowserWidget.h"

int main() {
  GLFWwindow* window;

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
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
  glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_TRUE);

  Camera::Camera(glm::vec3(0.0f, 0.0f, 3.0f));
  glfwSetCursorPosCallback(window, InputHandler::MouseCallback);
  glfwSetScrollCallback(window, InputHandler::ScrollCallback);
  glfwSetMouseButtonCallback(window, InputHandler::MouseButtonCallback);
  glfwSetKeyCallback(window, InputHandler::KeyCallback);

  glfwSwapInterval(1);

  if (glewInit() != GLEW_OK)
  {
    std::cout << "Couldn't initialize glew." << std::endl;
  }

  {
    glEnable(GL_BLEND);

    if (Configurations::IsDebugEnabled) {
      glEnable(GL_DEBUG_OUTPUT);
      glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
      glDebugMessageCallback(ErrorHandler::MessageCallback, nullptr);
    }

    glEnable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_ARB_separate_shader_objects);
    glEnable(GL_DEPTH_TEST);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init((char *)glGetString(GL_NUM_SHADING_LANGUAGE_VERSIONS));

    WidgetBroker widgetBroker;

    std::list<SceneElement*> scene;

    Context context(widgetBroker, scene);
    glfwSetWindowUserPointer(window, &context);

    SceneLoader sceneLoader(context);
    sceneLoader.InitializeScene();

    Renderer renderer(context);

    std::list<Widget*> widgetCollection;

    widgetCollection.emplace_back(widgetBroker.MakeWidget<ConsoleWidget>("Console", context));
    widgetCollection.emplace_back(widgetBroker.MakeWidget<TextEditorWidget>("TextEditor", context));
    widgetCollection.emplace_back(widgetBroker.MakeWidget<FileBrowserWidget>("FileBrowser", context));
    widgetCollection.emplace_back(widgetBroker.MakeWidget<SceneEditorWidget>("SceneEditor", context));

    ConsoleWidget::LogMessage("Successfully initialized.");

    while (!glfwWindowShouldClose(window)) {
      glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
      renderer.Clear();

      ImGui_ImplOpenGL3_NewFrame();
      ImGui_ImplGlfw_NewFrame();
      ImGui::NewFrame();

      //Initialize widgets
      for (auto const &widget : widgetCollection) {
        widget->RenderWidget();
      }

      Camera::ProcessCameraInput(window);

      glClear(GL_COLOR_BUFFER_BIT);

      renderer.Draw(scene);

      ImGui::Render();
      ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

      glfwSwapBuffers(window);
      glfwPollEvents();
    }
  }

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwTerminate();
  return 0;
}

