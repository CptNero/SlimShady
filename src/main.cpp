#include <glew.h>
#include <glfw3.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_impl_glfw.h>

#include <iostream>

#include "ErrorHandler.h"
#include "tests/Test.h"
#include "tests/TestClearColor.h"
#include "Configurations.h"

int main() {
  GLFWwindow *window;
  const Configurations* configurations = new Configurations(true);

  if (!glfwInit())
    return -1;

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window = glfwCreateWindow(960, 540, "OpenGL Template", NULL, NULL);
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

    test::Test* currentTest = nullptr;
    test::TestMenu* testMenu = new test::TestMenu(currentTest);
    currentTest = testMenu;

    testMenu->RegisterTest<test::TestClearColor>("Clear color");

    while (!glfwWindowShouldClose(window)) {
      glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

      ImGui_ImplOpenGL3_NewFrame();
      ImGui_ImplGlfw_NewFrame();
      ImGui::NewFrame();

      if (currentTest)
      {
        currentTest->OnUpdate(0.0f);
        currentTest->OnRender();
        ImGui::Begin("Test");
        if (currentTest != testMenu && ImGui::Button("<-"))
        {
          delete currentTest;
          currentTest = testMenu;
        }
        currentTest->OnImGuiRender();
        ImGui::End();
      }

      ImGui::Render();
      ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

      glfwSwapBuffers(window);

      glfwPollEvents();
    }
    delete currentTest;

    if (currentTest != testMenu)
      delete testMenu;
  }

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwTerminate();
  return 0;
}

