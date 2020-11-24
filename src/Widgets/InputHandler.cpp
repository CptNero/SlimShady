#include <iostream>
#include <imgui/imgui.h>
#include "InputHandler.h"
#include "../Camera.h"

namespace InputHandler {
    void ProcessHotkeyInput(
            GLFWwindow* window,
            TextEditorWidget* textEditor,
            SceneEditorWidget* sceneEditor) {
      if(ImGui::IsAnyWindowFocused()) {
        if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS &&
            glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
          textEditor->Save();
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS &&
            glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
          sceneEditor->Recompile();
        }
      }
    }

    void MouseCallback(GLFWwindow *window, double xpos, double ypos) {
      int middleMouseButtonState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE);

      if (middleMouseButtonState == GLFW_PRESS && !ImGui::IsAnyWindowHovered()) {
        if (Camera::firstMouse) {
          Camera::lastX = xpos;
          Camera::lastY = ypos;
          Camera::firstMouse = false;
        }

        float xoffset = xpos - Camera::lastX;
        float yoffset = Camera::lastY - ypos; // reversed since y-coordinates go from bottom to top

        Camera::lastX = xpos;
        Camera::lastY = ypos;

        Camera::ProcessMouseMovement(xoffset, yoffset);
      }
    }

    void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
    {
      Camera::ProcessMouseScroll(yoffset);
    }

    void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
      if (button == GLFW_MOUSE_BUTTON_LEFT && !ImGui::IsAnyWindowHovered() && !ImGui::IsAnyItemHovered()) {
        ImGui::SetWindowFocus(nullptr);
      }
    }
}
