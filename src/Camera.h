#ifndef SLIMSHADY_CAMERA_H
#define SLIMSHADY_CAMERA_H


#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glfw3.h>

enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

namespace Camera {

    extern void Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f));

    extern glm::vec3 Position;
    extern glm::vec3 Front;
    extern glm::vec3 Up;
    extern glm::vec3 Right;
    extern glm::vec3 WorldUp;

    extern float Yaw;
    extern float Pitch;

    extern float MovementSpeed;
    extern float MouseSensitivity;
    extern float Zoom;

    extern float lastX;
    extern float lastY;

    extern float deltaTime;
    extern float lastFrame;

    extern bool isHeld;

    extern void UpdateCameraTime();

    extern void UpdateCameraVectors();

    extern void ProcessMouseMovement(float xoffset, float yoffset);

    extern void ProcessMouseScroll(float yoffset);

    extern void ProcessKeyboard(Camera_Movement direction, float deltaTime);

    extern void ProcessCameraInput(GLFWwindow *window);

    extern glm::mat4 GetViewMatrix();
};


#endif //SLIMSHADY_CAMERA_H
