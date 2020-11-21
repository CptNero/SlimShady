#include <iostream>
#include "Camera.h"
#include "Frameworks/Configurations.h"

namespace Camera {
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;

    float Yaw;
    float Pitch;

    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

    float lastX;
    float lastY;

    float deltaTime;
    float lastFrame;

    bool firstMouse;

    void Camera(glm::vec3 position, glm::vec3 up) {
      Camera::Position = position;
      Camera::WorldUp = up;

      Camera::Yaw = YAW;
      Camera::Pitch = PITCH;

      Camera::lastX = Configurations::GetScreenWidth() / 2.0f;
      Camera::lastY = Configurations::GetScreenHeight() / 2.0f;

      Camera::MovementSpeed = 2.5f;
      Camera::MouseSensitivity = 0.1f;
      Camera::Zoom = 45.0f;

      Camera::deltaTime = 0.0f;
      Camera::lastFrame = 0.0f;

      Camera::firstMouse = true;

      UpdateCameraVectors();
    }

    void UpdateCameraTime() {
      float currentFrame = glfwGetTime();
      deltaTime = currentFrame - lastFrame;
      lastFrame = currentFrame;
    }

    void UpdateCameraVectors() {
      // calculate the new Front vector
      glm::vec3 front;
      front.x = cos(glm::radians(Camera::Yaw)) * cos(glm::radians(Camera::Pitch));
      front.y = sin(glm::radians(Camera::Pitch));
      front.z = sin(glm::radians(Camera::Yaw)) * cos(glm::radians(Camera::Pitch));
      Camera::Front = glm::normalize(front);
      // also re-calculate the Right and Up vector
      Camera::Right = glm::normalize(glm::cross(Camera::Front,
                                                Camera::WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
      Camera::Up = glm::normalize(glm::cross(Camera::Right, Camera::Front));
    }

    void ProcessMouseMovement(float xoffset, float yoffset) {
      GLboolean constraintPitch = true;

      xoffset *= MouseSensitivity;
      yoffset *= MouseSensitivity;

      Yaw += xoffset;
      Pitch += yoffset;

      // make sure that when pitch is out of bounds, screen doesn't get flipped
      if (constraintPitch) {
        if (Pitch > 89.0f)
          Pitch = 89.0f;
        if (Pitch < -89.0f)
          Pitch = -89.0f;
      }

      // update Front, Right and Up Vectors using the updated Euler angles
      Camera::UpdateCameraVectors();
    }

    void ProcessMouseScroll(float yoffset) {
      Camera::Zoom -= (float) yoffset;
      if (Camera::Zoom < 1.0f)
        Camera::Zoom = 1.0f;
      if (Camera::Zoom > 45.0f)
        Camera::Zoom = 45.0f;
    }

    void ProcessKeyboard(Camera_Movement direction, float deltaTime) {
      float velocity = MovementSpeed * deltaTime;
      if (direction == FORWARD)
        Position += Front * velocity;
      if (direction == BACKWARD)
        Position -= Front * velocity;
      if (direction == LEFT)
        Position -= Right * velocity;
      if (direction == RIGHT)
        Position += Right * velocity;
    }

    void ProcessInput(GLFWwindow *window) {
      if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

      if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        ProcessKeyboard(FORWARD, deltaTime);
      if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        ProcessKeyboard(BACKWARD, deltaTime);
      if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        ProcessKeyboard(LEFT, deltaTime);
      if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        ProcessKeyboard(RIGHT, deltaTime);
    }

    void MouseCallback(GLFWwindow *window, double xpos, double ypos) {
      int middleMouseButtonState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE);
      if (middleMouseButtonState == GLFW_PRESS) {
        if (firstMouse) {
          lastX = xpos;
          lastY = ypos;
          firstMouse = false;
        }

        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

        lastX = xpos;
        lastY = ypos;

        Camera::ProcessMouseMovement(xoffset, yoffset);
      }
    }

    void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
    {
      ProcessMouseScroll(yoffset);
    }

    glm::mat4 GetViewMatrix() {
      return glm::lookAt(Camera::Position, Camera::Position + Camera::Front, Camera::Up);
    }
}

