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
#include "Frameworks/InputHandler.h"
#include "Widgets/FileBrowserWidget.h"
#include "Widgets/TaskWidget.h"

int main() {
  GLFWwindow* window;

  if (!glfwInit())
    return -1;

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

  // TODO: maybe start by default in a smaller window? some laptops have 1366x768 resolution
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

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    // TODO: the GL_NUM_SHADING_LANGUAGE_VERSIONS is invalid for the glGetString, it is a number.
    //ImGui_ImplOpenGL3_Init((char *)glGetStringi(GL_SHADING_LANGUAGE_VERSION, 0)); //GL_NUM_SHADING_LANGUAGE_VERSIONS));
    ImGui_ImplOpenGL3_Init("#version 410"); // WORKAROUND for now
    //std::cout << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

    WidgetBroker widgetBroker;

    std::list<SceneElement*> scene;

    Context context(widgetBroker, scene);
    glfwSetWindowUserPointer(window, &context);

    SceneLoader sceneLoader(context);
    sceneLoader.InitializeScene();

    UniformManager uniformManager(context);
    Renderer renderer(context, uniformManager);

    std::list<Widget*> widgetCollection;

    widgetCollection.emplace_back(widgetBroker.MakeWidget<ConsoleWidget>("Console", context));
    widgetCollection.emplace_back(widgetBroker.MakeWidget<TextEditorWidget>("TextEditor", context));
    widgetCollection.emplace_back(widgetBroker.MakeWidget<FileBrowserWidget>("FileBrowser", context));
    widgetCollection.emplace_back(widgetBroker.MakeWidget<SceneEditorWidget>("SceneEditor", context, uniformManager));
    widgetCollection.emplace_back(widgetBroker.MakeWidget<TaskWidget>("TaskWidget", context));

    ConsoleWidget::LogMessage("Successfully initialized.");

    glfwSwapInterval(1);    // we can have vsync saves CPU time
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

