#include <imgui/imgui.h>
#include "InputHandler.h"
#include "../Camera.h"

namespace InputHandler {

    void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
      auto context = static_cast<Context*>(glfwGetWindowUserPointer(window));
      if (key == GLFW_KEY_S && action == GLFW_PRESS && mods == GLFW_MOD_CONTROL) {
        ((TextEditorWidget*) context->widgetBroker.GetWidget("TextEditor"))->Save();
      }
      if (key == GLFW_KEY_R && action == GLFW_PRESS && mods == GLFW_MOD_CONTROL) {
        ((SceneEditorWidget*) context->widgetBroker.GetWidget("SceneEditor"))->Recompile();
      }
    }

    void MouseCallback(GLFWwindow *window, double xpos, double ypos) {
      int middleMouseButtonState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE);

      if (middleMouseButtonState == GLFW_PRESS && !ImGui::IsAnyWindowHovered()) {
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

      if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS && !ImGui::IsAnyWindowHovered()) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        if (!Camera::isHeld) {
          glfwSetCursorPos(window, Camera::lastX, Camera::lastY);
          Camera::isHeld = true;
        }
      }

      if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_RELEASE && !ImGui::IsAnyWindowHovered()) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        Camera::isHeld = false;
      }
    }
}
