#include <imgui/imgui.h>
#include "InputHandler.h"
#include "../Camera.h"
#include "../Widgets/WidgetBroker.h"

namespace InputHandler {

    void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
      if (key == GLFW_KEY_S && action == GLFW_PRESS && mods == GLFW_MOD_CONTROL) {
        ((TextEditorWidget*) WidgetBroker::GetWidget("TextEditor"))->Save();
      }
      if (key == GLFW_KEY_R && action == GLFW_PRESS && mods == GLFW_MOD_CONTROL) {
        ((SceneEditorWidget*) WidgetBroker::GetWidget("SceneEditor"))->Recompile();
      }
    }

    void MouseCallback(GLFWwindow *window, double xpos, double ypos) {
      int middleMouseButtonState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE);

      if (middleMouseButtonState == GLFW_PRESS && !ImGui::IsAnyWindowHovered()) {
        float xoffset = xpos - Camera::lastX;
        float yoffset = Camera::lastY - ypos; // reversed since y-coordinates go from bottom to top

        if (Camera::firstMouse) {
          Camera::lastX = xpos;
          Camera::lastY = ypos;
          Camera::firstMouse = false;
        }

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
