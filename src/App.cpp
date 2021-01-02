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
      glDisable(GL_DEBUG_SEVERITY_NOTIFICATION);
      glDebugMessageCallback(ErrorHandler::MessageCallback, nullptr);
    }

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_ARB_separate_shader_objects);
    glEnable(GL_DEPTH_TEST);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init((char *)glGetString(GL_NUM_SHADING_LANGUAGE_VERSIONS));

    auto widgetBroker = new WidgetBroker();

    auto scene = new std::unordered_map<std::string, SceneElement*>;
    auto sceneLoader = new SceneLoader(scene);
    sceneLoader->InitializeScene();

    auto* context = new Context(*widgetBroker);
    glfwSetWindowUserPointer(window, context);

    auto renderer = new Renderer();

    Widget* consoleWidget = widgetBroker->MakeWidget<ConsoleWidget>("Console", *context);
    Widget* textEditorWidget = widgetBroker->MakeWidget<TextEditorWidget>("TextEditor", *context);
    Widget* sceneEditorWidget = widgetBroker->MakeWidget<SceneEditorWidget>("SceneEditor", *context, scene);

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

      Camera::ProcessCameraInput(window);

      glClear(GL_COLOR_BUFFER_BIT);

      if (!scene->empty()) {
        for (auto const &sceneElement : *scene) {
          sceneElement.second->Draw();
        }
      }

      ImGui::Render();
      ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

      glfwSwapBuffers(window);
      glfwPollEvents();
    }
    delete renderer;
    delete sceneLoader;
    delete scene;
    delete widgetBroker;
    delete context;
  }

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwTerminate();
  return 0;
}

